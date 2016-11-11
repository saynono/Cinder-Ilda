#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

#include "CinderEtherdream.h"
#include "CinderLaserDac.h"
#include "CinderIldaFrame.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Example_EtherdreamApp : public App {
public:
    void setup();
    void shutdown();
    void mouseMove( MouseEvent event );
    void update();
    void draw();
    
private:
    
    ciilda::Frame               mIldaFrame;
    ciilda::LaserController*    mLaserDac;
    
    gl::TextureRef                 mTexture;
    Font                        mFont;
};

void Example_EtherdreamApp::setup()
{
    
    mLaserDac = new ciilda::Etherdream();
    mLaserDac->setup();
    
    mIldaFrame.params.output.targetPointCount = 200;
    mIldaFrame.params.output.blankCount = 7;
    mIldaFrame.params.output.endCount = 7;
    mIldaFrame.params.output.moveStepDivider = .05;
    mIldaFrame.params.draw.points = true;
    
    
    ColouredShape2d completeShape;
    
    ColouredShape2d shapeOrg;
    
    shapeOrg.color( ColorAf(0.0,0.0,1.0,1.0) );
    shapeOrg.moveTo(vec2(200,100));
    shapeOrg.lineTo(vec2(200,400));
    
    shapeOrg.color( ColorAf(0.0,1.0,0.0,1.0) );
    shapeOrg.moveTo(vec2(300,100));
    shapeOrg.lineTo(vec2(300,400));
    
    shapeOrg.color( ColorAf(1.0,0.0,0.0,1.0) );
    shapeOrg.moveTo(vec2(400,100));
    shapeOrg.lineTo(vec2(400,400));
    
    //    shapeOrg.close();
    
    
    ColouredShape2d triangle;
    triangle.color( ColorAf(.12,0.2,1,1) );
    triangle.moveTo(vec2(vec2(getWindowWidth()-30,30)));
    triangle.lineTo(vec2(600,450));
    triangle.color( ColorAf(.3,0.2,0,1) );
    triangle.lineTo(vec2(vec2(30,getWindowHeight()-30)));
    
    ColouredShape2d something;
    something.moveTo(vec2(300,100));
    something.curveTo(vec2(300,150),vec2(150,250),vec2(100,250));
    
    
    mat3 matrix;
    matrix = mat3();
    matrix = translate( matrix, -vec2(getWindowWidth(),getWindowHeight())/vec2(2.0,2.0) );
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    matrix = mat3();
    matrix = scale( matrix, vec2( 1.0/(float)getWindowWidth(), 1.0/(float)getWindowHeight()));
    
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    matrix = mat3();
    matrix = translate( matrix, vec2(0.5,0.5) );
    triangle.transform(matrix);
    something.transform(matrix);
    shapeOrg.transform(matrix);
    
    completeShape.appendColouredShape2d(shapeOrg);
    
    mIldaFrame.begin();
    mIldaFrame.addColouredShape2d(completeShape);
    mIldaFrame.end();
    
    gl::enableAlphaBlending();
    
    
    std::string normalFont( "Arial" );
    mFont = Font( normalFont, 14 );
    
    TextLayout simple;
    simple.setFont( mFont );
    simple.setColor( Color( 1, 1, 1 ) );
    simple.addLine( "nono" );
    mTexture = gl::Texture::create( simple.render( true, true ) );
    
    mIldaFrame.draw(0,0,100,100);
    
    
    for(int i=0;i<mIldaFrame.getShape2d().mSegmentColours.size() ; i++){
        console() << i << " COLOR :  " << mIldaFrame.getShape2d().mSegmentColours[i].r << "     ::  " << mIldaFrame.getShape2d().mSegmentColours[i].g << "    ::  " << mIldaFrame.getShape2d().mSegmentColours[i].b << std::endl;
    }
}


void Example_EtherdreamApp::shutdown(){
    mLaserDac->kill();
    console() << "Exit. ByeBye..." << std::endl;
}


void Example_EtherdreamApp::mouseMove( MouseEvent event )
{
}

void Example_EtherdreamApp::update()
{
    mLaserDac->setPoints(mIldaFrame);
    
    float fps = getFrameRate();
    
    TextLayout simple;
    simple.setFont( mFont );
    simple.setColor( Color( 1, 1, 1 ) );
    simple.addLine( "CinderIlda Test Frame" );
    simple.addLine( "Framerate: " + toString((int)fps) );
    mTexture = gl::Texture::create( simple.render( true, true ) );
    
    
    
    
    
}

void Example_EtherdreamApp::draw()
{
    
    gl::enableAlphaBlending();
    gl::clear( Color( 0, 0, 0 ) );
    
    gl::color(1, 1, 1);
    gl::draw( mTexture , vec2( 10, 10 ) );
    
    float w = getWindowWidth();
    float h = getWindowHeight();
    mIldaFrame.draw(0,0,w,h);
    
    
}


CINDER_APP( Example_EtherdreamApp, RendererGl (RendererGl::Options().stencil().msaa (16)),
           [&] (App::Settings * settings)
{
    settings->setWindowSize (800, 800);
    settings->setFrameRate (60.0f);
    settings->setTitle ("CinderIlda Test Frame");
    settings->setHighDensityDisplayEnabled();
})