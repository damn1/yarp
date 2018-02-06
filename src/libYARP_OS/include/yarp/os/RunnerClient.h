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
    typedef std::string string;
public:
    
    RunnerClient() = default;

    /**
    * opens the client.
    * @param local the name of local port to open
    * @param remote the name of the remote port to connect to
    * @return true on success, false on failure
    */
    bool open(const string& local, const string& remote);

    /**
    * send a signal to the application.
    * @param alias the stringID of the application to close
    * @param signal the signal to send to the application
    * @return true on success, false on failure
    */
    bool kill(const string& alias, int signal);
    
    /**
    * send a  sigterm signal to the application.
    * @param alias the stringID of the application that will receive the signal
    * @return true on success, false on failure
    */
    bool sigterm(const string& alias);
    
    /**
    * send a  sigterm signal to all open applications.
    * @return true on success, false on failure
    */
    bool sigtermall();
    
    /**
    * request a list of information for every application open
    * @return a bottle containing the data
    */
    yarp::os::Bottle ps();
    
    /**
    * tells if an application is running
    * @param alias the stringID of the application that will receive the signal
    * @return true if it is running, false if not
    */
    bool isRunning(const string& alias);
    
    bool killstdio(const string& alias);
    
    yarp::os::SystemInfoSerializer sysInfo();
    string which(const string& alias);
    
    /**
    * close the remote runner
    * @return true on success, false on failure
    */
    bool exit();
    
    /**
    * asks to run an application
    * @param data a CmdData struct describing the application
    * @param error a string to receive an error on failure
    * @return 
    */
    int cmd(const CmdData& data, string& error);


    yarp::os::Bottle stdio(const StdioData& data);
};