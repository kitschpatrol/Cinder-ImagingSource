#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ImagingSource.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagingSourceBasicApp : public App {
public:
	void setup() override;
	void keyDown(KeyEvent event) override;

	void update() override;
	void draw() override;

private:
	ImagingSource mImagingSource;
	ci::gl::Texture2dRef mFrameTexture;
};

void ImagingSourceBasicApp::setup() {
	mImagingSource.listDevices();
	mImagingSource.setDeviceID(0);
}

void ImagingSourceBasicApp::keyDown(KeyEvent event) {
	switch (event.getCode()) {
		case KeyEvent::KEY_s: {
			// Save settings to XML file
			fs::path settingsPath = getAssetPath("/") / "settings.xml";
			mImagingSource.saveSettings(settingsPath);
		} break;
		case KeyEvent::KEY_l: {
			// Load Settings from XML file
			fs::path settingsPath = getAssetPath("/") / "settings.xml";
			mImagingSource.loadSettings(settingsPath);
		} break;
		case KeyEvent::KEY_v: {
			// Pop settings window
			mImagingSource.videoSettings();
		} break;
		case KeyEvent::KEY_e: {
			// Toggle camera enabled
			mImagingSource.setIsEnabled(!mImagingSource.getIsEnabled());
		} break;
		case KeyEvent::KEY_t: {
			// Toggle capture mode
			switch (mImagingSource.getCaptureMode()) {
				case CaptureMode::STILL:
					mImagingSource.setCaptureMode(CaptureMode::VIDEO);
					break;
				case CaptureMode::VIDEO:
					mImagingSource.setCaptureMode(CaptureMode::STILL);
					break;
			}
		} break;
		case KeyEvent::KEY_SPACE: {
			mImagingSource.snapStill();
		} break;
	}
}

void ImagingSourceBasicApp::update() {
	if (mImagingSource.getFrame() != nullptr) {
		mFrameTexture = gl::Texture2d::create(*mImagingSource.getFrame());
		mFrameTexture->setTopDown(true);
	}
}

void ImagingSourceBasicApp::draw() {
	gl::clear(Color(0, 0, 0));

	if (mFrameTexture) {
		const float scale = (float)getWindowWidth() / (float)mFrameTexture->getWidth();
		gl::ScopedModelMatrix scopedMatrix;
		gl::scale(scale, scale);
		gl::draw(mFrameTexture);
	}
}

CINDER_APP(ImagingSourceBasicApp, RendererGl)
