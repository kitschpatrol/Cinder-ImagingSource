#include "ofxImagingSource.h"




ofxImagingSource::ofxImagingSource() {
	if (DShowLib::InitLibrary()) {
		ofLogVerbose("IC Imaging Control Library Initialized.");
	}
	else {
		ofLogError("IC Imaging Control Library failed to initialized, unsupported platform.");
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
		ofLogError("Failed to set sink type.");
	}


	// register for events
	m_pGrabber->addListener(this, GrabberListener::eFRAMEREADY);

	pixels.allocate(640, 480, 3);

}


ofxImagingSource::

ofxImagingSource::~ofxImagingSource() {
	delete m_pGrabber;
}

void ofxImagingSource::listDevices() {
	ofLogVerbose("Listing devices");
	
	DShowLib::Grabber::tVidCapDevListPtr deviceList = m_pGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		ofLogVerbose("No cameras detected.");
	}
	else {
		int index = 0;

		for (DShowLib::VideoCaptureDeviceItem &device : *deviceList) {
			long long serialNumber = 0;
			if (device.getSerialNumber(serialNumber)) {
				ofLogVerbose("Camera ID: " + ofToString(index) + "\t Name: " + device.getBaseName() + " \tSerial: " + ofToString(serialNumber));
				index++;
			}
			else {
				ofLogError("Could not get camera serial number");
			}
		}
	}
}


void ofxImagingSource::videoSettings() {

	bool wasLive = m_pGrabber->isLive();
	if (wasLive) {
		m_pGrabber->stopLive();
	}

	m_pGrabber->showDevicePage();

	if (wasLive) {
		m_pGrabber->startLive();
	}

}

void ofxImagingSource::setDeviceID(int id) {
	// get serial of device index
	DShowLib::Grabber::tVidCapDevListPtr deviceList = m_pGrabber->getAvailableVideoCaptureDevices();

	if (deviceList == 0 || deviceList->empty()) {
		ofLogVerbose("No cameras detected.");
	}
	else {
		long long serialNumber = 0;
		if (deviceList->at(id).getSerialNumber(serialNumber)) {
			setDeviceSerial(serialNumber);
		}
		else {
			ofLogError("Could not get camera serial number");
		}
	}
}

void ofxImagingSource::setDeviceSerial(long long serial) {
	// Todo close device

	if (m_pGrabber->openDev(serial)) {
		ofLogVerbose("Open camera with serial " + ofToString(serial));


		if (m_pGrabber->startLive(false)) {
			ofLogError("Camera is live.");

		}
		else {
			ofLogError("Failed to take the camera live.");

		}

	}
	else {
		ofLogError("Failed to open camera with serial " + ofToString(serial));

	}
}


void ofxImagingSource::loadSettings(const string &fileName) {
	ofLogVerbose("Loading camera settings from " + fileName);


	if (m_pGrabber->loadDeviceStateFromFile(ofToDataPath(fileName, true))) {
		ofLogError("Loaded camera settings successfully");
	}
	else {
		ofLogError("Loading camera settings failed");
	}
}

void ofxImagingSource::frameReady(DShowLib::Grabber& caller, smart_ptr<DShowLib::MemBuffer> pBuffer, DWORD FrameNumber) {
	ofLogVerbose("Frame Ready! Number: " + ofToString(FrameNumber));
	pBuffer->lock();
		pixels.setFromPixels(pBuffer->getPtr(), pBuffer->getSize().cx, pBuffer->getSize().cy, OF_IMAGE_COLOR);

	pBuffer->unlock();


}


//--------------------------------------------------------------------
void ofxImagingSource::clearMemory() {
	pixels.clear();
}

//---------------------------------------------------------------------------
ofPixels & ofxImagingSource::getPixels() {
	return pixels;
}

//---------------------------------------------------------------------------
const ofPixels & ofxImagingSource::getPixels() const {
	return pixels;
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