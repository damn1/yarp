/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * Authors: Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <yarp/os/RFModuleFactory.h>
#include <map>
using namespace std;
using namespace yarp::os;

struct yarp::os::RFModuleFactory::Private
{
    map<string, RFModule*> delegates;
};

RFModuleFactory::RFModuleFactory() : impl(new Private){}
RFModuleFactory& RFModuleFactory::GetInstance()
{
    static RFModuleFactory instance;
    return instance;
}

void RFModuleFactory::AddModule(const string &name, RFModule* module)
{
    GetInstance().impl->delegates[name] = module;
}

RFModule* RFModuleFactory::GetModule(const string& name)
{
    if(impl->delegates.find(name) != impl->delegates.end())
    {
        return impl->delegates[name];
    }
    return nullptr;
}


