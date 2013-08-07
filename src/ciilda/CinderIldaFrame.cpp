//
//  CinderIldaFrame.cpp
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 www.say-nono.com All rights reserved.
//

#include "CinderIldaFrame.h"


namespace ciilda {

    Frame::Frame() {
        setDefaultParams();
    }

    void Frame::setDefaultParams() {
        memset(&params, 0, sizeof(params));  // safety catch all default to zero
        memset(&stats, 0, sizeof(stats));  // safety catch all default to zero
        
        params.draw.lines = true;
        params.draw.points = true;
        
        
        params.output.color = params.output.color.white();
        params.output.blankCount = 3;
        params.output.endCount = 1;
        params.output.doCapX = false;
        params.output.doCapY = false;
        
        params.output.targetPointCount = 600;
        params.output.spacing = 0;
        
        params.output.transform.doFlipX = false;
        params.output.transform.doFlipY = false;
        params.output.transform.offset.set(0, 0);
        params.output.transform.scale.set(1, 1);
        
    }

    //--------------------------------------------------------------
    string Frame::getString() {
        stringstream s;
//        s << polyProcessor.getString();
        
        s << "params:" << endl;
        s << "draw.lines : " << params.draw.lines << endl;
        s << "draw.point : " << params.draw.points << endl;
        
        s << "output.color : " << params.output.color << endl;
        s << "output.blankCount : " << params.output.blankCount << endl;
        s << "output.endCount : " << params.output.endCount << endl;
        s << "output.doCapX : " << params.output.doCapX << endl;
        s << "output.doCapY : " << params.output.doCapY << endl;
        s << "output.transform.doFlipX : " << params.output.transform.doFlipX << endl;
        s << "output.transform.doFlipY : " << params.output.transform.doFlipY << endl;
        s << "output.transform.offset : " << params.output.transform.offset << endl;
        s << "output.transform.scale : " << params.output.transform.scale << endl;
        
        s << endl;
        
        s << "stats:" << endl;
        s << "stats.pointCountOrig : " << stats.pointCountOrig << endl;
        s << "stats.pointCountProcessed : " << stats.pointCountProcessed << endl;
        
        return s.str();
    }
    
    //--------------------------------------------------------------
    void Frame::update() {
        updateFinalPoints();
    }
    
    //--------------------------------------------------------------
    void Frame::draw(float x, float y, float w, float h) {
        
        if(w==0) w = getWindowWidth();
        if(h==0) h = getWindowHeight();
        
        float sw = w / (float) kIldaDimension;
        float sh = h / (float) kIldaDimension;
        
        float cx = lerp(x, x+w, .5);
        float cy = lerp(y, y+h, .5);
                
        if(params.draw.lines) {
            gl::pushMatrices();
            gl::translate(x, y);
            float wScaled = w*params.output.transform.scale.x;
            float hScaled = h*params.output.transform.scale.y;
            gl::translate((w-wScaled)*.5, (h-hScaled)*.5);
            gl::scale(wScaled, hScaled);
            origShape.draw();
            gl::popMatrices();
        }
        
        if(params.draw.points) {
            gl::pushMatrices();
            gl::translate(cx, cy);
            gl::scale(sw, sh);
            gl::color(Color::white());
            glPointSize(3);
            gl::begin(GL_POINTS);
            for(int i=0;i<points.size();i++){
                gl::vertex(points[i].x, points[i].y);
            }
            gl::end();
            
            gl::popMatrices();
        }
        
        
    }

    //--------------------------------------------------------------
    void Frame::clear() {
        points.clear();
        origShape.clear();
    }
    
    //--------------------------------------------------------------
    int Frame::size() {
        // TODO clean up?
        return 10;//origPolys.size();
    }
    
    //--------------------------------------------------------------
    
    void Frame::begin(){
        origShape.clear();
    }

    void Frame::end(){
        updateFinalPoints();
    }

    //--------------------------------------------------------------

    void Frame::addColouredShape2d(const ColouredShape2d& shape){
        origShape.appendColouredShape2d( shape );
    }

    void Frame::addShape2d(const Shape2d& shape){
        origShape.appendShape2d( shape );
    }
    
    void Frame::addShape2d(const Shape2d& shape, ColorA clr){
        origShape.color( clr );
        origShape.appendShape2d( shape );
    }

    //--------------------------------------------------------------

    void Frame::addPath2d(const Path2d& path){
        origShape.appendPath2d( path );
    }
    
    void Frame::addPath2d(const Path2d& path, ColorA clr){
        origShape.color( clr );
        origShape.appendPath2d( path );
    }
    
    //--------------------------------------------------------------
    
    void Frame::moveTo(Vec2f p){
        origShape.moveTo(p);
    }
    
    void Frame::lineTo(Vec2f p){
        origShape.lineTo(p);
    }
    
    void Frame::arcTo(const Vec2f &p, const Vec2f &t, float radius){
        origShape.arcTo(p,t,radius);
    }
    
    void Frame::curveTo(const Vec2f &p, const Vec2f &p1, const Vec2f &p2 ){
        origShape.curveTo(p,p1,p2);
    }
    
    void Frame::quadTo(const Vec2f &p1, const Vec2f &p2 ){
        origShape.quadTo(p1,p2);
    }
    
    //--------------------------------------------------------------
    
    void Frame::setColor(ColorA clr){
        origShape.color( clr );
    }
    
    const ColorA Frame::getCurrentColor(){
        return origShape.getCurrentColor();
    }


    //--------------------------------------------------------------

    const ColouredShape2d& Frame::getShape2d(){
        return origShape;
    }
    
    //--------------------------------------------------------------
    const vector<Point>& Frame::getPoints() const {
        return points;
    }
    
    //--------------------------------------------------------------
    void Frame::drawCalibration() {
        
        Shape2d calib;
        
        Path2d square;
        square.moveTo(0.25, 0.25);
        square.lineTo(0.75, 0.25);
        square.lineTo(0.75, 0.75);
        square.lineTo(0.25, 0.75);
        square.close();
        
        
        Path2d lineVert;
        lineVert.moveTo(0.5,0.25);
        lineVert.moveTo(0.5,0.75);
        
        Path2d lineHori;
        lineHori.moveTo(0.25,0.5);
        lineHori.moveTo(0.75,0.5);
        
        Path2d circle;
        circle.moveTo(0,-0.2);
        circle.arcTo(0, 0.2, 0, 0, M_PI);
        
        calib.appendContour(square);
        calib.appendContour(lineHori);
        calib.appendContour(lineVert);
        calib.appendContour(circle);
        
        begin();
        addShape2d(calib);
        end();
    }
    
    //--------------------------------------------------------------
    Point Frame::transformPoint(Vec2f p, ColorA clr) const {
        
        Point pIlda;
        
        // flip
        if(params.output.transform.doFlipX) p.x = 1 - p.x;
        if(params.output.transform.doFlipY) p.y = 1 - p.y;
        
        // scale
        if(params.output.transform.scale.lengthSquared() > 0) {
            p -= Vec2f(0.5, 0.5);
            p *= params.output.transform.scale;
            p += Vec2f(0.5, 0.5);
        }
        
        // offset
        p += params.output.transform.offset;
        
        
        
        
        // cap or wrap
        if(p.x < 0) {
            p.x = params.output.doCapX ? 0 : 1 + p.x - ceil(p.x);
        } else if(p.x > 1) {
            p.x = params.output.doCapX ? 1 : p.x - floor(p.x);
        }
        
        if(p.y < 0) {
            p.y = params.output.doCapY ? 0 : 1 + p.y - ceil(p.y);
        } else if(p.y > 1) {
            p.y = params.output.doCapY ? 1 : p.y - floor(p.y);
        }
        
        pIlda.set(p, clr);        
        return pIlda;
    }
    
    //--------------------------------------------------------------
    void Frame::updateFinalPoints() {
        
        calculateIldaPoints();
        
    }
    
    
    
    //--------------------------------------------------------------
    
    void Frame::calculateIldaPoints(){
        
        if(origShape.getNumContours() == 0){
            console() << "TODO : BLANK FRAME!" << std::endl;
            return;
        }
        
        vector<float> segmentLengths;
        
        float totalLength = 0;
        float totalLengthBlank = 0;
        int blankCount = params.output.blankCount;
        int endCount = params.output.endCount;
        
        int totalAmountStripped = params.output.targetPointCount;
        
        points.clear();
        
        for(int i=0; i<origShape.getNumContours(); i++) {
            float len = 0;
            size_t pathType;
            Path2d path = origShape.getContour(i);
            totalAmountStripped -= blankCount;
            totalAmountStripped -= endCount;
            for(int j=0;j<path.getNumSegments();j++){
                pathType = path.getSegmentType(j);
                len = 0;
                if(pathType == Path2d::LINETO || pathType == Path2d::CLOSE ){
                    len = (path.getSegmentPosition(j, 1) - path.getSegmentPosition(j, 0)).length();
                }else if(pathType == Path2d::MOVETO){
                    // Don't do anything
                }else{
                    len = getSegmentLength(path,j);
                }
                segmentLengths.push_back(len);
                totalLength += len;
            }
            totalAmountStripped -= blankCount;
            //        if(pathType!=Path2d::CLOSE)
            totalAmountStripped -= endCount;
            
            totalAmountStripped -= 1; // TODO only if not closed?
        }
        
        
        float step = totalLength/totalAmountStripped;
        int segCounter = 0;
        float steps;
        float percentSeg;
        Vec2f pos;
        if(origShape.getNumContours()!=0) pos = origShape.getContour(origShape.getNumContours()-1).getPosition(1);
        Point pIlda;
        ColorA clr;
        for(int i=0; i<origShape.getNumContours(); i++) {
            float len = 0;
            Path2d path = origShape.getContour(i);
            totalLengthBlank += (path.getPosition(0)-pos).length();
            pos = path.getPosition(0);
            pIlda = transformPoint(pos);
            clr = origShape.getSegmentColor( segCounter );
            
            for(int k=0;k<blankCount;k++){ points.push_back(pIlda); }
            pIlda = transformPoint(pos,clr);
            for(int k=0;k<endCount;k++){ points.push_back(pIlda); }
                        
            for(int j=0;j<path.getNumSegments();j++){
                clr = origShape.getSegmentColor( segCounter );
                len = segmentLengths[segCounter++];
                steps = round(len / step);
                for(int k=0;k<steps;k++){
                    percentSeg = k/steps;
                    pos = path.getSegmentPosition(j, k/steps);
//                    
//                    if(Path2d::QUADTO == path.getSegmentType(j)){
//                        console() << "  -> pathType: " << path.getSegmentType(j) << "      " << pos << std::endl;
//                    }

                    pIlda = transformPoint(pos,clr);
                    points.push_back(pIlda);
                }
                pos = path.getSegmentPosition(j, 1);
            }
            pIlda = transformPoint(pos,clr);
            points.push_back(pIlda);
            
            for(int k=0;k<endCount;k++){ points.push_back(pIlda); }
            pIlda = transformPoint(pos);
            for(int k=0;k<blankCount;k++){ points.push_back(pIlda); }
                        
        }
        
        stats.pointCountProcessed = points.size();
        stats.lengthTotal = totalLengthBlank + totalLength;
        stats.lengthBlank = totalLengthBlank;
        stats.lengthLines = totalLength;
        stats.countContours = origShape.getNumContours();
        stats.countSegments = segmentLengths.size();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    ////////////////// little helper
    
    float Frame::getSegmentLength(const Path2d& path, int segment, int detail ){
        float len = 0;
        Vec2f pos1, pos2;
        float step = 1.0/detail;
        pos1 = path.getSegmentPosition(segment, 0);
        for(float percent=step;percent<=1;percent+=step){
            pos2 = path.getSegmentPosition(segment, percent);
            len += (pos2 - pos1).length();
            pos1 = pos2;
        }
        return len;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    
    float Frame::getPathLength(const Path2d& path, int detail ){
        float len = 0;
        float totalLength = 0;
        for(int j=0;j<path.getNumSegments();j++){
            size_t pathType = path.getSegmentType(j);
            len = 0;
            if(pathType == Path2d::LINETO || pathType == Path2d::CLOSE ){
                len = (path.getSegmentPosition(j, 1) - path.getSegmentPosition(j, 0)).length();
            }else if(pathType == Path2d::MOVETO){
                // Don't do anything
            }else{
                len = getSegmentLength(path,j,detail);
            }
            totalLength += len;
        }
        return totalLength;
    }


    //////////////////////////////////////////////////////////////////////////////////////////
    

} // namespace ciilda
