//
//  CinderEtherdream.h
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "cinder/app/AppBasic.h"
#include "cinder/Thread.h"
#include "etherdream.h"
#include "CinderIldaFrame.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace ciilda {
    
class Etherdream{

public:
	
    Etherdream();
    
    ~Etherdream() {
        kill();
    }
    
    bool stateIsFound();
    
    void kill() {
        clear();
        stop();
        if(stateIsFound()) {
            etherdream_stop(device);
            etherdream_disconnect(device);
        }
    }
    
    void setup(bool bStartThread = true);
    virtual void threadedFunction();
    
    
    // check if the device has shutdown (weird bug in etherdream driver) and reconnect if nessecary
    bool checkConnection(bool bForceReconnect = true);
    
    void clear();
    void start();
    void stop();
    
    void addPoints(const vector<ciilda::Point>& _points);
    void addPoints(const ciilda::Frame& ildaFrame);
    
    void setPoints(const vector<ciilda::Point>& _points);
    void setPoints(const ciilda::Frame& ildaFrame);
    
    void send();
    
    void setPPS(int i);
    int getPPS() const;
    
    void setWaitBeforeSend(bool b);
    bool getWaitBeforeSend() const;
    
    
private:
    int init();
    
private:
    enum {
        ETHERDREAM_NOTFOUND = 0,
        ETHERDREAM_FOUND
    } state;
    
    int pps;
    bool bWaitBeforeSend;
    bool bThreadRunning;
    
    struct etherdream *device;
    vector<ciilda::Point> points;
    
    
	mutable std::mutex mMutex;
	std::shared_ptr<std::thread> mThread;

    
    
};

} // namespace ciilda



