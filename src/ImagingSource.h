#pragma once

#include "cinder/Cinder.h"
#include "cinder/Surface.h"

// Import ImagingSource Library
#define UDSHL_LIB_NO_COMPILER_CHECK
#include "tisudshl.h"

// TODO extend ofBaseVideoGrabber

enum class CaptureMode {
	STILL,
	VIDEO,
};

class ImagingSource : DShowLib::GrabberListener {
public:
	ImagingSource();
	~ImagingSource();

	void listDevices();
	void setDeviceID(int id);
	void videoSettings();

	void setDeviceSerial(long long serial);
	void loadSettings(const ci::fs::path &path);
	void saveSettings(const ci::fs::path &path);

	void setCaptureMode(CaptureMode mode);
	CaptureMode getCaptureMode();

	ci::Surface8uRef getFrame();
	void snapStill();

	void setIsEnabled(bool value);
	bool getIsEnabled();

private:
	DShowLib::Grabber *mGrabber;
	DShowLib::FrameHandlerSink::tFHSPtr mFrameHandlerSink;
	ci::Surface8uRef mFrame;

	// GrabberListener implementation
	virtual void frameReady(DShowLib::Grabber &caller, smart_ptr<DShowLib::MemBuffer> pBuffer, DWORD FrameNumber);
	// virtual void deviceLost(Grabber& caller);
	// virtual void overlayCallback(Grabber& caller, smart_ptr<OverlayBitmap> pBitmap, const tsMediaSampleDesc& desc);
};
