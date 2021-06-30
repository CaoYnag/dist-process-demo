#include <iostream>
#include <zmq.h>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <sys/inotify.h>
#include <unistd.h>
#include "../common/common.h"
using namespace std;

constexpr const int EVENT_NUM = 12;
char *event_str[EVENT_NUM] =
		{
				"IN_ACCESS",
				"IN_MODIFY",
				"IN_ATTRIB",
				"IN_CLOSE_WRITE",
				"IN_CLOSE_NOWRITE",
				"IN_OPEN",
				"IN_MOVED_FROM",
				"IN_MOVED_TO",
				"IN_CREATE",
				"IN_DELETE",
				"IN_DELETE_SELF",
				"IN_MOVE_SELF"
		};

class Dispacher
{
    void* ctx;
    void* ss; // ventilator
    void* sc; // sink
    atomic<bool> running;
	shared_ptr<Serializer> serializer;
	int fd, wd;
	char buff[BUFSIZ];
public:
    Dispacher()
	{
		ctx = zmq_ctx_new();
		ss = zmq_socket(ctx, ZMQ_PUSH);
		sc = zmq_socket(ctx, ZMQ_PULL);

		if(zmq_bind(ss, "tcp://0.0.0.0:11111"))
		{
			cerr << "[disp] bind push port failed: " << zmq_strerror(zmq_errno()) << endl;
			ss = nullptr;
			return;
		}

		if(zmq_bind(sc, "tcp://0.0.0.0:11112"))
		{
			cerr << "[disp] bind pull port failed: " << zmq_strerror(zmq_errno()) << endl;
			sc = nullptr;
			return;
		}
		serializer = SerializerFactory::get();
	}
	~Dispacher()
	{
		if(ss) zmq_close(ss);
		if(sc) zmq_close(sc);
		if(ctx) zmq_ctx_term(ctx);
	}

	void dispatch(shared_ptr<task_t> t)
	{
    	auto data = serializer->serialize(t);
		zmq_send(ss, data.c_str(), data.size(), 0);
	}
	void daemon()
	{
		char buff[2048];
		running = true;
		while(running)
		{
			auto cnt = zmq_recv(sc, buff, 2048, 0);
			if(cnt)
			{
				string msg(buff, cnt);
				auto task = serializer->deserialize(msg);
				// task done or health check
			}
		}
	}
	int watch(const string& path)
	{
		fd = inotify_init();
		if(fd < 0)
		{
			cerr << "[disp] init inotify failed." << endl;
			return 1;
		}
		wd = inotify_add_watch(fd, path.c_str(), IN_ALL_EVENTS);
		if(wd < 0)
		{
			cerr << "[disp] watch " << path << " failed." << endl;
			return 1;
		}
		int len = 0;
		buff[sizeof(buff) - 1] = 0;
		inotify_event* ev;
		while((len = read(fd, buff, sizeof(buff) - 1)) > 0)
		{
			int nread = 0;
			while(len > 0)
			{
				ev = (inotify_event*)(buff + nread);
				for (int i = 0; i < EVENT_NUM; i++)
				{
					if ((ev->mask >> i) & 1)
					{
						if (ev->len > 0)
							fprintf(stdout, "%s --- %s\n", ev->name, event_str[i]);
						else
							fprintf(stdout, "------ %s\n", event_str[i]);
					}
				}
				nread = nread + sizeof(inotify_event) + ev->len;
				len = len - sizeof(inotify_event) - ev->len;
			}
		}
	}
};

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cerr << "args not enough." << endl;
		return 1;
	}

	Dispacher disp;
	disp.watch(argv[1]);
	return 0;
}
