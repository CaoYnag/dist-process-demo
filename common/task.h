#pragma once
#include "types.h"
#include <string>
using namespace std;

struct task_t
{
    u64 id;
    u32 state;
    string name;
    string conf;

    task_t(u64, u32, const string&, const string&);
};
