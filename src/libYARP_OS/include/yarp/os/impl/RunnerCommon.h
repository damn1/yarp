/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/
#ifndef RUNNERCOMMON_H
#define RUNNERCOMMON_H
#include <vector>
#include <string>
namespace yarp {
namespace os {
namespace impl {
namespace runnercommon {

struct CmdData
{
    bool        log;
    std::string loggerName;
    std::string alias;
    std::string cmd;
    std::string env;
    std::string workdir;
};

struct StdioData
{
    std::string strOnPort;
    std::string strStdioPort;
    CmdData     cmdData;
};

struct PsData
{
    int         pid;
    std::string alias;
    bool        status;
    std::string cmd;
    std::string env;
};

}
}
}
}
#endif