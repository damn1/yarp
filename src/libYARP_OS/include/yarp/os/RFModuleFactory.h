/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * Authors: Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#include <yarp/os/RFModule.h>
#include <string>

namespace yarp {
namespace os {

class YARP_OS_API RFPlugin
{
    class RFPlugin_Private;
    RFPlugin_Private* impl{nullptr};

public:
    RFPlugin();

    virtual ~RFPlugin();

    virtual bool open(const std::string& command);

    virtual void close();

    virtual bool isRunning();

    virtual std::string getCmd();

    virtual std::string getAlias();

    virtual int getThreadKey();
};

class YARP_OS_API RFModuleFactory
{
private:
    struct Private;
    Private* impl;
    RFModuleFactory();
public:

    static RFModuleFactory& GetInstance();
    static void AddModule(const std::string& name, RFModule*(*moduleCreate)(void));
    RFModule* GetModule(const std::string name);
};

}
}
