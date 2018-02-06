/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include <yarp/os/RunnerClient.h>
#include <yarp/os/Network.h>
using namespace std;
using namespace yarp::os;
bool RunnerClient::open(const string& local, const string& remote)
{
    isValid = false;
    if (port.isOpen())port.close();

    if (!port.open(local)) return false;
    if (!yarp::os::Network::connect(local, remote)) return false;
    
    isValid = true;
    return true;
}

bool RunnerClient::kill(const string& alias, int signal)
{
    Property& killgrp = request.addGroup("kill");

    killgrp.put("alias", alias);
    killgrp.put("signal", signal);
    
    port.write(request, reply);

    return reply.get(0).asString() == "kill OK";
}

bool RunnerClient::sigterm(const string& alias)
{
    request.put("sigterm", alias);

    port.write(request, reply);

    return reply.get(0).asString() == "sigterm OK";
}

bool RunnerClient::sigtermall()
{
    request.put("sigtermall", "");
    port.write(request, reply);

    return reply.get(0).asString() == "sigtermall OK";
}
Bottle RunnerClient::ps()
{
    Bottle psreply;
    request.put("ps", "");
    port.write(request, psreply);

    return psreply;
}

bool RunnerClient::isRunning(const string& alias)
{
    request.put("ps", "");
    port.write(request, reply);
    return reply.get(0).asString() == "running";
}

bool RunnerClient::killstdio(const string& alias)
{
    request.put("killstdio", alias);
    port.write(request, reply);
    return reply.get(0).asString() == "running";
}

SystemInfoSerializer RunnerClient::sysInfo()
{
    yarp::os::SystemInfoSerializer sysreply;
    request.put("sysinfo", "");
    port.write(request, sysreply);
    return sysreply;
}

string RunnerClient::which(const string& alias)
{
    yarp::os::Value v;
    request.put("which", alias);
    port.write(request, v);
    return v.asString();
}

bool RunnerClient::exit()
{
    request.put("exit", "");
    port.write(request, reply);
    return reply.get(0).asString() == "exit OK";
}

int RunnerClient::cmd(const CmdData& data, string& error)
{
    Property cmdB = request.addGroup("cmd");
    cmdB.fromArguments(data.cmd.c_str());
    request.put("alias", data.alias);
    request.put("env", data.env);
    if(data.log) request.put("log", data.loggerName);
    request.put("workdir", data.workdir);
    port.write(request, reply);
    error = reply.get(1).asString();
    return reply.get(0).asInt();

}

Bottle RunnerClient::stdio(const StdioData& data)
{
    request.put("on", data.strOnPort);
    request.put("stdio", data.strOnPort);
    Property cmdB = request.addGroup("cmd");
    cmdB.fromArguments(data.cmdData.cmd.c_str());
    request.put("alias", data.cmdData.alias);
    request.put("env", data.cmdData.env);
    if (data.cmdData.log) request.put("log", data.cmdData.loggerName);
    request.put("workdir", data.cmdData.workdir);
    port.write(request, reply);
    return reply;
}