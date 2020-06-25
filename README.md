# texture2ddecoder

[![PyPI supported Python versions](https://img.shields.io/pypi/pyversions/UnityPy.svg)](https://pypi.python.org/pypi/texture2ddecoder)
[![Win/Mac/Linux](https://img.shields.io/badge/platform-windows%20%7C%20macos%20%7C%20linux-informational)]()
[![MIT](https://img.shields.io/pypi/l/UnityPy.svg)](https://github.com/K0lb3/texture2ddecoder/blob/master/LICENSE)
[![Build Status](https://travis-ci.com/K0lb3/texture2ddecoder.svg?branch=master)](https://travis-ci.com/K0lb3/texture2ddecoder)
[![Build Status (Github)](https://github.com/K0lb3/texture2ddecoder/workflows/Test%20and%20Publish/badge.svg?branch=master)](https://github.com/K0lb3/texture2ddecoder/actions?query=workflow%3A%22Test+and+Publish%22)

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
