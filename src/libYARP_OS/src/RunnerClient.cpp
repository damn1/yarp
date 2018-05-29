/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include <yarp/os/RunnerClient.h>
#include <yarp/os/Network.h>
using namespace std;
using namespace yarp::os;
bool RunnerClient::connect(const string& remote)
{
    isValid = false;
    if (port.isOpen())port.close();

    if (!port.open("...")) return false;
    if (!yarp::os::Network::connect(port.getName(), remote)) return false;
    
    isValid = true;
    return true;
}

void RunnerClient::disconnect()
{
    port.close();
}

RunnerClient::RunnerResult RunnerClient::kill(const string& alias, int signal, const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    Property& killgrp = request.addGroup("kill");

    killgrp.put("alias", alias);
    killgrp.put("signal", signal);
    
    port.write(request, reply);

    return reply.get(0).asString() == "kill OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::RunnerResult RunnerClient::sigterm(const string& alias, const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("sigterm", alias);

    port.write(request, reply);

    return reply.get(0).asString() == "sigterm OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::RunnerResult RunnerClient::sigtermall(const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("sigtermall", "");
    port.write(request, reply);

    return reply.get(0).asString() == "sigtermall OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}
RunnerClient::RunnerResult RunnerClient::ps(const std::string& remote, Bottle& output)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.clear();
    request.put("ps", "");
    port.write(request, output);

    return RUNNER_RESULT_TRUE;
}

RunnerClient::RunnerResult RunnerClient::isRunning(const string& alias, const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("isrunning", alias);
    port.write(request, reply);
    return reply.get(0).asString() == "running" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::RunnerResult RunnerClient::killstdio(const string& alias, const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("killstdio", alias);
    port.write(request, reply);
    return reply.get(0).asString() == "running" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::RunnerResult RunnerClient::sysInfo(const std::string& remote, SystemInfoSerializer& output)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("sysinfo", "");
    port.write(request, output);
    return RUNNER_RESULT_TRUE;
}

RunnerClient::RunnerResult RunnerClient::which(const string& alias, const std::string& remote, string& output)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    yarp::os::Value v;
    request.put("which", alias);
    port.write(request, v);
    output = v.asString();
    return RUNNER_RESULT_TRUE;
}

RunnerClient::RunnerResult RunnerClient::exit(const std::string& remote)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("exit", "");
    port.write(request, reply);
    return reply.get(0).asString() == "exit OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::RunnerResult RunnerClient::cmd(const CmdData& data, string& error, const std::string& remote, string& logPort)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }
    request.clear();
    request.put("cmd", data.cmd.c_str());
    request.put("as", data.alias);
    request.put("env", data.env);
    if(data.log) request.put("log", data.loggerName);
    if(!data.workdir.empty() && data.workdir[0] != '\0') request.put("workdir", data.workdir);
    port.write(request, reply);
    error   = reply.get(1).asString();
    logPort = reply.get(2).asString();
    return reply.get(0).asInt() == 0 ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;

}

RunnerClient::RunnerResult RunnerClient::stdio(const StdioData& data, const std::string& remote, Bottle& output)
{
    if (!connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("on", data.strOnPort);
    request.put("stdio", data.strOnPort);
    Property cmdB = request.addGroup("cmd");
    cmdB.fromArguments(data.cmdData.cmd.c_str());
    request.put("alias", data.cmdData.alias);
    request.put("env", data.cmdData.env);
    if (data.cmdData.log) request.put("log", data.cmdData.loggerName);
    request.put("workdir", data.cmdData.workdir);
    port.write(request, output);
    return RUNNER_RESULT_TRUE;
}