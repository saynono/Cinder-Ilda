//
//  CinderEtherdream.h
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "cinder/app/App.h"
#include "cinder/Thread.h"
#include "etherdream.h"
#include "CinderIldaFrame.h"
#include "CinderLaserDac.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace ciilda {
    
    class Etherdream: public LaserController{

public:
	
    Etherdream();
    
    virtual ~Etherdream() {
        kill();
    }
    
    virtual void kill() {
        clear();
        stop();
        if(stateIsFound()) {
            etherdream_stop(device);
            etherdream_disconnect(device);
        }
    }
    
    virtual void setup();
    
    virtual void clear();
    virtual void start();
    virtual void stop();
    
    virtual void addPoints(const vector<ciilda::Point>& _points);
    virtual void addPoints(const ciilda::Frame& ildaFrame);
    
    virtual void setPoints(const vector<ciilda::Point>& _points);
    virtual void setPoints(const ciilda::Frame& ildaFrame);
    
    virtual void setBlankFrame();
    virtual void send();
    
    virtual void setPPS(int i);
    virtual int getPPS() const;
    
    
    
private:
    int init();
    bool stateIsFound();
    void threadedFunction();
        
    // check if the device has shutdown (weird bug in etherdream driver) and reconnect if nessecary
    bool checkConnection(bool bForceReconnect = true);
        
    void setWaitBeforeSend(bool b);
    bool getWaitBeforeSend() const;
    
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



