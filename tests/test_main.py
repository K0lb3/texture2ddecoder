import texture2ddecoder
import os
import json
from zipfile import ZipFile
from sys import platform
from PIL import Image, ImageChops

root = os.path.dirname(os.path.abspath(__file__))
zip = ZipFile(os.path.join(root, "samples.zip"))

def test_atc_rgb4():
    _test("ATC_RGB4", texture2ddecoder.decode_atc_rgb4)

def test_atc_rgba8():
    _test("ATC_RGBA8", texture2ddecoder.decode_atc_rgba8)

def test_pvrtc_rgb4():
    _test("PVRTC_RGB4", texture2ddecoder.decode_pvrtc)

def test_pvrtc_rgba2():
    _test("PVRTC_RGBA2", texture2ddecoder.decode_pvrtc)

def test_etc():
    _test("ETC_RGB4", texture2ddecoder.decode_etc1)

def test_etc2():
    _test("ETC2_RGB", texture2ddecoder.decode_etc2)
    _test("ETC2_RGBA8", texture2ddecoder.decode_etc2a8)

def test_etc_crunched():
    _test_crunched("ETC_RGB4Crunched", texture2ddecoder.decode_etc1, True)

def test_etc2_crunched():
    _test_crunched("ETC2_RGBA8Crunched", texture2ddecoder.decode_etc2a8, True)

def test_astc():
    for name in zip.namelist():
        if "ASTC" not in name or name[-5:] != ".data":
            continue
        name = name[:-5]

        # load sample data
        data : bytes= zip.open(name+".data", "r").read()
        details : dict = json.loads(zip.open(name+".json", "r").read())
        ori_img : Image = Image.open(zip.open(name+".png", "r"))

        # decompress data
        width = details["m_Width"]
        height = details["m_Height"]
        bw = bh = int(name.rsplit("x",1)[1])
        dec = texture2ddecoder.decode_astc(data, width, height, bw, bh)

        # load raw image data
        dec_img = Image.frombytes("RGBA", (width, height), dec, 'raw', ("BGRA"))
        dec_img = dec_img.convert(ori_img.mode)
        # compare images
        #assert(ImageChops.difference(ori_img, dec_img).getbbox() is None)

def _test(name, func):
    # load sample data
    data : bytes= zip.open(name+".data", "r").read()
    details : dict = json.loads(zip.open(name+".json", "r").read())
    ori_img : Image = Image.open(zip.open(name+".png", "r"))

    # decompress data
    width = details["m_Width"]
    height = details["m_Height"]
    dec = func(data, width, height)

    # load raw image data
    dec_img = Image.frombytes("RGBA", (width, height), dec, 'raw', ("BGRA"))
    dec_img = dec_img.convert(ori_img.mode)
    # compare images
    if "PVRTC" in name:
        return
    assert(ImageChops.difference(ori_img, dec_img).getbbox() is None)

def _test_crunched(name, func, unity = False):
    # load sample data
    data : bytes= zip.open(name+".data", "r").read()
    details : dict = json.loads(zip.open(name+".json", "r").read())
    ori_img : Image = Image.open(zip.open(name+".png", "r"))

    if unity:
        data = texture2ddecoder.unpack_unity_crunch(data) 
    else:
        data = texture2ddecoder.unpack_crunch(data)
    
    # decompress data
    width = details["m_Width"]
    height = details["m_Height"]
    dec = func(data, width, height)

    # load raw image data
    dec_img = Image.frombytes("RGBA", (width, height), dec, 'raw', ("BGRA"))
    dec_img = dec_img.convert(ori_img.mode)
    # compare images
    assert(ImageChops.difference(ori_img, dec_img).getbbox() is None)

if __name__ == "__main__":
    for x in list(locals()):
        if str(x)[:4] == "test":
            locals()[x]()
    input("All Tests Passed")
    
