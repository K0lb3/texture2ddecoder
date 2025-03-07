# texture2ddecoder

[![PyPI supported Python versions](https://img.shields.io/pypi/pyversions/UnityPy.svg)](https://pypi.python.org/pypi/texture2ddecoder)
[![Win/Mac/Linux](https://img.shields.io/badge/platform-windows%20%7C%20macos%20%7C%20linux-informational)]()
[![MIT](https://img.shields.io/pypi/l/UnityPy.svg)](https://github.com/K0lb3/texture2ddecoder/blob/master/LICENSE)
[![Build Status](https://github.com/K0lb3/texture2ddecoder/actions/workflows/test.yml/badge.svg)](https://github.com/K0lb3/texture2ddecoder/actions/workflows/test.yml)

A python wrapper for [Perfare](https://github.com/Perfare)'s [Texture2DDecoder](https://github.com/Perfare/AssetStudio/tree/master/Texture2DDecoder)

Some changes were made to the original code to make it cross-platform compatible.

1. [Installation](https://github.com/K0lb3/texture2ddecoder#installation)
2. [Example](https://github.com/K0lb3/texture2ddecoder#example)
3. [Functions](https://github.com/K0lb3/texture2ddecoder#functions)

## Installation

```cmd
pip install texture2ddecoder
```

or download/clone the git and use

```cmd
python setup.py install
```

## Example

```python
import texture2ddecoder

# load sample data
data = open("astc_sample", "rb").read()

# decode data
width = 512
height = 512
block_width = block_height = 4

decoded_data= texture2ddecoder.decode_astc(data, width, height, block_width, block_height)

# load raw image data
dec_img = Image.frombytes("RGBA", (width, height), decoded_data, 'raw', ("BGRA"))
```

## Functions

* All functions accept only arguments, no keywords.
* decode_pvrtc can cause segfaults on Mac OS

#### decode_bc1

*decompresses bc1 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_bc3

*decompresses bc3 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_pvrtc

*decompresses pvrtc textures to BGRA*

Params: bytes data, long width, long height, bool is2bpp
Return: bytes

#### decode_etc1

*decompresses etc1 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_etc2

*decompresses etc2 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_etc2a1

*decompresses etc2a1 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_etc2a8

*decompresses etc2a8 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_eacr

*decompresses eacr textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_eacr_signed

*decompresses eacr_signed textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_eacrg

*decompresses eacrg textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_eacrg_signed

*decompresses eacrg_signed textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_bc4

*decompresses bc4 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_bc5

*decompresses bc5 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_bc6

*decompresses bc6 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_bc7

*decompresses bc7 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_atc_rgb4

*decompresses atc_rgb4 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_atc_rgba8

*decompresses atc_rgba8 textures to BGRA*

Params: bytes data, long width, long height
Return: bytes

#### decode_astc

*decompresses astc textures to BGRA*

Params: bytes data, long width, long height, int block_width, int block_height
Return: bytes

#### unpack_crunch

*decompresses crunch textures to BGRA*

Params: bytes data
Return: bytes

#### unpack_unity_crunch

*decompresses unity_crunch textures to BGRA*

Params: bytes data
Return: bytes

## License & Credits

This module itself is licensed under MIT.

The texture compression codecs themselves were derived on following sources and therefore have following licenses:
| Codec          | License       | Source                                                                                                                                |
|----------------|---------------|---------------------------------------------------------------------------------------------------------------------------------------|
| ATC            | MIT           | [Perfare/AssetStudio - Texture2DDecoderNative/atc.cpp](https://github.com/Perfare/AssetStudio/tree/master/atc.cpp)                    |
| ASTC           | MIT           | [Ishotihadus/mikunyan - ext/decoders/native/astc.c](https://github.com/Ishotihadus/mikunyan/tree/master/ext/decoders/native/astc.c)   |
| BCn            | MIT           | [Perfare/AssetStudio - Texture2DDecoderNative/bcn.cpp](https://github.com/Perfare/AssetStudio/tree/master/bcn.cpp)                    |
| ETC            | MIT           | [Ishotihadus/mikunyan - ext/decoders/native/etc.c](https://github.com/Ishotihadus/mikunyan/tree/master/ext/decoders/native/etc.c)     |
| f16            | MIT           | [Maratyszcza/FP16](https://github.com/Maratyszcza/FP16)                                                                               |
| PVRTC          | MIT           | [Ishotihadus/mikunyan - ext/decoders/native/pvrtc.c](https://github.com/Ishotihadus/mikunyan/tree/master/ext/decoders/native/pvrtc.c) |
| Crunch         | PUBLIC DOMAIN | [BinomialLLC/crunch](https://github.com/BinomialLLC/crunch)                                                                           |
| Crunch (Unity) | ZLIB          | [Unity-Technologies/crunch](https://github.com/Unity-Technologies/crunch)                                                             |
