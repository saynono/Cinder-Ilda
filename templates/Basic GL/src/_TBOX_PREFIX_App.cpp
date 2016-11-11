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

class _TBOX_PREFIX_App : public App {
  public:
	void prepareSettings( Settings* settings );
	void setup();
	void update();
	void draw();
	
    ciilda::Frame               mIldaFrame;
    ciilda::Etherdream          mEtherdream;

};

void _TBOX_PREFIX_App::prepareSettings( Settings* settings )
{
	settings->setWindowSize( 1280, 480 );
}

void _TBOX_PREFIX_App::setup()
{
	mEtherdream.setup();
}

void _TBOX_PREFIX_App::update()
{	
	mEtherdream.setPoints(mIldaFrame);
}

void _TBOX_PREFIX_App::draw()
{
	
	gl::clear( Color( 0, 0, 0 ) );

    float w = 500;
    float h = 500;
    gl::color(.1,.1,.1);
    gl::drawSolidRect(Rectf(0,0,w,h));
    gl::color(1, 1, 1);
    mIldaFrame.draw(0,0,w,h);
}

CINDER_APP( _TBOX_PREFIX_App, RendererGl )
