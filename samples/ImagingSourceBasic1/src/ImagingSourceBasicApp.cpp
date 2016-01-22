#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagingSourceBasicApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ImagingSourceBasicApp::setup()
{
}

void ImagingSourceBasicApp::mouseDown( MouseEvent event )
{
}

void ImagingSourceBasicApp::update()
{
}

void ImagingSourceBasicApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( ImagingSourceBasicApp, RendererGl )
