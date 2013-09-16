

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "CinderEtherdream.h"
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
    
    setWindowSize(800, 800);
    
    mLaserDac = new ciilda::Etherdream();
    mLaserDac->setup();

    mIldaFrame.params.output.targetPointCount = 200;
    mIldaFrame.params.output.blankCount = 7;
    mIldaFrame.params.output.endCount = 7;
    mIldaFrame.params.output.moveStepDivider = .05;
    mIldaFrame.params.draw.points = true;

    
    ColouredShape2d completeShape;
    
    ColouredShape2d shapeOrg;
//    shapeOrg.color( ColorAf(1.0,1.0,0.0,1.0) );
//    shapeOrg.moveTo(Vec2f(100,100));
//    shapeOrg.lineTo(Vec2f(500,100));
//    shapeOrg.lineTo(Vec2f(500,450));
//    shapeOrg.color( ColorAf(.3,0.2,.4,1.0) );
//    shapeOrg.lineTo(Vec2f(100,450));
//    shapeOrg.lineTo(Vec2f(100,100));
    
    
    shapeOrg.color( ColorAf(0.0,0.0,1.0,1.0) );
    shapeOrg.moveTo(Vec2f(200,100));
    shapeOrg.lineTo(Vec2f(200,400));

    shapeOrg.color( ColorAf(0.0,1.0,0.0,1.0) );
    shapeOrg.moveTo(Vec2f(300,100));
    shapeOrg.lineTo(Vec2f(300,400));

    shapeOrg.color( ColorAf(1.0,0.0,0.0,1.0) );
    shapeOrg.moveTo(Vec2f(400,100));
    shapeOrg.lineTo(Vec2f(400,400));

//    shapeOrg.close();
    
    
    ColouredShape2d triangle;
    triangle.color( ColorAf(.12,0.2,1,1) );
    triangle.moveTo(Vec2f(Vec2f(getWindowWidth()-30,30)));
    triangle.lineTo(Vec2f(600,450));
    triangle.color( ColorAf(.3,0.2,0,1) );
    triangle.lineTo(Vec2f(Vec2f(30,getWindowHeight()-30)));
    
    ColouredShape2d something;
    something.moveTo(Vec2f(300,100));
    something.curveTo(Vec2f(300,150),Vec2f(150,250),Vec2f(100,250));

    
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
//    matrix.scale(.1);
//    matrix.scale(.75);

    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    matrix.setToIdentity();
    matrix.translate( Vec2f(0.5,0.5) );
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    completeShape.appendColouredShape2d(shapeOrg);
//    completeShape.appendColouredShape2d(triangle);
//    completeShape.appendColouredShape2d(something);

    

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
    
    
    for(int i=0;i<mIldaFrame.getShape2d().mSegmentColours.size() ; i++){
        console() << i << " COLOR :  " << mIldaFrame.getShape2d().mSegmentColours[i].r << "     ::  " << mIldaFrame.getShape2d().mSegmentColours[i].g << "    ::  " << mIldaFrame.getShape2d().mSegmentColours[i].b << std::endl;
    }
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
    
    float w = 800;
    float h = 800;
    gl::color(.1,.1,.1);
    gl::drawSolidRect(Rectf(0,0,w,h));
    gl::color(1, 1, 1);
    mIldaFrame.draw(0,0,w,h);
    
}

CINDER_APP_NATIVE( CinderIldaApp, RendererGl )
