// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2008 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#include <yarp/DgramTwoWayStream.h>
#include <yarp/String.h>
#include <yarp/UnitTest.h>
#include <yarp/NetType.h>

using namespace yarp;
using namespace yarp::os;

#define MAX_PACKET 100

class DgramTest : public DgramTwoWayStream {
public:
    ManagedBytes store[MAX_PACKET];
    int cursor;
    int readCursor;

    DgramTest() {
        cursor = 0;
        readCursor = 0;
    }

    virtual void onMonitorInput() {
        //printf("Waiting for input %d %d\n", readCursor, cursor);
        removeMonitor();
        if (readCursor<cursor) {
            setMonitor(store[readCursor].bytes());
            //printf("Gave input of size %d\n", getMonitor().length());
            readCursor++;
        }
    }

    virtual void onMonitorOutput() {
        if (cursor<MAX_PACKET) {
            store[cursor] = ManagedBytes(getMonitor(),false);
            store[cursor].copy();
        } else {
            printf("message too big\n");
            exit(1);
        }
        cursor++;
        removeMonitor();
    }

    int size() {
        return cursor;
    }

    void clear() {
        readCursor = 0;
        cursor = 0;
    }

    Bytes get(int i) {
        return store[i].bytes();
    }

    void copyMonitor(DgramTest& alt) {
        readCursor = 0;
        for (int i=0; i<alt.cursor; i++) {
            store[cursor] = ManagedBytes(alt.get(i),false);
            store[cursor].copy();
            cursor++;
        }
        removeMonitor();
    }

    void corrupt(int index, int offset = 0) {
        if (index<MAX_PACKET) {
            if (offset<store[index].length()) {
                store[index].get()[offset] ^= 255;
            } else {
                printf("cannot corrupt nonexistent byte\n");
            }
        } else {
            printf("cannot corrupt nonexistent dgram\n");
        }
    }
};

class DgramTwoWayStreamTest : public yarp::UnitTest {
public:
    virtual yarp::String getName() { return "DgramTwoWayStreamTest"; }

    void checkNormal() {
        report(0, "checking that normal messages get through");

        DgramTest out;

        int sz = 100;
        out.openMonitor(sz,sz);

        ManagedBytes msg(200);
        for (int i=0; i<msg.length(); i++) {
            msg.get()[i] = i%128;
        }
        out.beginPacket();
        out.write(msg.bytes());
        out.flush();
        out.endPacket();
        printf("created %d packets\n", out.size());
        checkEqual(3,out.size(),"right number of packets");


        ////////////////////////////////////////////////////////////////////
        // Send a multi-dgram message, see if it gets through

        DgramTest in;
        in.openMonitor(sz,sz);
        ManagedBytes recv(200);
        for (int i=0; i<recv.length(); i++) {
            recv.get()[i] = 0;
        }
        in.copyMonitor(out);
        in.beginPacket();
        NetType::readFull(in,recv.bytes());
        in.endPacket();
        bool mismatch = false;
        for (int i=0; i<recv.length(); i++) {
            if (recv.get()[i]!=msg.get()[i]) {
                printf("Mismatch, at least as early as byte %d\n", i);
                mismatch = true;
                break;
            }
        }
        checkFalse(mismatch,"received what is sent");


        ////////////////////////////////////////////////////////////////////
        // Send three messages, see if all get through
        // (just testing the receiver side)

        in.clear();
        in.copyMonitor(out);
        in.copyMonitor(out);
        in.copyMonitor(out);
        mismatch = false;
        for (int k=0; k<3; k++) {
            for (int i=0; i<recv.length(); i++) {
                recv.get()[i] = 0;
            }
            in.beginPacket();
            NetType::readFull(in,recv.bytes());
            in.endPacket();
            for (int i=0; i<recv.length(); i++) {
                if (recv.get()[i]!=msg.get()[i]) {
                    printf("Mismatch, at least as early as byte %d\n", i);
                    mismatch = true;
                    break;
                }
            }
        }
        checkFalse(mismatch,"multiple messages ok");

        
        ////////////////////////////////////////////////////////////////////
        // Send three messages, corrupt middle one

        in.clear();
        in.copyMonitor(out);
        in.copyMonitor(out);
        in.copyMonitor(out);

        // corrupt 10th byte of 4th dgram
        in.corrupt(4,10);

        mismatch = false;
        bool goodRead[4];
        for (int k=0; k<4; k++) {
            printf("Iteration %d\n", k);
            for (int i=0; i<recv.length(); i++) {
                recv.get()[i] = 0;
            }
            in.beginPacket();
            NetType::readFull(in,recv.bytes());
            in.endPacket();
            mismatch = false;
            for (int i=0; i<recv.length(); i++) {
                if (recv.get()[i]!=msg.get()[i]) {
                    printf("Mismatch, at least as early as byte %d\n", i);
                    mismatch = true;
                    break;
                }
            }
            goodRead[k] = !mismatch;
        }
        checkTrue(goodRead[0],"first read should be good");
        checkTrue(!goodRead[1],"second read should be broken");
        checkTrue(!goodRead[2],"third read should be broken");
        checkTrue(goodRead[3],"fource read should be good again");
    }

    virtual void runTests() {
        checkNormal();
    }
};

static DgramTwoWayStreamTest theDgramTwoWayStreamTest;

yarp::UnitTest& getDgramTwoWayStreamTest() {
    return theDgramTwoWayStreamTest;
}

