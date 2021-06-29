#pragma once
#include "serializer.h"
#include <memory>
using namespace std;

class SerializerFactory
{
    static shared_ptr<Serializer> _inst;
    SerializerFactory() = delete;
public:
    ~SerializerFactory();

    shared_ptr<Serializer> get();
};
