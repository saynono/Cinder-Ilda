//
//  Etherdream.cpp
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CinderEasyLase.h"

namespace ciilda {
    
    
    EasyLase::EasyLase(){
        bIsRunning = false;
    }
    
    void EasyLase::setup(){
        
        bCardIsConnected = false;
        unsigned int val = easyLaseGetCardNum();
        if(val > 0) bCardIsConnected = true;

        setPPS(18000);
        start();
        
        console() << "EasyLase::init - Connecting..." << std::endl;
//        if (etherdream_connect(device) < 0) return 1;
        
        console() << "EasyLase::init - done" << std::endl;
                
        bIsRunning = true;
    }
    
    
    //--------------------------------------------------------------
	
    void EasyLase::start(){
        bIsRunning = true;
    }

    void EasyLase::stop(){
        bIsRunning = false;
    }
    
    void EasyLase::send(){
        
        if(!bIsRunning || points.empty()) return;
   
        // NOT SO NICE BUT ... (actually no excuse)
        int cnt = 0;
        int l = points.size();
        vector<ciilda::Point>::iterator it;
        EasyLaseData easyLasePoints[l];

        for(it=points.begin();it!=points.end();++it){
            EasyLaseData *p = &easyLasePoints[cnt++];
            p->x = ((*it).x/16 + 2048);
            p->y = ((*it).y/16 + 2048);
            p->r = (*it).r/256;
            p->g = (*it).g/256;
            p->b = (*it).b/256;
            p->i = (*it).a/256;
        }

        int status;
        if(bCardIsConnected){
            status = easyLaseGetStatus(0);
            if(status == 1){
                int sent = easyLaseWriteFrame(0, (unsigned char *)(easyLasePoints), l*8, pps);
            }
        }
    }
    
    
    void EasyLase::clear(){
        points.clear();
    }
    
    
    //--------------------------------------------------------------
    void EasyLase::addPoints(const vector<ciilda::Point>& _points) {
        if(!_points.empty()) {
            points.insert(points.end(), _points.begin(), _points.end());
        }
    }
    
    
    //--------------------------------------------------------------
    void EasyLase::addPoints(const ciilda::Frame& ildaFrame) {
        const vector<Point> points = ildaFrame.getPoints();
        addPoints(points);
    }
    
    
    //--------------------------------------------------------------
    void EasyLase::setPoints(const vector<ciilda::Point>& _points) {
        points.clear();
        points = _points;
    }
    
    
    //--------------------------------------------------------------
    void EasyLase::setPoints(const ciilda::Frame &ildaFrame) {
        setPoints(ildaFrame.getPoints());
    }    
    
    //--------------------------------------------------------------
    void EasyLase::setPPS(int i) {
        pps = i;
    }
    
    //--------------------------------------------------------------
    int EasyLase::getPPS() const {
        return pps;
    }
    
    void EasyLase::setBlankFrame() {
        points.clear();
        
        for(int i=0;i<10;i++){
            ciilda::Point p;
            p.x = 0;
            p.y = i*20;
            p.r = 255;
            p.g = 0;
            p.b = 0;
            points.push_back(p);
        }

    }

} // namespace ciilda
