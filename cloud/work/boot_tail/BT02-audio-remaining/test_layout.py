#!/usr/bin/env python3
"""Check consumed C89 aggregate offsets with pinned native-width IDO."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
P = Path(__file__).resolve().parent
ROOT = P.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
source = P / 'nonmatch/func_80014198.c'
types = source.read_text().split('extern unsigned char D_800382CD;')[0]
# The flag is volatile in the final source; retain only the declared view.
types = source.read_text().split('extern volatile unsigned char D_800382CD;')[0]
checks = r'''
#define OFF(type, member) ((unsigned int)&(((type *)0)->member))
typedef char word_is_4[sizeof(unsigned int) == 4 ? 1 : -1];
typedef char short_is_2[sizeof(unsigned short) == 2 ? 1 : -1];
typedef char pointer_is_4[sizeof(void *) == 4 ? 1 : -1];
typedef char frequency_offset[OFF(AudioConfiguration, frequency) == 0 ? 1 : -1];
typedef char mode_offset[OFF(AudioConfiguration, mode) == 4 ? 1 : -1];
typedef char bits_offset[OFF(AudioConfiguration, sample_bits) == 5 ? 1 : -1];
typedef char reserved_offset[OFF(AudioConfiguration, unknown06) == 6 ? 1 : -1];
typedef char text_offset[OFF(AudioConfiguration, text) == 0x106 ? 1 : -1];
int layout_check(void) { return 0; }
'''
with tempfile.TemporaryDirectory(prefix='bt02-audio-layout-') as directory:
    path = Path(directory) / 'layout.c'
    path.write_text(types + checks)
    score.compile_single(path, score.DEFAULT_FLAGS, Path(directory) / 'layout.o')
print(json.dumps({'result': 'PASS', 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                  'native_widths': {'word': 4, 'short': 2, 'pointer': 4},
                  'field_offsets': {'frequency': 0, 'mode': 4, 'sample_bits': 5, 'unknown06': 6, 'text': 262}}, indent=2))
