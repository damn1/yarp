/*
* Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
* Authors: Andrea Ruzzenenti
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*/

#include <yarp/os/RunnerServer.h>
#include <yarp/os/Os.h>
#include <yarp/os/impl/RunCheckpoints.h>
#include <yarp/os/impl/NameClient.h>
#include <yarp/os/LogStream.h>
#include <map>
using namespace std;
using namespace yarp::os;

bool RunnerServer::start(string portName)
{
    if (!rpc.open(portName)) return false;

    yarp::os::Bottle cmd, reply;
    cmd.addString("set");
    cmd.addString(rpc.getName());
    cmd.addString("yarprun");
    cmd.addString("true");
    yarp::os::impl::NameClient::getNameClient().send(cmd, reply);

    yInfo() << "Yarprun succesfully started on port: " << portName.c_str();

    while (rpc.isOpen())
    {
        Bottle msg;

        RUNLOG("<<<port.read(msg, true)")
        if (!rpc.read(msg, true)) break;
        RUNLOG(">>>port.read(msg, true)")

        // command with stdio management
        if (msg.check("stdio") && callbacks.stdio != nullptr)
        {
            yarp::os::ConstString strOnPort    = msg.find("on").asString();
            yarp::os::ConstString strStdioPort = msg.find("stdio").asString();

            auto r = callbacks.stdio(
            {
                strOnPort,
                strStdioPort,
                {
                    msg.check("log"),
                    msg.findGroup("log").get(1).asString(),
                    msg.find("as").asString(),
                    msg.findGroup("cmd").get(1).asString(),
                    msg.find("env").asString(),
                    ""
                }
            });

            rpc.reply(r);

            continue;
        }

        // without stdio
        if (msg.check("cmd") && callbacks.cmd != nullptr)
        {
            CmdData command
            {
                msg.check("log"), 
                msg.findGroup("log").get(1).asString(), 
                msg.find("as").asString(), 
                msg.findGroup("cmd").get(1).asString(),
                msg.find("env").asString(),
                msg.find("workdir").asString()
            };

            Bottle b;
            string error;
            b.addInt(callbacks.cmd(command, error));
            b.addString(error);
            rpc.reply(b);

            continue;
        }

        else if (msg.check("kill") && callbacks.kill != nullptr)
        {
            Bottle result;
            bool   ok;

            ok = callbacks.kill(msg.findGroup("kill").get(1).asString(), msg.findGroup("kill").get(2).asInt());
            result.addString(ok ? "kill OK" : "kill FAILED");
            rpc.reply(result);
            continue;
        }

        else if (msg.check("sigterm") && callbacks.sigterm != nullptr)
        {
            Bottle result;
            bool   ok;

            ok = callbacks.sigterm(msg.find("sigterm").asString());
            result.addString(ok ? "sigterm OK" : "sigterm FAILED");
            rpc.reply(result);
            continue;
        }

        else if (msg.check("sigtermall") && callbacks.sigtermall != nullptr)
        {
            callbacks.sigtermall();
            Bottle result;
            result.addString("sigtermall OK");
            rpc.reply(result);
            continue;
        }

        else if (msg.check("ps") && callbacks.ps != nullptr)
        {
            auto r = callbacks.ps();
            rpc.reply(r);
            continue;
        }

        else if (msg.check("isrunning") && callbacks.isRunning != nullptr)
        {
            Bottle result;
            result.addString(callbacks.isRunning(msg.find("isrunning").asString()) ? "running" : "not running");
            rpc.reply(result);
            continue;
        }

        else if (msg.check("killstdio") && callbacks.killstdio != nullptr)
        {
            callbacks.killstdio(msg.find("killstdio").asString());
            Bottle result;
            result.addString("killstdio OK");
            rpc.reply(result);
            continue;
        }

        else if (msg.check("sysinfo") && callbacks.sysInfo != nullptr)
        {
            auto r = callbacks.sysInfo();
            rpc.reply(r);
            continue;
        }

        else if (msg.check("which") && callbacks.which != nullptr)
        {
            auto r = yarp::os::Value(callbacks.which(msg.find("which").asString()));
            rpc.reply(r);
            continue;
        }

        else if (msg.check("exit") && callbacks.exitClbk != nullptr)
        {
            callbacks.exitClbk();
            Bottle result;
            result.addString("exit OK");
            rpc.reply(result);
        }

        else
        {
            string rep("function not implemented\ncurrent runner implements the following commands:\n\n");
            if(callbacks.kill       != nullptr) rep += "kill \n";
            if(callbacks.sigterm    != nullptr) rep += "sigterm \n";
            if(callbacks.sigtermall != nullptr) rep += "sigtermall \n";
            if(callbacks.ps         != nullptr) rep += "ps \n";
            if(callbacks.isRunning  != nullptr) rep += "isrunning \n";
            if(callbacks.killstdio  != nullptr) rep += "killstdio \n";
            if(callbacks.sysInfo    != nullptr) rep += "sysinfo \n";
            if(callbacks.which      != nullptr) rep += "which \n";
            if(callbacks.exitClbk   != nullptr) rep += "exit \n";
            if(callbacks.cmd        != nullptr) rep += "cmd \n";
            if(callbacks.stdio      != nullptr) rep += "stdio \n";

            Bottle v;
            v.addString(rep);
            rpc.reply(v);
        }

    }

    return 0;
}
