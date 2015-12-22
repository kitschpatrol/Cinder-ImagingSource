#Cinder-ImagingSource

[Cinder](https://libcinder.org) block wrapping [The Imaging Source](http://www.theimagingsource.com/en_US/)'s camera control SDK for Windows.

The Imaging Source's cameras have a few advantages over generic webcams in computer vision projects:

- Extensive programmatic control of exposure / shutter / gain / etc.
- Interchangeable lenses
- Higher framerates

This addon isn't strictly necessary to use Imaging Source cameras.

Most Imaging Source cameras are recognized as UVC devices. This gives you some control over exposure etc., but the disadvantage is that you can't control camera parameters programmatically through Cinder, or save / reload configuration files.

#WIP