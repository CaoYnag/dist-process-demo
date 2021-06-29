#include <iostream>
#include <zmq.h>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <atomic>
#include "../common/common.h"
using namespace std;

class Dispacher
{
    void* ctx;
    void* ss; // ventilator
    void* sc; // sink
    atomic<bool> running;
	shared_ptr<Serializer> serializer;
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

	void daemon()
	{
		char buff[2048];
		running = true;
		while(running)
		{
			auto cnt = zmq_recv(s, buff, 2048, 0);
			if(cnt)
			{
				string msg(buff, cnt);
				auto task = serializer->deserialize(msg);
				// task done or health check
			}
		}
	}
};

int main()
{
	auto ctx = zmq_ctx_new();
	auto s = zmq_socket(ctx, ZMQ_PUSH);
	if(zmq_bind(s, "tcp://0.0.0.0:11111"))
	{
		cerr << "bind tcp failed: " << zmq_strerror(zmq_errno()) << endl;
		zmq_ctx_term(ctx);
		return 1;
	}
	char buff[2048];
	cout << "[vent] listening " << 11111 << endl;

	int cnt = 0;
	while(true)
	{
		sprintf(buff, "t_%d", ++cnt);
		zmq_send(s, buff, strlen(buff), 0);
		cout << "[vent] dispathed: " << buff << endl;
		zmq_sleep(2);
	}
}
