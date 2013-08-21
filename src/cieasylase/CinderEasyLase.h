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
#include "easyLase.h"
#include "CinderIldaFrame.h"
#include "CinderLaserDac.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace ciilda {
    
    class EasyLase: public LaserController{

public:
	
    EasyLase();
    
    virtual ~EasyLase() {
        kill();
    }
    
    virtual void kill() {
        clear();
        stop();
        if(bCardIsConnected){
            setBlankFrame();
            send();
            easyLaseStop(0);
            bIsRunning = false;
            console() << "EasyLase::kill!" << std::endl;
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
    // check if the device has shutdown (weird bug in etherdream driver) and reconnect if nessecary
    bool checkConnection(bool bForceReconnect = true);
    
private:
    enum {
        EASYLASE_NOTFOUND = 0,
        EASYLASE_FOUND
    } state;
    
    int pps;
    bool bIsRunning;
    bool bCardIsConnected;
    
    vector<ciilda::Point> points;    
    
};

} // namespace ciilda



