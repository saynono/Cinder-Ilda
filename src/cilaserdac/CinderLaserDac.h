//
//  CinderLaserDac.h
//  QLT_GenomeLaser
//
//  Created by say nono on 07.07.13.
//  Copyright (c) 2013 http://say-nono.com. All rights reserved.
//

#pragma once


#include "cinder/app/AppBasic.h"
#include "CinderIldaFrame.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace ciilda {

class LaserController{

public:
	
    LaserController(){};
    
    virtual ~LaserController(){};
    
    virtual bool stateIsFound(){return false;};
    
    virtual void kill(){};
    virtual void setup(bool bStartThread = true){};
    virtual void threadedFunction(){};
    
    
    // check if the device has shutdown (weird bug in etherdream driver) and reconnect if nessecary
    virtual bool checkConnection(bool bForceReconnect = true){return false;};
    
    virtual void clear(){};
    virtual void start(){};
    virtual void stop(){};
    
    virtual void addPoints(const vector<ciilda::Point>& _points){};
    virtual void addPoints(const ciilda::Frame& ildaFrame){};
    
    virtual void setPoints(const vector<ciilda::Point>& _points){};
    virtual void setPoints(const ciilda::Frame& ildaFrame){};
    
    virtual void send(){};
    
    virtual void setPPS(int i){};
    virtual int getPPS() const {return 0;};
    
    virtual void setWaitBeforeSend(bool b) {};
    virtual bool getWaitBeforeSend() const {return false;};
	
};

} // namesapce ciilda

