#pragma once

#include "cinder/Cinder.h"
#include "cinder/Surface.h"



// Import ImagingSource Library
#define UDSHL_LIB_NO_COMPILER_CHECK
#include "tisudshl.h"

// TODO extend ofBaseVideoGrabber

class ImagingSource : DShowLib::GrabberListener {
public:
	ImagingSource();
	~ImagingSource();

	void listDevices();
	void setDeviceID(int id);
	void videoSettings();

	void setDeviceSerial(long long serial);
	void loadSettings(const std::string &fileName);

	ci::Surface8uRef getFrame();
	//ofPixels& getPixels();
	//const ofPixels& getPixels() const;

private:
	DShowLib::Grabber* m_pGrabber;
	smart_com<DShowLib::IFrameFilter> m_pSaveImageFilter; // hmm

	DShowLib::FrameHandlerSink::tFHSPtr frameHandlerSink;
	ci::Surface8uRef mFrame;

	// GrabberListener implementation
	virtual void frameReady(DShowLib::Grabber& caller, smart_ptr<DShowLib::MemBuffer> pBuffer, DWORD FrameNumber);
	//virtual void deviceLost(Grabber& caller);
	//virtual void overlayCallback(Grabber& caller, smart_ptr<OverlayBitmap> pBitmap, const tsMediaSampleDesc& desc);

};



