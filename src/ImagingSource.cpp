#include "ImagingSource.h"

#include "cinder/Log.h"
#include "cinder/app/App.h"

ImagingSource::ImagingSource() {
	if (DShowLib::InitLibrary()) {
		CI_LOG_V("IC Imaging Control Library Initialized.");
	} else {
		CI_LOG_E("IC Imaging Control Library failed to initialized, unsupported platform.");
	}

	mGrabber = new DShowLib::Grabber();

	// Create a FrameTypeInfoArray data structure describing the allowed color formats.
	DShowLib::FrameTypeInfoArray acceptedTypes = DShowLib::FrameTypeInfoArray::createRGBArray();
	mFrameHandlerSink = DShowLib::FrameHandlerSink::create(acceptedTypes, 1);
	mFrameHandlerSink->setSnapMode(false);

	if (!mGrabber->setSinkType(mFrameHandlerSink)) {
		CI_LOG_E("Failed to set sink type.");
	}

	// register for events
	mGrabber->addListener(this, GrabberListener::eFRAMEREADY);
}

ImagingSource::~ImagingSource() {
	delete mGrabber;
}

void ImagingSource::listDevices() {
	CI_LOG_V("Listing devices");

	DShowLib::Grabber::tVidCapDevListPtr deviceList = mGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		CI_LOG_V("No cameras detected.");
	} else {
		int index = 0;

		for (DShowLib::VideoCaptureDeviceItem &device : *deviceList) {
			long long serialNumber = 0;
			if (device.getSerialNumber(serialNumber)) {
				CI_LOG_V("Camera ID: " << index << "\t Name: " << device.getBaseName() << " \tSerial: " << serialNumber);
				index++;
			} else {
				CI_LOG_W("Could not get camera serial number");
			}
		}
	}
}

void ImagingSource::videoSettings() {
	CI_LOG_V("Showing camera settings window.");

	const bool wasLive = mGrabber->isLive();
	if (wasLive) {
		mGrabber->stopLive();
	}

	mGrabber->showVCDPropertyPage((HWND)ci::app::getWindow()->getNative());

	if (wasLive) {
		mGrabber->startLive(false);
	}
}

void ImagingSource::setDeviceID(int id) {
	// get serial of device index
	DShowLib::Grabber::tVidCapDevListPtr deviceList = mGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		CI_LOG_V("No cameras detected.");
	} else {
		long long serialNumber = 0;
		if (deviceList->at(id).getSerialNumber(serialNumber)) {
			setDeviceSerial(serialNumber);
		} else {
			CI_LOG_E("Could not get camera serial number");
		}
	}
}

void ImagingSource::setDeviceSerial(long long serial) {
	// Todo close device

	if (mGrabber->openDev(serial)) {
		CI_LOG_V("Open camera with serial number " << serial);

		if (mGrabber->startLive(false)) {
			CI_LOG_V("Camera is live.");

		} else {
			CI_LOG_E("Failed to take the camera live.");
		}

	} else {
		CI_LOG_E("Failed to open camera with serial number " << serial);
	}
}

void ImagingSource::loadSettings(const ci::fs::path &path) {
	CI_LOG_V("Loading camera settings from " << path);

	const bool wasLive = mGrabber->isLive();
	if (wasLive) {
		mGrabber->stopLive();
	}

	if (mGrabber->loadDeviceStateFromFile(path.string())) {
		CI_LOG_V("Loaded camera settings successfully");
	} else {
		CI_LOG_E("Loading camera settings failed");
	}

	if (wasLive) {
		mGrabber->startLive(false);
	}
}

void ImagingSource::saveSettings(const ci::fs::path &path) {
	CI_LOG_V("Saving camera settings to " << path);

	if (mGrabber->saveDeviceStateToFile(path.string())) {
		CI_LOG_E("Saved camera settings successfully");
	} else {
		CI_LOG_E("Save camera settings failed");
	}
}

void ImagingSource::setCaptureMode(CaptureMode mode) {
	switch (mode) {
		case CaptureMode::STILL:
			mFrameHandlerSink->setSnapMode(true);
			break;
		case CaptureMode::VIDEO:
			mFrameHandlerSink->setSnapMode(false);
			break;
	}
}

CaptureMode ImagingSource::getCaptureMode() {
	return mFrameHandlerSink->getSnapMode() ? CaptureMode::STILL : CaptureMode::VIDEO;
}

void ImagingSource::frameReady(DShowLib::Grabber &caller, smart_ptr<DShowLib::MemBuffer> pBuffer, DWORD FrameNumber) {
	// CI_LOG_V("Frame Ready! Number: " << FrameNumber);

	if (!mFrame) {
		mFrame = ci::Surface8u::create(pBuffer->getSize().cx, pBuffer->getSize().cy, true, cinder::SurfaceChannelOrder::BGRX);
	}

	pBuffer->lock();
	memcpy(mFrame->getData(), pBuffer->getPtr(), pBuffer->getSize().cx * pBuffer->getSize().cy * 4);
	pBuffer->unlock();
}

ci::Surface8uRef ImagingSource::getFrame() {
	return mFrame;
}

void ImagingSource::snapStill() {
	mFrameHandlerSink->snapImagesAsync(1);
}

void ImagingSource::setIsEnabled(bool value) {
	if (value) {
		mGrabber->startLive(false);
	} else {
		mGrabber->stopLive();
	}
}

bool ImagingSource::getIsEnabled() {
	return mGrabber->isLive();
}
