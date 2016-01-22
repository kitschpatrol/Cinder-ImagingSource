#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "ImagingSource.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagingSourceBasicApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	ImagingSource mImagingSource;
	ci::gl::Texture2dRef mFrameTexture;

	

};

void ImagingSourceBasicApp::setup()
{
	mImagingSource.listDevices();
	mImagingSource.setDeviceID(0);
}

void ImagingSourceBasicApp::mouseDown( MouseEvent event )
{
}

void ImagingSourceBasicApp::update()
{
	if (mImagingSource.getFrame() != nullptr) {
		mFrameTexture = gl::Texture2d::create(*mImagingSource.getFrame());
	}

}

void ImagingSourceBasicApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 

	gl::draw(mFrameTexture);

}

CINDER_APP( ImagingSourceBasicApp, RendererGl )
