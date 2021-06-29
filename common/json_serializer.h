#pragma once
#include "serializer.h"
using namespace std;

class JsonSerializer : public Serializer
{
public:
    virtual ~JsonSerializer();

    virtual string serialize(shared_ptr<task_t> task);
    virtual shared_ptr<task_t> deserialize(const string& data);
};
