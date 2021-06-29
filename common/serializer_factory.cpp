#include "serializer_factory.h"
#include "json_serializer.h"
using namespace std;

shared_ptr<Serializer> SerializerFactory::_inst = nullptr;

SerializerFactory::~SerializerFactory()
{}

shared_ptr<Serializer> SerializerFactory::get()
{
    if(!_inst) _inst = make_shared<JsonSerializer>();
    return _inst;
}
