# astc-transcoder
Transcoder is a [CLI](https://en.wikipedia.org/wiki/Command-line_interface) tool for saving 2D images to [KTXv1](https://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/) media container for [ASTC](https://www.khronos.org/opengl/wiki/ASTC_Texture_Compression) format with mipmap autogenerating.

## Supported formats
- `dds`
- `png`
- `jpg`
- `tga`
- `bmp`

## Deployment
Tutorial video is [here](https://drive.google.com/file/d/13Yli_vlsDumqeN2v-G_JgP3BvFET9agr/view?usp=sharing).

## How to use
Please refer to the [documentation](./documentation/manual.md).

## Quick start instructions
Requirements:

* _Windows Vista_+
* _Visual Studio 2019 v16.7.6_
* _Windows SDK v10.0.18362.0_
* [_AMD Compressonator_](https://github.com/GPUOpen-Tools/compressonator) _v4.0.4855.0_

Optional:
* [_InnoSetup_](https://jrsoftware.org/isdl.php) _v6.0.5 (u)_

To begin, clone this repository onto your local drive.

Next step is to compile project via _Visual Studio_ as usual.
