#Cinder-ImagingSource

Unofficial [Cinder](https://libcinder.org) block wrapping [The Imaging Source](http://www.theimagingsource.com/en_US/)'s camera control SDK for Windows.

The Imaging Source's cameras have a few advantages over generic webcams in computer vision projects:

- Extensive programmatic control of exposure / shutter / gain / etc.
- Interchangeable lenses
- Higher framerates

This addon isn't strictly necessary to use Imaging Source cameras with Cinder.

Most Imaging Source cameras are recognized as UVC devices. This gives you some control over exposure etc., but the disadvantage is that you can't control camera parameters programmatically through Cinder, or save / reload configuration files.

Embeds version 3.4.0.2550 of the [IC Imaging Control SDK](http://www.theimagingsource.com/en_US/support/downloads/details/icimagingcontrol/) for convenience and portability. If you'd like to update, install the latest IC Imaging Control SDK normally, then use the `copy_libraries.bat` script to pull dependenceis from their default install locations into the block's repo.

Note that you still need to install the appropriate [driver for your camera](http://www.theimagingsource.com/en_US/support/downloads/).
