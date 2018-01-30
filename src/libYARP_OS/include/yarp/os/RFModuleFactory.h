/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * Authors: Andrea Ruzzenenti <andrea.ruzzenenti@iit.it>
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <yarp/os/RFModule.h>
#include <string>

namespace yarp {
namespace os {

class YARP_OS_API RFModuleFactory
{
private:
    struct Private;
    Private* impl;
    RFModuleFactory();
public:

    static RFModuleFactory& GetInstance();
    static void AddModule(const std::string& name, RFModule* module);
    RFModule* GetModule(const std::string& name);
};

}
}
