//
//  CinderIldaFrame.h
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "cinder/app/AppBasic.h"
#include "cinder/Shape2d.h"
#include "cinder/utilities.h"
#include "cinder/gl/gl.h"

#include "CinderIldaPoint.h"
//#include "CinderIldaFrameProcessor.h"
//#include "CinderIldaPoly.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace ciilda {
    
class Frame{

public:
	
    struct {
        struct {
            bool lines; // draw lines
            bool points;    // draw points
            bool pointNumbers;  // draw point numbers (not implemented yet)
        } draw;
        
        struct {
            ColorA color; // color
            int blankCount;     // how many blank points to send at path ends
            int endCount;       // how many end repeats to send
            bool doCapX;        // cap out of range on x (otherwise wraps around)
            bool doCapY;        // cap out of range on y (otherwise wraps around)
            int targetPointCount;
            int spacing;
            
            struct {
                bool doFlipX;
                bool doFlipY;
                Vec2f offset;
                Vec2f scale;
            } transform;
        } output;
    } params;

    
    struct {
        int pointCountOrig;    // READONLY current total number of points across all paths (excluding blanks and end repititons)
        int pointCountProcessed; // same as above, except AFTER being processed
    } stats;
    
    Frame();
    
    //--------------------------------------------------------------
    void setDefaultParams();
    string getString();
	void update();

    
    //--------------------------------------------------------------
    void draw(float x=0, float y=0, float w=0, float h=0);
    
    void drawSegment(const Path2d& path, int part );
    
    //--------------------------------------------------------------
    void clear();
    
    //--------------------------------------------------------------
    int size();
    
    //--------------------------------------------------------------
    
//    void addPath2d(const Path2d& path);
    
    //--------------------------------------------------------------
    
    void begin();
    
//    void setShape2d(const Shape2d& shape);
//    void setShape2d(const Shape2d& shape, ColorA clr);

    void addShape2d(const Shape2d& shape);
    void addShape2d(const Shape2d& shape, ColorA clr);
    
    void addPath2d(const Path2d& path);
    void addPath2d(const Path2d& path, ColorA clr);
    
    void addColoursToShape(const Shape2d& shape, ColorA clr);
    void addColoursToPath(const Path2d& path, ColorA clr);
    
    void moveTo(Vec2f p);
    void lineTo(Vec2f p);
    void arcTo(const Vec2f &p, const Vec2f &t, float radius);
    void curveTo(const Vec2f &p, const Vec2f &p1, const Vec2f &p2 );
    void quadTo(const Vec2f &p1, const Vec2f &p2 );
    
    void setColor(ColorA clr);
    const ColorA getColor(ColorA clr);
    
    void end();

    //--------------------------------------------------------------

    
    const Shape2d& getShape2d();
    
    //--------------------------------------------------------------
    const vector<Point>& getPoints() const;
    
    //--------------------------------------------------------------
    void drawCalibration();
    
protected:
    
    //--------------------------------------------------------------
    Point transformPoint(Vec2f p, ColorA clr = ColorA::black()) const;
    
    //--------------------------------------------------------------
    void updateFinalPoints();
    
    //--------------------------------------------------------------
    void calculateIldaPoints();
    
    //--------------------------------------------------------------
    float getSegmentLength(const Path2d& path, int segment, int detail = 100 );
    
    //--------------------------------------------------------------
    float getPathLength(const Path2d& path, int detail = 100 );
    
protected:
    
//    CinderIldaFrameProcessor polyProcessor;
    vector<Point> points;   // final points to send
    Shape2d origShape;
    Shape2d processedShape;
    
    vector<ColorA>  mColorsContours;
    vector<ColorA>  mColorsSegments;
    
    ColorA mCurrentColor;
	
};


} // namespace ciilda

