

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "CinderEasyLase.h"
#include "CinderLaserDac.h"
#include "CinderIldaFrame.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const bool PREMULT = false;


class CinderIldaApp : public AppNative {
    
    public:
        void setup();
        void shutdown();
        void mouseDown( MouseEvent event );
        void update();
        void draw();
    
    private:
    
        ciilda::Frame               mIldaFrame;
        ciilda::LaserController*    mLaserDac;
    
        gl::Texture                 mTexture;
        Font                        mFont;

};

void CinderIldaApp::setup()
{
    
//    setFrameRate(200);
    
    setWindowSize(800, 600);
    
    mLaserDac = new ciilda::EasyLase();
    mLaserDac->setup();
    
    mIldaFrame.params.output.targetPointCount = 400;
    mIldaFrame.params.output.blankCount = 4;
    mIldaFrame.params.output.endCount = 2;

    ColouredShape2d completeShape;
    
    ColouredShape2d shapeOrg;
    shapeOrg.color( ColorAf(1.0,0.5,.4,1.0) );
    shapeOrg.moveTo(Vec2f(100,100));
    shapeOrg.lineTo(Vec2f(500,100));
    shapeOrg.lineTo(Vec2f(500,450));
    shapeOrg.color( ColorAf(.3,0.2,.4,1.0) );
    shapeOrg.lineTo(Vec2f(100,450));
    shapeOrg.lineTo(Vec2f(100,100));
    //    shapeOrg.close();
    
    
    ColouredShape2d triangle;
    triangle.color( ColorAf(.12,0.2,1,1) );
    triangle.moveTo(Vec2f(Vec2f(getWindowWidth()-30,30)));
    triangle.lineTo(Vec2f(600,450));
    triangle.color( ColorAf(.3,0.2,0,1) );
    triangle.lineTo(Vec2f(Vec2f(30,getWindowHeight()-30)));
    
    completeShape.color( ColorAf(0,1,1,1) );
    completeShape.moveTo(Vec2f(300,100));
    completeShape.lineTo(Vec2f(150,250));
    completeShape.lineTo(Vec2f(100,250));
//    completeShape.curveTo(Vec2f(300,150),Vec2f(150,250),Vec2f(100,250));
    
    
    completeShape.appendColouredShape2d(shapeOrg);
    completeShape.appendColouredShape2d(triangle);
    
    
    MatrixAffine2f matrix;    
    matrix.setToIdentity();
    matrix.translate( -Vec2f(getWindowWidth(),getWindowHeight())/2.0 );
    completeShape.transform(matrix);

    matrix.setToIdentity();
    matrix.scale(Vec2f( 1.0/(float)getWindowWidth(), 1.0/(float)getWindowHeight()));
    matrix.scale(.1);
    completeShape.transform(matrix);

    matrix.setToIdentity();
    matrix.translate( Vec2f(0.5,0.5) );
    completeShape.transform( matrix );
    
    
    
    //    MatrixAffine2f matrix;
    //    matrix.scale(.5);
    //    matrix.translate(Vec2f(.5,.5));
    //    shapeOrg.transform(matrix);
    
    //    mIldaFrame.params.output.transform.scale = Vec2f(.5,.5);
    //    mIldaFrame.params.output.transform.offset = Vec2f(.5,.5);
    
    
    mIldaFrame.begin();
    mIldaFrame.addColouredShape2d(completeShape);
    mIldaFrame.end();
    
    gl::enableAlphaBlending();
    
    
    std::string normalFont( "Arial" );
    mFont = Font( normalFont, 14 );
    
    TextLayout simple;
	simple.setFont( mFont );
	simple.setColor( Color( 1, 1, 1 ) );
	simple.addLine( "Cinder" );
	simple.addLine( "Font From Resource" );
	mTexture = gl::Texture( simple.render( true, PREMULT ) );
    
    mIldaFrame.draw(0,0,100,100);
    
}

void CinderIldaApp::shutdown(){
    mLaserDac->kill();
    console() << "Exit. ByeBye..." << std::endl;
}


void CinderIldaApp::mouseDown( MouseEvent event )
{
}

void CinderIldaApp::update()
{
    mLaserDac->setPoints(mIldaFrame);
//    mLaserDac->send();
    
    float fps = getFrameRate();
    
    TextLayout simple;
	simple.setFont( mFont );
	simple.setColor( Color( 1, 1, 1 ) );
	simple.addLine( "CinderIlda" );
	simple.addLine( "Framerate: " + toString((int)fps) );
	mTexture = gl::Texture( simple.render( true, PREMULT ) );
    
}

void CinderIldaApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::draw( mTexture , Vec2f( 10, 10 ) );
    gl::color(.3, 1, 1);
    
    float w = 500;
    float h = 500;
    gl::color(.1,.1,.1);
    gl::drawSolidRect(Rectf(0,0,w,h));
    gl::color(1, 1, 1);
    mIldaFrame.draw(0,0,w,h);
    
}

CINDER_APP_NATIVE( CinderIldaApp, RendererGl )
