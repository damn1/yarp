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

RunnerClient::boolOutcome RunnerClient::kill(const string& alias, int signal, const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    Property& killgrp = request.addGroup("kill");

    killgrp.put("alias", alias);
    killgrp.put("signal", signal);
    
    port.write(request, reply);

    return reply.get(0).asString() == "kill OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::boolOutcome RunnerClient::sigterm(const string& alias, const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("sigterm", alias);

    port.write(request, reply);

    return reply.get(0).asString() == "sigterm OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::boolOutcome RunnerClient::sigtermall(const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("sigtermall", "");
    port.write(request, reply);

    return reply.get(0).asString() == "sigtermall OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}
RunnerClient::boolOutcome RunnerClient::ps(const std::string& remote, Bottle& output)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    
    request.put("ps", "");
    port.write(request, output);

    return RUNNER_RESULT_TRUE;
}

RunnerClient::boolOutcome RunnerClient::isRunning(const string& alias, const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("ps", "");
    port.write(request, reply);
    return reply.get(0).asString() == "running" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::boolOutcome RunnerClient::killstdio(const string& alias, const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("killstdio", alias);
    port.write(request, reply);
    return reply.get(0).asString() == "running" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::boolOutcome RunnerClient::sysInfo(const std::string& remote, SystemInfoSerializer& output)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("sysinfo", "");
    port.write(request, output);
    return RUNNER_RESULT_TRUE;
}

RunnerClient::boolOutcome RunnerClient::which(const string& alias, const std::string& remote, string& output)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    yarp::os::Value v;
    request.put("which", alias);
    port.write(request, v);
    output = v.asString();
    return RUNNER_RESULT_TRUE;
}

RunnerClient::boolOutcome RunnerClient::exit(const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    request.put("exit", "");
    port.write(request, reply);
    return reply.get(0).asString() == "exit OK" ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;
}

RunnerClient::boolOutcome RunnerClient::cmd(const CmdData& data, string& error, const std::string& remote)
{
    if (connect(remote))
    {
        return RUNNER_RESULT_CONNECTION_ERROR;
    }

    Property cmdB = request.addGroup("cmd");
    cmdB.fromArguments(data.cmd.c_str());
    request.put("alias", data.alias);
    request.put("env", data.env);
    if(data.log) request.put("log", data.loggerName);
    request.put("workdir", data.workdir);
    port.write(request, reply);
    error = reply.get(1).asString();
    return reply.get(0).asInt() == 0 ? RUNNER_RESULT_TRUE : RUNNER_RESULT_FALSE;

}

RunnerClient::boolOutcome RunnerClient::stdio(const StdioData& data, const std::string& remote, Bottle& output)
{
    if (connect(remote))
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