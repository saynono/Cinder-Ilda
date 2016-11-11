//
//  ColouredShape2d.cpp
//  QLT_GenomeLaser
//
//  Created by say nono on 28.07.13.
//  Copyright (c) 2013 http://say-nono.com. All rights reserved.
//

#include "ColouredShape2d.h"


ColouredShape2d::ColouredShape2d(){
    mCurrentColour = ColorAf(1,1,1,1);
}

void ColouredShape2d::clear(){
    Shape2d::clear();
    mSegmentColours.clear();
    mCurrentColour = ColorAf(1,1,1,1);
}

void ColouredShape2d::color( ColorAf clr ){
    mCurrentColour = clr;
}

const ColorAf ColouredShape2d::getCurrentColor(){
    return mCurrentColour;
}
vector<ColorAf> ColouredShape2d::getColors() const{
    return mSegmentColours;
}
const ColorAf ColouredShape2d::getSegmentColor(int seg){
//    console() << "NumSegmentColours : " << mSegmentColours.size() << "      requesting : " << << std::endl;
    // TODO : Vincent: Really bad.... need to fix this
    return mSegmentColours[min(seg,(int)mSegmentColours.size()-1)];
}

//--------------------------------------------------------------

void ColouredShape2d::appendColouredShape2d(const ColouredShape2d& path){
    Shape2d::append( path );
    const vector<ColorAf> clrs = path.getColors();
    for(int i=0;i<clrs.size();i++){
        mSegmentColours.push_back( clrs[i] );
    }
}

void ColouredShape2d::appendShape2d(const Shape2d& shape){
    for(int i=0;i<shape.getNumContours();i++){
        appendPath2d(shape.getContour(i));
    }
}

void ColouredShape2d::appendPath2d(const Path2d& path){
    Shape2d::appendContour(path);
    mSegmentColours.push_back(mCurrentColour);
    addColoursToPath(path, mCurrentColour);
}

void ColouredShape2d::addColoursToPath(const Path2d& path, ColorA clr){
    for(int i=0;i<path.getNumSegments();i++){
        mSegmentColours.push_back(clr);
    }
}



//--------------------------------------------------------------

void ColouredShape2d::moveTo( const vec2 &p ){
    // TODO: Check if this is right!
    Shape2d::moveTo( p );
//    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::moveTo( float x, float y ){
    Shape2d::moveTo( x, y );
    // TODO: Check if this is right!
//    if(prevcontour no segments) remmove that contour!
//    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::lineTo( const vec2 &p ){
    Shape2d::lineTo( p );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::lineTo( float x, float y ){
    Shape2d::lineTo( x, y );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::quadTo( const vec2 &p1, const vec2 &p2 ){
    Shape2d::quadTo( p1, p2 );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::quadTo( float x1, float y1, float x2, float y2 ){
    Shape2d::quadTo( x1, y1, x2, y2 );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::curveTo( const vec2 &p1, const vec2 &p2, const vec2 &p3 ){
    Shape2d::curveTo( p1, p2, p3 );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::curveTo( float x1, float y1, float x2, float y2, float x3, float y3 ){
    Shape2d::curveTo( x1, y1, x2, y2, x3, y3 );
    mSegmentColours.push_back( mCurrentColour );
}


void ColouredShape2d::arc( const vec2 &center, float radius, float startRadians, float endRadians, bool forward ){
    Shape2d::arc( center, radius, startRadians, endRadians, forward );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::arc( float centerX, float centerY, float radius, float startRadians, float endRadians, bool forward ){
    Shape2d::arc( centerX, centerY, radius, startRadians, endRadians, forward);
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::arcTo( const vec2 &p, const vec2 &t, float radius ){
    Shape2d::arcTo( p, t, radius );
    mSegmentColours.push_back( mCurrentColour );
}

void ColouredShape2d::arcTo( float x, float y, float tanX, float tanY, float radius){
    Shape2d::arcTo( x, y, tanX, tanY, radius);
    mSegmentColours.push_back( mCurrentColour );
}




void ColouredShape2d::draw(){

    Path2d path;
    int pointCounter;
    int pathType;
    int segCounter = 0;
    float detail = 50.0f;
    vec2 startPoint;
    ColorAf clr = ColorA::white();
    
    gl::lineWidth(1);
    for(int i=0; i<getNumContours(); i++) {
        path = getContour(i);
        pointCounter = 0;
        startPoint = path.getPoint(pointCounter++);
        clr = mSegmentColours[segCounter];
        gl::color(clr);
        gl::begin(GL_LINE_STRIP);
        gl::vertex(startPoint);
        for(int j=0;j<path.getNumSegments();j++){
            pathType = path.getSegmentType(j);
            clr = mSegmentColours[segCounter];
            gl::color(clr);
            if(pathType == Path2d::LINETO){
                gl::vertex( path.getPoint(pointCounter++) );
            }else if(pathType == Path2d::CLOSE ){
                gl::vertex( startPoint );
            }else if(pathType == Path2d::MOVETO){
                //                        gl::moveTo(path.getPoint(pointCounter++));
            }else{
                
                if(pathType == Path2d::CUBICTO) pointCounter += 3;
                else if(pathType == Path2d::QUADTO) pointCounter += 2;
                
                float step = 1.0/detail;
                for(float percent=step;percent<=1;percent+=step){
                    gl::vertex( path.getSegmentPosition(j, percent) );
                }
            }
            segCounter++;
        }
        gl::end();
//        segCounter++;
    }
}

