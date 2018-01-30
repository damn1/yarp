/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * Authors: Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */
#include <yarp/os/YarpPluginSelector.h>
#include <yarp/os/YarpPluginSettings.h>
#include <yarp/os/YarpPlugin.h>
#include <yarp/os/RFModuleFactory.h>
#include <map>
using namespace std;
using namespace yarp::os;

class RFModuleSelector : public YarpPluginSelector
{
    virtual bool select(Searchable& options) override
    {
        return options.check("type",Value("none")).asString() == "RFModule";
    }
};



class RFPlugin::RFPlugin_Private
{
public:
    string                       name;
    YarpPlugin<RFModule>         yarpPlugin;
    SharedLibraryClass<RFModule> sharedLibClass;
    RFModuleSelector             selector;

};

void RFPlugin::close()
{
    impl->sharedLibClass->stopModule();
}

bool RFPlugin::isRunning()
{
    return !impl->sharedLibClass->isStopping();
}

bool RFPlugin::open(const string& command)
{
    ResourceFinder     rf;
    string name = command.substr(0, command.find(" "));

    char* str = new char[command.size()];
    memcpy(str, command.c_str(), command.size());

    enum { kMaxArgs = 64 };
    int argc = 0;
    char* argv[kMaxArgs];

    char* p2 = strtok(str, " ");
    while (p2 && argc < kMaxArgs - 1)
    {
        argv[argc++] = p2;
        p2 = strtok(0, " ");
    }
    argv[argc] = 0;

    
    rf.configure(argc, argv);
    delete str;

    RFModule* staticmodule{nullptr};
    staticmodule = RFModuleFactory::GetInstance().GetModule(name);
    if (staticmodule)
    {
        try
        {
            if(staticmodule->configure(rf)) return false;
            staticmodule->runModuleThreaded();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    YarpPluginSettings settings;
    impl = new RFPlugin_Private;
    impl->name = name;
    impl->selector.scan();

    settings.setPluginName(impl->name);
    settings.setVerboseMode(true);
    
    if (!settings.setSelector(impl->selector)) return false;
    if (!impl->yarpPlugin.open(settings)) return false;
    
    impl->sharedLibClass.open(*impl->yarpPlugin.getFactory());
    
    if (!impl->sharedLibClass.isValid()) return false;
    
    settings.setLibraryMethodName(impl->yarpPlugin.getFactory()->getName(), settings.getMethodName());
    settings.setClassInfo(impl->yarpPlugin.getFactory()->getClassName(), impl->yarpPlugin.getFactory()->getBaseClassName());
    
    try
    {
        impl->sharedLibClass.getContent().configure(rf);
    }
    catch ( ... )
    {
        return false;
    }

    bool&& ret = impl->sharedLibClass->configure(rf);
    if(ret) impl->sharedLibClass->runModuleThreaded();
    return ret;
}

struct RFModuleFactory::Private
{
    map<string, RFModule*> delegates;
};

RFModuleFactory::RFModuleFactory() : impl(new Private)
{
    //add rfmodule here
}

RFModuleFactory& RFModuleFactory::GetInstance()
{
    static RFModuleFactory instance;
    return instance;
}

void RFModuleFactory::AddModule(const string &name, RFModule* module)
{
    GetInstance().impl->delegates[name] = module;
}

RFModule* RFModuleFactory::GetModule(const string name)
{
    if(impl->delegates.find(name) != impl->delegates.end())
    {
        return impl->delegates[name];
    }

    return nullptr;
}

