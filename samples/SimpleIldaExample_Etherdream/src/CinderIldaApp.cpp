

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "CinderEtherdream.h"
#include "CinderIldaFrame.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const bool PREMULT = false;


class CinderIldaApp : public AppNative {
    
    public:
        void setup();
        void mouseDown( MouseEvent event );
        void update();
        void draw();
    
    private:
    
        ciilda::Frame               mIldaFrame;
        ciilda::Etherdream          mEtherdream;
    
        gl::Texture                 mTexture;
        Font                        mFont;

};

void CinderIldaApp::setup()
{
    
//    setFrameRate(200);
    
    setWindowSize(800, 600);
    
    Shape2d shapeOrg;
    shapeOrg.moveTo(Vec2f(100,100));
    shapeOrg.lineTo(Vec2f(500,100));
    shapeOrg.lineTo(Vec2f(500,450));
    shapeOrg.lineTo(Vec2f(100,450));
    shapeOrg.close();
    
    
    Path2d triangle;
    triangle.moveTo(Vec2f(Vec2f(getWindowWidth()-30,30)));
    triangle.lineTo(Vec2f(600,450));
    triangle.lineTo(Vec2f(Vec2f(30,getWindowHeight()-30)));
    
    Path2d something;
    something.moveTo(Vec2f(300,100));
    something.curveTo(Vec2f(300,150),Vec2f(150,250),Vec2f(100,250));
    //    something.close();

    MatrixAffine2f matrix;
    
    matrix.setToIdentity();
    matrix.translate( -Vec2f(getWindowWidth(),getWindowHeight())/2.0 );
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);

    matrix.setToIdentity();
//    matrix.translate(-Vec2f(getWindowWidth(),getWindowHeight())/2.0);
//    matrix.transformPoint(Vec2f(getWindowWidth(),getWindowHeight())/2.0);
    matrix.scale(Vec2f( 1.0/(float)getWindowWidth(), 1.0/(float)getWindowHeight()));
    matrix.scale(.1);
//    matrix.scale(.25);

    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    matrix.setToIdentity();
    matrix.translate( Vec2f(0.5,0.5) );
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    

    

//    MatrixAffine2f matrix;
//    matrix.scale(.5);
//    matrix.translate(Vec2f(.5,.5));
//    shapeOrg.transform(matrix);
    
//    mIldaFrame.params.output.transform.scale = Vec2f(.5,.5);
//    mIldaFrame.params.output.transform.offset = Vec2f(.5,.5);
    
    mIldaFrame.params.output.targetPointCount = 200;
    
    mIldaFrame.begin();
    mIldaFrame.addShape2d(shapeOrg,ColorA(1,0,0,1));
    mIldaFrame.addPath2d(something);
    mIldaFrame.addPath2d(triangle, ColorA(1,0,1,1));
    mIldaFrame.moveTo(Vec2f(.3,.2));
    mIldaFrame.lineTo(Vec2f(.6,.2));
    mIldaFrame.lineTo(Vec2f(.6,.6));
    mIldaFrame.end();
    
    
    mEtherdream.setup();
    
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

void CinderIldaApp::mouseDown( MouseEvent event )
{
}

void CinderIldaApp::update()
{
    mEtherdream.setPoints(mIldaFrame);
    
    
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
