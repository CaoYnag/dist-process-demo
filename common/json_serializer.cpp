#include "json_serializer.h"
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

JsonSerializer::~JsonSerializer()
{}


string JsonSerializer::serialize(shared_ptr<task_t> task)
{
    json j;
    j["id"] = task->id;
    j["state"] = task->state;
    j["name"] = task->name;
    j["conf"] = task->conf;
    return j.dump(4);
}

shared_ptr<task_t> JsonSerializer::deserialize(const string& data)
{
    json j = json::parse(data);
    return make_shared<task_t>(j["id"], j["state"], j["name"], j["conf"]);
}

