//
//  CinderIldaPoint.h
//  LaserGateway
//
//  Created by say nono on 27.06.13.
//
//

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ciilda {
    
#define kIldaMinPoint -32768
//#define kIldaBitResolution 16
//#define kIldaMaxPoint pow(2, kIldaBitResolution)
#define kIldaMaxPoint 32768
#define kIldaDimension (kIldaMaxPoint - kIldaMinPoint)
#define kIldaMaxIntensity 65535
    
    class Point {
    public:
        Point() : x(0.5), y(0.5), r(0), g(0), b(0), a(0) {}
        Point(int16_t x, int16_t y, int16_t r=0, int16_t g=0, int16_t b=0, int16_t a=0): x(x), y(y), r(r), g(g), b(b), a(a) {}
        Point(Vec2f p, ColorA c, Vec2f pmin = Vec2f::zero(), Vec2f pmax = Vec2f::one()) { set(p, c, pmin, pmax); }
        Point(Vec2f p, Vec2f pmin = Vec2f::zero(), Vec2f pmax = Vec2f::one()) { setPosition(p, pmin, pmax); }
        
        int16_t x;
        int16_t y;
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
        uint16_t u1;  // what are these for? standard ILDA or just etherdream?
        uint16_t u2;
        
        //--------------------------------------------------------------
        void set(int16_t x, int16_t y) {
            this->x = x;
            this->y = y;
        }
        
        //--------------------------------------------------------------
        void set(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b, int16_t a) {
            this->x = x;
            this->y = y;
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
        
        
        //--------------------------------------------------------------
        // set color and position mapped from custom range (defaults to normalized)
        void set(Vec2f p, ColorA c, Vec2f pmin = Vec2f::zero(), Vec2f pmax = Vec2f::one()) {
            set(
                lmap<float>(p.x, pmin.x, pmax.x, kIldaMinPoint, kIldaMaxPoint),
                lmap<float>(p.y, pmin.y, pmax.y, kIldaMinPoint, kIldaMaxPoint),
                c.r * kIldaMaxIntensity,
                c.g * kIldaMaxIntensity,
                c.b * kIldaMaxIntensity,
                c.a * kIldaMaxIntensity
            );
        }
        
        //--------------------------------------------------------------
        // set position mapped from custom range (defaults to normalized)
        void setPosition(Vec2f p, Vec2f pmin = Vec2f::zero(), Vec2f pmax = Vec2f::one()) {
            set(
                lmap<float>(p.x, pmin.x, pmax.x, kIldaMinPoint, kIldaMaxPoint),
                lmap<float>(p.y, pmin.y, pmax.y, kIldaMinPoint, kIldaMaxPoint)
            );
        }
        
        
        //--------------------------------------------------------------
        // gets position of point mapped to desired range (defaults to normalized)
        Vec2f getPosition(Vec2f pmin = Vec2f::zero(), Vec2f pmax = Vec2f::one()) {
            return Vec2f(
                            lmap<float>(x, kIldaMinPoint, kIldaMaxPoint, pmin.x, pmax.x),
                            lmap<float>(y, kIldaMinPoint, kIldaMaxPoint, pmin.y, pmax.y)
            );
        }
        
    };
}