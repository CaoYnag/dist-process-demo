#pragma once
#include "task.h"
#include <string>
#include <memory>
using namespace std;

class Serializer
{
public:
    virtual ~Serializer();

    virtual string serialize(shared_ptr<task_t> task) = 0;
    virtual shared_ptr<task_t> deserialize(const string& data) = 0;
};

