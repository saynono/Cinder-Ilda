//
//  Etherdream.cpp
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CinderEtherdream.h"

namespace ciilda {
    
    
    Etherdream::Etherdream():state(ETHERDREAM_NOTFOUND){
        bThreadRunning = false;
    }
    
    void Etherdream::setup(bool bStartThread){
        etherdream_lib_start();
    
        setPPS(18000);
        setWaitBeforeSend(false);
    
        /* Sleep for a bit over a second, to ensure that we see broadcasts
         * from all available DACs. */
        usleep(100000);
        init();
    
        if(bStartThread) start();
    
    }
    
    //--------------------------------------------------------------
    bool Etherdream::stateIsFound() {
        return state == ETHERDREAM_FOUND;
    }
    
    //--------------------------------------------------------------
    bool Etherdream::checkConnection(bool bForceReconnect) {
        if(device->state == ST_SHUTDOWN) {
            if(bForceReconnect) {
                kill();
                setup();
            }
            return false;
        }
        return true;
    }
    
    //--------------------------------------------------------------
    int Etherdream::init() {
        int device_num = etherdream_dac_count();
        if (!device_num) {
//            console() << "ofxEtherdream::init - No DACs found" << std::endl;
            return 0;
        }
        
        for (int i=0; i<device_num; i++) {
            console() << "ofxEtherdream::init - " << i << " Ether Dream " << etherdream_get_id(etherdream_get(i)) << std::endl;
        }
        
        device = etherdream_get(0);
        
        console() << "ofxEtherdream::init - Connecting..." << std::endl;
        if (etherdream_connect(device) < 0) return 1;
        
        console() << "ofxEtherdream::init - done" << std::endl;
        
        lock_guard<mutex> lock(mMutex);
        state = ETHERDREAM_FOUND;
        return 1;
    }
    
    //--------------------------------------------------------------
    void Etherdream::threadedFunction() {
//        while (isThreadRunning() != 0) {
        while(bThreadRunning){
            switch (state) {
                case ETHERDREAM_NOTFOUND:
                    init();
                    break;
                    
                case ETHERDREAM_FOUND:
                    lock_guard<mutex> lock(mMutex);
                    send();
                    break;
            }
            cinder::sleep(10);
        }
    }
    
    //--------------------------------------------------------------
	
    void Etherdream::start(){
        if(!bThreadRunning){
            console() << "Etherdream::start thread " << std::endl;
            bThreadRunning = true;
            mThread = std::shared_ptr<std::thread>( new std::thread( &Etherdream::threadedFunction, this ) );
        }
    }

    void Etherdream::stop(){
        if(bThreadRunning){
            console() << "Etherdream::stop thread " << std::endl;
            bThreadRunning = false;
            mThread->join();
        }
        
    }
    
    void Etherdream::send(){
        
//        lock_guard<mutex> lock(mMutex);
        
        
        if(!stateIsFound() || points.empty()) return;
        
//        console() << "Etherdream::send()" << std::endl;
        
        if(bWaitBeforeSend) etherdream_wait_for_ready(device);
        else if(!etherdream_is_ready(device)) return;
        
//        console() << "stateIsFound() : " << stateIsFound() << "     points.size: " << points.size() << std::endl;
//        
//        for(int i=0;i<points.size();i++){
//            console() << i << "     -> x: " << points[i].x << "    y: " << points[i].y << "         rgba " << points[i].r << " | " << points[i].g << " | " << points[i].b << " | " << std::endl;
//        }
        
        // DODGY HACK: casting ofxIlda::Point* to etherdream_point*
        int res = etherdream_write(device, (etherdream_point*)points.data(), points.size(), pps, 1);
//        if (res != 0) {
//            console() << "Etherdream::write " << res << std::endl;
//        }
    
    }
    
    
    void Etherdream::clear(){
        lock_guard<mutex> lock(mMutex);
        points.clear();		
    }
    
    
    //--------------------------------------------------------------
    void Etherdream::addPoints(const vector<ciilda::Point>& _points) {
        lock_guard<mutex> lock(mMutex);
        if(!_points.empty()) {
            points.insert(points.end(), _points.begin(), _points.end());
        }
    }
    
    
    //--------------------------------------------------------------
    void Etherdream::addPoints(const ciilda::Frame& ildaFrame) {
        const vector<Point> points = ildaFrame.getPoints();
        addPoints(points);
    }
    
    
    //--------------------------------------------------------------
    void Etherdream::setPoints(const vector<ciilda::Point>& _points) {
        lock_guard<mutex> lock(mMutex);
        points.clear();
        points = _points;
    }
    
    
    //--------------------------------------------------------------
    void Etherdream::setPoints(const ciilda::Frame &ildaFrame) {
//        ildaFrame.
        setPoints(ildaFrame.getPoints());
    }
    
    //--------------------------------------------------------------
    void Etherdream::setWaitBeforeSend(bool b) {
        lock_guard<mutex> lock(mMutex);
        bWaitBeforeSend = b;
    }
    
    //--------------------------------------------------------------
    bool Etherdream::getWaitBeforeSend() const {
        return bWaitBeforeSend;
    }
    
    
    //--------------------------------------------------------------
    void Etherdream::setPPS(int i) {
        lock_guard<mutex> lock(mMutex);
        pps = i;
    }
    
    //--------------------------------------------------------------
    int Etherdream::getPPS() const {
        return pps;
    }
    
    void Etherdream::setBlankFrame() {
        lock_guard<mutex> lock(mMutex);
        points.clear();
        
        for(int i=0;i<10;i++){
            ciilda::Point p;
            p.x = i/200.0;
            p.y = 0;
            p.r = 0;
            p.g = 0;
            p.b = 0;
            points.push_back(p);
        }

    }

} // namespace ciilda
