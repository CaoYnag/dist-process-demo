#include "task.h"

task_t::task_t(u64 iid, u32 sstate, const string& nname, const string& cconf)
    : id(iid), state(sstate), name(nnmae), conf(cconf)
{}
