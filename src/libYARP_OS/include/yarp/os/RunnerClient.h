/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include <yarp/os/SystemInfoSerializer.h>
#include <yarp/os/impl/RunnerCommon.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <string>

namespace yarp {
    namespace os {
        class RunnerClient;
        
    }
}

class YARP_OS_API yarp::os::RunnerClient
{
    typedef yarp::os::impl::runnercommon::CmdData CmdData;
    typedef yarp::os::impl::runnercommon::StdioData StdioData;
    //flags
    yarp::os::Port     port;
    bool               isValid{ false };
    yarp::os::Property request;
    yarp::os::Bottle   reply;
    
    bool connect(const std::string& remote);
    void disconnect();

public:
    
    enum boolOutcome
    {
        RUNNER_RESULT_CONNECTION_ERROR,
        RUNNER_RESULT_TRUE,
        RUNNER_RESULT_FALSE
    };
    
    RunnerClient() = default;

    /**
    * send a signal to the application.
    * @param alias the stringID of the application to close
    * @param signal the signal to send to the application
    * @return true on success, false on failure
    */
    boolOutcome kill(const std::string& alias, int signal, const std::string& remote);
    
    /**
    * send a  sigterm signal to the application.
    * @param alias the stringID of the application that will receive the signal
    * @return true on success, false on failure
    */
    boolOutcome sigterm(const std::string& alias, const std::string& remote);
    
    /**
    * send a  sigterm signal to all open applications.
    * @return true on success, false on failure
    */
    boolOutcome sigtermall(const std::string& remote);
    
    /**
    * request a list of information for every application open
    * @return a bottle containing the data
    */
    boolOutcome ps(const std::string& remote, yarp::os::Bottle& output);
    
    /**
    * tells if an application is running
    * @param alias the stringID of the application that will receive the signal
    * @return true if it is running, false if not
    */
    boolOutcome isRunning(const std::string& alias, const std::string& remote);
    
    boolOutcome killstdio(const std::string& alias, const std::string& remote);
    
    boolOutcome sysInfo(const std::string& remote, yarp::os::SystemInfoSerializer& output);
    boolOutcome which(const std::string& alias, const std::string& remote, std::string& output);
    
    /**
    * close the remote runner
    * @return true on success, false on failure
    */
    boolOutcome exit(const std::string& remote);
    
    /**
    * asks to run an application
    * @param data a CmdData struct describing the application
    * @param error a string to receive an error on failure
    * @return 
    */
    boolOutcome cmd(const CmdData& data, std::string& error, const std::string& remote);


    boolOutcome stdio(const StdioData& data, const std::string& remote, yarp::os::Bottle& output);
};