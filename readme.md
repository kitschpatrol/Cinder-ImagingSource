<!-- title({ prefix: "Cinder-" }) -->

# Cinder-ImagingSource

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**CinderBlock for The Imaging Source's IC Imaging Control camera SDK on Windows.**

<!-- /short-description -->

## Overview

An unofficial [CinderBlock](https://libcinder.org/docs/guides/cinder-blocks/index.html) for using [The Imaging Source](https://www.theimagingsource.com/en-us/) cameras with [Cinder](https://libcinder.org).

The Imaging Source's cameras have a few advantages over generic webcams in computer vision projects:

- Extensive programmatic control of exposure / shutter / gain / etc.
- Interchangeable lenses
- Higher framerates

The wrapper supports device selection by index or serial number, a camera settings dialog, XML configuration files, and live video or triggered still capture. Frames are returned as `ci::Surface8uRef` objects.

## Getting started

### Dependencies

Requires Cinder on Windows and an Imaging Source camera with its [device driver](https://www.theimagingsource.com/en-us/support/download/). SDK headers and x86/x64 libraries are included.

The block was written in early 2016 against [Cinder's master branch](https://github.com/cinder/Cinder/commit/8a4ac41621afcf840c27160057e1f54a721659a0) (0.9.1dev, between the 0.9.0 and 0.9.1 releases) with Visual Studio 2015. It hasn't been tested with later Cinder releases.

### Installation

From your Cinder `blocks` directory:

```sh
git clone https://github.com/kitschpatrol/Cinder-ImagingSource.git
```

Then add **ImagingSource** to a project with [TinderBox](https://libcinder.org/docs/guides/tinderbox/index.html). The block manifest configures SDK linking and copies the matching DLL beside the executable.

## Usage

Create an `ImagingSource` instance, open a camera, turn new frames into textures in `update()`, and draw them:

```cpp
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ImagingSource.h"

class MyApp : public ci::app::App {
public:
	void setup() override {
		mCamera.listDevices(); // Logs each camera's index, name, and serial number (debug builds only)
		mCamera.setDeviceID(0); // Opens the first camera and starts live capture
	}

	void update() override {
		if (ci::Surface8uRef frame = mCamera.getFrame()) {
			mTexture = ci::gl::Texture2d::create(*frame);
			mTexture->setTopDown(true);
		}
	}

	void draw() override {
		ci::gl::clear();
		if (mTexture) {
			ci::gl::draw(mTexture);
		}
	}

private:
	ImagingSource mCamera;
	ci::gl::Texture2dRef mTexture;
};

CINDER_APP(MyApp, ci::app::RendererGl)
```

`getFrame()` returns `nullptr` until the first frame arrives, and then returns the same surface, updated in place. Status messages, including the `listDevices()` output, are logged with `CI_LOG_V`, which Cinder compiles out of release builds unless `CI_MIN_LOG_LEVEL` is set to `0`.

The rest of the [ImagingSource.h](src/ImagingSource.h) API:

| Method                                      | Description                                                                                              |
| ------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| `setDeviceSerial(serial)`                   | Open a camera by serial number. Unlike the index, the serial doesn't depend on device enumeration order. |
| `videoSettings()`                           | Show the SDK's camera properties dialog. Capture pauses while it's open.                                 |
| `saveSettings(path)` / `loadSettings(path)` | Save or restore the camera's device state as an XML file.                                                |
| `setCaptureMode(mode)` / `getCaptureMode()` | Switch between `CaptureMode::VIDEO` (continuous) and `CaptureMode::STILL` (on demand).                   |
| `snapStill()`                               | Capture one frame in still mode.                                                                         |
| `setIsEnabled(enabled)` / `getIsEnabled()`  | Start or stop live capture.                                                                              |

## Examples

### ImagingSourceBasic

The [basic sample](samples/ImagingSourceBasic/src/ImagingSourceBasicApp.cpp) opens camera `0` and displays live frames. Open [ImagingSourceBasic.sln](samples/ImagingSourceBasic/vc2015/ImagingSourceBasic.sln) to build it; it includes Visual Studio 2015 projects for Win32 and x64. The sample projects expect the block to be in Cinder's `blocks` directory.

Its keyboard controls are:

| Key       | Action                                 |
| --------- | -------------------------------------- |
| `V`       | Open the camera settings dialog        |
| `S` / `L` | Save / load camera settings as XML     |
| `E`       | Toggle capture on or off               |
| `T`       | Switch between video and still capture |
| `Space`   | Trigger a frame in still mode          |

## Updating bundled libraries

The block embeds version 3.4.0.2550 of the IC Imaging Control SDK for convenience and portability. The [IC Imaging Control C++ SDK](https://www.theimagingsource.com/en-us/support/download/icimagingcontrolcpp-3.5.7/) is available from The Imaging Source. To refresh the bundled headers and libraries, review the installation paths in [copy_libraries.bat](copy_libraries.bat), then run it from the repository root. The script targets the IC Imaging Control 3.4 directory layout.

## Background

### Implementation notes

The wrapper drives the SDK's DirectShow-based `DShowLib::Grabber` with a single-buffer `FrameHandlerSink`. The SDK calls `frameReady()` on its own thread for each frame, and the wrapper copies the frame into a BGRX `ci::Surface8u`. Frames arrive bottom-up, so call `setTopDown(true)` on textures made from them.

Still mode uses the sink's snap mode: frames are only delivered when `snapStill()` requests one.

The bundled SDK headers stop compilation on Visual Studio versions newer than 2015 and on non-MSVC compilers, so `ImagingSource.h` defines `UDSHL_LIB_NO_COMPILER_CHECK` to skip that check. The 32-bit libraries are renamed with an `_x86` suffix so the block manifest can select the right ones through `$(PlatformTarget)`.

### Other projects

- Cinder's built-in [`ci::Capture`](https://libcinder.org/docs/classcinder_1_1_capture.html) can read from many Imaging Source cameras as generic video devices, but without the vendor property dialog, device state files, or still capture.
- The Imaging Source's [IC Imaging Control 4 examples](https://github.com/TheImagingSource/ic4-examples) cover the current generation of the SDK.
- [tiscamera](https://github.com/TheImagingSource/tiscamera) is The Imaging Source's SDK for Linux.

### Known issues

- Frames are always copied as 32-bit BGRX, but the sink also accepts RGB24, RGB565, RGB555, and RGB8. If the camera is set to one of those formats, frames will be corrupt and the copy reads past the end of the SDK's buffer. Keep the camera in an RGB32 format.
- The frame surface is allocated at the first frame's size. Changing the resolution afterward, for example through `videoSettings()`, isn't handled.
- `frameReady()` writes into the surface returned by `getFrame()` without a lock, so a frame can change while you're reading it.
- `setDeviceID()` throws `std::out_of_range` if the index is negative or not less than the number of cameras.
- `ImagingSource` owns a raw `Grabber` pointer but doesn't disable copying, so copying an instance leads to a double delete.
- Only Visual Studio 2015 project files are included.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/Cinder-ImagingSource/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © Eric Mika

<!-- /license -->

The MIT license covers the wrapper code in [`src`](src) and the sample. The IC Imaging Control SDK headers in [`include/icimagingcontrol`](include/icimagingcontrol) and the libraries in [`lib/msw`](lib/msw) are proprietary software from [The Imaging Source](https://www.theimagingsource.com/) and are not covered by the MIT license.
