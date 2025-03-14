def decode_bc1(texture: bytes, width: int, height: int) -> bytes: ...
def decode_bc3(texture: bytes, width: int, height: int) -> bytes: ...
def decode_pvrtc(
    texture: bytes, width: int, height: int, is2bpp: bool = False
) -> bytes: ...
def decode_etc1(texture: bytes, width: int, height: int) -> bytes: ...
def decode_etc2(texture: bytes, width: int, height: int) -> bytes: ...
def decode_etc2a1(texture: bytes, width: int, height: int) -> bytes: ...
def decode_etc2a8(texture: bytes, width: int, height: int) -> bytes: ...
def decode_eacr(texture: bytes, width: int, height: int) -> bytes: ...
def decode_eacr_signed(texture: bytes, width: int, height: int) -> bytes: ...
def decode_eacrg(texture: bytes, width: int, height: int) -> bytes: ...
def decode_eacrg_signed(texture: bytes, width: int, height: int) -> bytes: ...
def decode_bc4(texture: bytes, width: int, height: int) -> bytes: ...
def decode_bc5(texture: bytes, width: int, height: int) -> bytes: ...
def decode_bc6(texture: bytes, width: int, height: int) -> bytes: ...
def decode_bc7(texture: bytes, width: int, height: int) -> bytes: ...
def decode_atc_rgb4(texture: bytes, width: int, height: int) -> bytes: ...
def decode_atc_rgba8(texture: bytes, width: int, height: int) -> bytes: ...
def decode_astc(
    data: bytes, width: int, height: int, block_width: int, block_height: int
) -> bytes: ...
def unpack_crunch(data: bytes) -> bytes: ...
def unpack_unity_crunch(data: bytes) -> bytes: ...
