"""Portable semantic tests. Native comparison remains a separate IDO gate."""
import ctypes
import itertools
from pathlib import Path
import random
import subprocess
import pytest

HERE = Path(__file__).resolve().parents[2] / "cloud/work/solid_rectangle_8008A46C"

@pytest.fixture(scope="module")
def runner(tmp_path_factory):
    lib = tmp_path_factory.mktemp("rectangle") / "rectangle.so"
    subprocess.run(["cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                    "-shared", "-fPIC", "-O2", str(HERE / "host_harness.c"), "-o", str(lib)], check=True)
    fn = ctypes.CDLL(str(lib)).rectangle_run
    fn.argtypes = [ctypes.c_int]*4 + [ctypes.POINTER(ctypes.c_ubyte)] + [ctypes.c_int]*4 + [ctypes.POINTER(ctypes.c_uint)]
    fn.restype = ctypes.c_int
    def run(rect, clip=(0, 0, 319, 239), rgba=(17, 34, 51, 68)):
        color = (ctypes.c_ubyte*4)(*rgba)
        out = (ctypes.c_uint*68)()
        assert fn(*rect, color, *clip, out) == 1
        l,t,r,b = rect
        cl,ct,cr,cb = clip
        l,t,r,b = max(l,cl),max(t,ct),min(r,cr),min(b,cb)
        if r < l or b < t:
            assert list(out[:4]) == [0,0xA5,0,1]
            assert all(x == 0xDEADBEEF for x in out[4:])
        else:
            assert list(out[:4]) == [8,rgba[3],4,1]
            expected = [0xE7000000,0,0xFA000000,
                        int.from_bytes(bytes(rgba), "big"),
                        0xAB000001,0xFFFFFFFF,0xAB000002,1,
                        0xAB000003,0x4000,
                        0xF6000000 | (((r+1)&1023)<<14) | (((b+1)&1023)<<2),
                        ((l&1023)<<14) | ((t&1023)<<2),
                        0xE7000000,0,0xAB000004,0x4000]
            assert list(out[4:20]) == expected
            assert all(x == 0xDEADBEEF for x in out[20:])
    return run

def test_boundary_grid(runner):
    for rect in itertools.product((-1,0,1,238,239,240,318,319,320), repeat=4):
        runner(rect)

def test_single_pixel_inclusive(runner):
    runner((5,7,5,7))

def test_fully_rejected(runner):
    for rect in [(-8,0,-1,3),(320,0,400,3),(0,-9,3,-1),(0,240,3,300),(9,1,2,5)]:
        runner(rect)

def test_negative_and_wrapped_coordinates(runner):
    for rect in [(-1024,-1023,1023,1024),(-1,-1,0,0),(1023,1023,1024,1024)]:
        runner(rect, (-2048,-2048,2048,2048))

def test_channels_and_alpha(runner):
    for i in range(256): runner((0,0,10,10), rgba=(i,255-i,i^0xA5,i))

def test_clip_inversion(runner):
    runner((-100,-100,100,100),(10,10,0,0))

def test_seeded_random(runner):
    rng=random.Random(0x8008A46C)
    for _ in range(10000):
        runner(tuple(rng.randrange(-2048,2049) for _ in range(4)),
               tuple(rng.randrange(-1024,1025) for _ in range(4)),
               tuple(rng.randrange(256) for _ in range(4)))
