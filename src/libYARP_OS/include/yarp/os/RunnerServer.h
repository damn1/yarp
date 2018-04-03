/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/
#ifndef RUNNERSERVER_H
#define RUNNERSERVER_H

#include <functional>
#include <string>
#include <yarp/os/Bottle.h>
#include <yarp/os/Value.h>
#include <yarp/os/Port.h>
#include <yarp/os/SystemInfoSerializer.h>
#include <yarp/os/impl/RunnerCommon.h>

namespace yarp {
    namespace os {
        class RunnerServer;
    }
}



class YARP_OS_API yarp::os::RunnerServer
{
public:

private:
    typedef std::string string;
    typedef yarp::os::impl::runnercommon::CmdData   CmdData;
    typedef yarp::os::impl::runnercommon::StdioData StdioData;
    typedef yarp::os::impl::runnercommon::PsData    PsData;

    typedef std::function<bool(const string& alias, int signal)> killClbk;
    typedef std::function<bool(const string& alias)>             sigtermClbk;
    typedef std::function<bool()>                                sigtermallClbk;
    typedef std::function<std::vector<PsData>()>                 psClbk;
    typedef std::function<bool(const string& alias)>             isRunningClbk;
    typedef std::function<bool(const string& alias)>             killstdioClbk;
    typedef std::function<yarp::os::SystemInfoSerializer()>      sysInfoClbk;
    typedef std::function<string(const string&)>                 whichClbk;
    typedef std::function<bool()>                                exitReqClbk;
    typedef std::function<int (const CmdData&, string& error)>   cmdClbk;
    typedef std::function<yarp::os::Bottle(const StdioData&)>    stdioClbk;

    struct Callbacks
    {
        killClbk       kill{nullptr};
        sigtermClbk    sigterm{nullptr};
        sigtermallClbk sigtermall{nullptr};
        psClbk         ps{nullptr};
        isRunningClbk  isRunning{nullptr};
        killstdioClbk  killstdio{nullptr};
        sysInfoClbk    sysInfo{nullptr};
        whichClbk      which{nullptr};
        exitReqClbk    exitClbk{nullptr};
        cmdClbk        cmd{nullptr};
        stdioClbk      stdio{nullptr};
    } callbacks;

    yarp::os::Port rpc;

public:
    /**
    * the class allow to set a callback (static or class member using the templates) for every method called remotely by the yarp::os::RunnerClient.
    * please refer to the documentation of yarp::os::RunnerClient for the single methods pourposes
    */

    RunnerServer() = default;
    RunnerServer(Callbacks clbks) : callbacks(clbks) {}

    //blocking call
    bool start(std::string portName);
    bool isOpen();              // tells if runner port is opened
    std::string getPortname();  // tells on which port the runner is listening

    void setKill(const killClbk& clbk) {callbacks.kill = clbk;}
    void setSigterm(const sigtermClbk& clbk) {callbacks.sigterm = clbk;}
    void setSigtermall(const sigtermallClbk& clbk) {callbacks.sigtermall = clbk;}
    void setPs(const psClbk& clbk) {callbacks.ps = clbk;}
    void setIsRunning(const isRunningClbk& clbk) {callbacks.isRunning = clbk;}
    void setKillstdio(const killstdioClbk& clbk) {callbacks.killstdio = clbk;}
    void setSysInfo(const sysInfoClbk& clbk) {callbacks.sysInfo = clbk;}
    void setWhich(const whichClbk& clbk) {callbacks.which = clbk;}
    void setExit(const exitReqClbk& clbk) {callbacks.exitClbk = clbk;}
    void setCmd(const cmdClbk& clbk) {callbacks.cmd = clbk;}
    void setStdio(const stdioClbk& clbk) {callbacks.stdio = clbk;}

//template section
    template<class T> void setKill(killClbk clbk, T* instance)            {setKill      (std::bind(clbk, instance, std::placeholders::_1, std::placeholders::_2));}
    template<class T> void setSigterm(sigtermClbk clbk, T* instance)      {setSigterm   (std::bind(clbk, instance, std::placeholders::_1));}
    template<class T> void setSigtermall(sigtermallClbk clbk, T* instance){setSigtermall(std::bind(clbk, instance));}
    template<class T> void setPs(psClbk clbk, T* instance)                {setPs        (std::bind(clbk, instance));}
    template<class T> void setIsRunning(isRunningClbk clbk, T* instance)  {setIsRunning (std::bind(clbk, instance, std::placeholders::_1));}
    template<class T> void setKillstdio(killstdioClbk clbk, T* instance)  {setKillstdio (std::bind(clbk, instance, std::placeholders::_1));}
    template<class T> void setSysInfo(sysInfoClbk clbk, T* instance)      {setSysInfo   (std::bind(clbk, instance));}
    template<class T> void setWhich(whichClbk clbk, T* instance)          {setWhich     (std::bind(clbk, instance, std::placeholders::_1));}
    template<class T> void setExit(exitReqClbk clbk, T* instance)         {setExit      (std::bind(clbk, instance));}
    template<class T> void setCmd(cmdClbk clbk, T* instance)              {setCmd       (std::bind(clbk, instance, std::placeholders::_1));}
    template<class T> void setStdio(stdioClbk clbk, T* instance)          {setStdio     (std::bind(clbk, instance, std::placeholders::_1));}

};

#endif // !RUNNERSERVER_H
