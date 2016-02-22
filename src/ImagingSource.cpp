#include "ImagingSource.h"

#include "cinder/app/App.h"
#include "cinder/Log.h"



ImagingSource::ImagingSource() {
	if (DShowLib::InitLibrary()) {
		CI_LOG_V("IC Imaging Control Library Initialized.");
	}
	else {
		CI_LOG_E("IC Imaging Control Library failed to initialized, unsupported platform.");
	}

	m_pGrabber = new DShowLib::Grabber();

/*
m_pSaveImageFilter = DShowLib::FilterLoader::createFilter("Save Image");
	if (m_pSaveImageFilter != NULL) {
		if (m_pGrabber->setDeviceFrameFilters(m_pSaveImageFilter.get()))
		{
			ofLogVerbose("Added Save Image filter");

		}
		else {
			ofLogError("Failed to add image filter.");

		}
	}
	else {
		ofLogError("Failed to load save image filter.");
	}
	*/

	// Create a FrameTypeInfoArray data structure describing the allowed color formats.
	DShowLib::FrameTypeInfoArray acceptedTypes = DShowLib::FrameTypeInfoArray::createRGBArray();
	frameHandlerSink = DShowLib::FrameHandlerSink::create(acceptedTypes, 1);
	frameHandlerSink->setSnapMode(false); // ?

	if (!m_pGrabber->setSinkType(frameHandlerSink)) {
		CI_LOG_E("Failed to set sink type.");
	}


	// register for events
	m_pGrabber->addListener(this, GrabberListener::eFRAMEREADY);

	//pixels.allocate(640, 480, 3);

}



ImagingSource::~ImagingSource() {
	delete m_pGrabber;
}

void ImagingSource::listDevices() {
	CI_LOG_V("Listing devices");
	
	DShowLib::Grabber::tVidCapDevListPtr deviceList = m_pGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		CI_LOG_V("No cameras detected.");
	}
	else {
		int index = 0;

		for (DShowLib::VideoCaptureDeviceItem &device : *deviceList) {
			long long serialNumber = 0;
			if (device.getSerialNumber(serialNumber)) {
				CI_LOG_V("Camera ID: " << index << "\t Name: " << device.getBaseName() << " \tSerial: " << serialNumber);
				index++;
			}
			else {
				CI_LOG_W("Could not get camera serial number");
			}
		}
	}
}


void ImagingSource::videoSettings() {

	bool wasLive = m_pGrabber->isLive();
	if (wasLive) {
		m_pGrabber->stopLive();
	}

	m_pGrabber->showDevicePage();

	if (wasLive) {
		m_pGrabber->startLive();
	}

}

void ImagingSource::setDeviceID(int id) {
	// get serial of device index
	DShowLib::Grabber::tVidCapDevListPtr deviceList = m_pGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		CI_LOG_V("No cameras detected.");
	}
	else {
		long long serialNumber = 0;
		if (deviceList->at(id).getSerialNumber(serialNumber)) {
			setDeviceSerial(serialNumber);
		}
		else {
			CI_LOG_E("Could not get camera serial number");
		}
	}
}

void ImagingSource::setDeviceSerial(long long serial) {
	// Todo close device

	if (m_pGrabber->openDev(serial)) {
		CI_LOG_V("Open camera with serial " << serial);


		if (m_pGrabber->startLive(false)) {
			CI_LOG_E("Camera is live.");

		}
		else {
			CI_LOG_E("Failed to take the camera live.");

		}

	}
	else {
		CI_LOG_E("Failed to open camera with serial " << serial);
	}
}


void ImagingSource::loadSettings(const std::string &fileName) {
	CI_LOG_V("Loading camera settings from " + fileName);

	
	if (m_pGrabber->loadDeviceStateFromFile(fileName)) {
		CI_LOG_E("Loaded camera settings successfully");
	}
	else {
		CI_LOG_E("Loading camera settings failed");
	}
}

void ImagingSource::frameReady(DShowLib::Grabber& caller, smart_ptr<DShowLib::MemBuffer> pBuffer, DWORD FrameNumber) {
	CI_LOG_V("Frame Ready! Number: " << FrameNumber);

	if (!mFrame) {
		mFrame = ci::Surface8u::create(pBuffer->getSize().cx, pBuffer->getSize().cy, true, cinder::SurfaceChannelOrder::ARGB);
	}

	int bpp = pBuffer->getBitsPerPixel();

	pBuffer->lock();
	memcpy(mFrame->getData(), pBuffer->getPtr(), pBuffer->getSize().cx * pBuffer->getSize().cy * 4);
	pBuffer->unlock();
}


//--------------------------------------------------------------------
ci::Surface8uRef ImagingSource::getFrame() {
	return mFrame;
}


/*/
void CamCapture::myQueryFrame(IplImage* frame) {

	//Grab one Image from Camera:

	if (!pSink.get() == NULL) {

		pSink->snapImages(1, 100);
	}
	else {
		cout << "could not read from camera" << endl;
		exit(1);
	}

	MemBufferCollection::tMemBufferPtr buffer = pSink->getLastAcqMemBuffer();

	IplImage* raw = cvCreateImage(NATIVEIMAGESIZE, 8, 3);
	raw->imageData = (char*)buffer->getPtr();

	cvResize(raw, frame);
	cvReleaseImage(&raw);

}
*/