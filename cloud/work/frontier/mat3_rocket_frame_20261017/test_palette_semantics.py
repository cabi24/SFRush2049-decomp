"""Focused host checks for the corrected N64 palette draft, not match credit."""
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
SOURCE = HERE / 'group' / 'rocket.c'


class PaletteSemantics(unittest.TestCase):
    def test_native_table_access_width(self):
        import sys
        sys.path.insert(0, str(ROOT))
        from tools.cloud.owndata import ImageData
        import struct
        data = ImageData.from_artifact(ROOT / 'asm/us/blob_data')
        self.assertIsNotNone(data)
        raw = data.read(0x8011B468, 19 * 4)
        self.assertIsNotNone(raw)
        indexes = struct.unpack('>19I', raw)
        self.assertTrue(all(0 < index < 256 for index in indexes))
        self.assertTrue(all(raw[4 * i] == 0 for i in range(19)))
        # No native table bytes are copied into this packet or test output.
        source = SOURCE.read_text()
        self.assertIn('extern s32 D_8011B468[];', source)
        self.assertNotIn('D_8011B468[D_8014978C * 4]', source)

    def test_layout_and_palette_branches(self):
        cc = shutil.which('cc')
        if not cc:
            self.skipTest('A host C compiler is needed for semantic checks')
        harness = r'''
#include <stddef.h>
#include <assert.h>
#include "SOURCE"

_Static_assert(sizeof(Car) == 0x3B8, "native Car stride");
_Static_assert(offsetof(Car, pal) == 0x344, "palette samples");
_Static_assert(sizeof(((Car *)0)->pal) == 8, "four palette samples");
_Static_assert(offsetof(Car, irColor) == 0x34C, "current IR color");
_Static_assert(offsetof(Car, prevIRColor) == 0x350, "previous IR color");
_Static_assert(offsetof(Car, fade) == 0x35A, "later fields stay in place");
_Static_assert(offsetof(Car, scale) == 0x374, "rocket scale stays in place");

u8 *D_8011AF90[19];
s32 D_8011B468[19];
s8 D_8014978C;

int main(void) {
    Color colors[32];
    Car car = {0};
    CarState model = {0};
    Color got;
    unsigned i, track, c;
    for (i = 0; i < 32; ++i) {
        colors[i].b[0] = 3 * i;
        colors[i].b[1] = 5 * i;
        colors[i].b[2] = 7 * i;
        colors[i].b[3] = 255;
    }
    for (track = 0; track < 19; ++track) {
        D_8011AF90[track] = (u8 *)colors;
        D_8011B468[track] = 1 + track % 4;
    }
    for (i = 0; i < 4; ++i) car.pal[i] = (i + 1) << 11;
    for (track = 0; track < 19; ++track) {
        unsigned avg[3];
        D_8014978C = track;
        for (c = 0; c < 3; ++c)
            avg[c] = (colors[1].b[c] + colors[2].b[c] + colors[3].b[c] + colors[4].b[c]) >> 2;
        model.dist = 0.0f;
        got = GetCarsIRColor(&car, &model);
        for (c = 0; c < 3; ++c) assert(got.b[c] == avg[c]);
        assert(got.b[3] == 255);
        model.dist = 30.0f;
        got = GetCarsIRColor(&car, &model);
        for (c = 0; c < 3; ++c)
            assert(got.b[c] == (u8)(avg[c] * 0.75f + colors[D_8011B468[track]].b[c] * 0.25f));
        assert(got.b[3] == 255);
        model.dist = 80.0f;
        got = GetCarsIRColor(&car, &model);
        assert(got.w == colors[D_8011B468[track]].w);
    }
    return 0;
}
'''.replace('SOURCE', str(SOURCE))
        with tempfile.TemporaryDirectory(prefix='mat3-palette-') as tmp:
            src, exe = Path(tmp) / 'check.c', Path(tmp) / 'check'
            src.write_text(harness)
            build = subprocess.run([cc, '-std=c11', '-O2', '-ffunction-sections',
                                    '-fdata-sections', str(src), '-Wl,--gc-sections',
                                    '-o', str(exe)], text=True, capture_output=True)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            run = subprocess.run([str(exe)], text=True, capture_output=True)
            self.assertEqual(run.returncode, 0, run.stdout + run.stderr)


if __name__ == '__main__':
    unittest.main()
