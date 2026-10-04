"""C89 packed-state behavior, live cache ownership and honest score checks."""
import ctypes as C
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]


class Voice(C.Structure):
    _pack_ = 1
    _fields_ = [('unknown00',C.c_ubyte*36),('flags24',C.c_uint32),
                ('unknown28',C.c_ubyte*34),('channel4A',C.c_ubyte),('set4B',C.c_ubyte),
                ('unknown4C',C.c_ubyte*20),('id60',C.c_uint32),('unknown64',C.c_ubyte*268),
                ('lfo170',C.c_int16),('unknown172',C.c_ubyte*10),('lfo17C',C.c_int16),
                ('unknown17E',C.c_ubyte*34)]


class PacketTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cc = shutil.which('cc')
        if not cc: raise RuntimeError('host C compiler required')
        cls.tmp = tempfile.TemporaryDirectory(prefix='c13-tail-host-')
        p = Path(cls.tmp.name)
        matched = ROOT/'cloud/matches/boot_tail/func_80021700.c'
        prefix = matched.read_text().split('extern VoiceState')[0]
        harness = prefix + '''
#include <stddef.h>
VoiceState D_8004BEB8[256];
u8 D_80056160[8][16];
u8 D_800561E0[32];
static unsigned int get_calls, set_calls, seen_ctrl, seen_channel, seen_set, seen_value;
static unsigned int translation_calls, translated_ctrl;
static u8 translation_result;
u8 test_translate(u8 controller) { ++translation_calls; translated_ctrl=controller; return translation_result; }
void set_translation(unsigned int value) { translation_result=value; }
unsigned int count_translate(void) { return translation_calls; }
unsigned int translation_input(void) { return translated_ctrl; }
u16 func_80020A04(u8 c, u8 ch, u8 set) {
    ++get_calls; seen_ctrl=c; seen_channel=ch; seen_set=set; return 0xABCD;
}
void func_800206AC(u8 c, u8 ch, u8 set, u16 value) {
    ++set_calls; seen_ctrl=c; seen_channel=ch; seen_set=set; seen_value=value;
}
void reset_calls(void) { get_calls=0; set_calls=0; translation_calls=0; }
unsigned int count_get(void) { return get_calls; }
unsigned int count_set(void) { return set_calls; }
unsigned int argument(unsigned int n) {
    if (n==0) return seen_ctrl;
    if (n==1) return seen_channel;
    if (n==2) return seen_set;
    return seen_value;
}
typedef char stride_check[sizeof(VoiceState)==416 ? 1 : -1];
typedef char flags_check[offsetof(VoiceState,flags24)==36 ? 1 : -1];
typedef char id_check[offsetof(VoiceState,id60)==96 ? 1 : -1];
typedef char lfo_check[offsetof(VoiceState,lfo170)==368 ? 1 : -1];
typedef char lfo2_check[offsetof(VoiceState,lfo17C)==380 ? 1 : -1];
'''
        (p/'h.c').write_text(harness)
        sources=[str(matched)]
        for source in sorted((WORK/'nonmatch').glob('*.c')):
            if source.stem in ['func_800215A8','func_8002165C']:
                # Test-only external contract: do not assume the unproved table mapping.
                copy=p/source.name
                copy.write_text('#define func_80021548 test_translate\n'+source.read_text())
                sources.append(str(copy))
            else:
                sources.append(str(source))
        subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-shared','-fPIC',
                        str(p/'h.c'),*sources,'-o',str(p/'test.so')],check=True,capture_output=True,text=True)
        cls.lib=C.CDLL(str(p/'test.so'))
        cls.lib.func_80021548.argtypes=[C.c_ubyte];cls.lib.func_80021548.restype=C.c_ubyte
        cls.lib.func_800215A8.argtypes=[C.POINTER(Voice),C.c_ubyte];cls.lib.func_800215A8.restype=C.c_uint16
        cls.lib.func_8002165C.argtypes=[C.POINTER(Voice),C.c_ubyte,C.c_int16];cls.lib.func_8002165C.restype=None
        cls.lib.func_80021700.argtypes=[C.c_uint32];cls.lib.func_80021700.restype=C.c_int32
        for name in ['80021764','800217E4','80021844']:
            getattr(cls.lib,'func_'+name).argtypes=[C.POINTER(Voice)]
            getattr(cls.lib,'func_'+name).restype=C.c_int32 if name=='80021764' else None
        cls.voices=(Voice*256).in_dll(cls.lib,'D_8004BEB8')
        cls.midi=(C.c_ubyte*128).in_dll(cls.lib,'D_80056160')
        cls.effects=(C.c_ubyte*32).in_dll(cls.lib,'D_800561E0')

    @classmethod
    def tearDownClass(cls): cls.tmp.cleanup()

    def test_reference_lead_translation_candidate(self):
        mapping={128:128,129:130,130:160,131:161,132:131,133:132}
        for c in range(256): self.assertEqual(self.lib.func_80021548(c),mapping.get(c,c))

    def test_lfo_read_and_controller_call(self):
        v=Voice();v.set4B=7
        for channel in [0,255]:
            v.channel4A=channel
            for value in [-32768,-8192,-4096,-1,0,1,32767]:
                v.lfo170=value;v.lfo17C=-value if value!=-32768 else 32767
                for c in [0,130,255]:
                    for translated in [0,160,161,255]:
                        self.lib.reset_calls();self.lib.set_translation(translated)
                        result=self.lib.func_800215A8(C.byref(v),c)
                        special=translated in [160,161]
                        expected=((v.lfo170 if translated==160 else v.lfo17C)*2+8192)&65535 if special else (0xABCD if channel!=255 else 0)
                        self.assertEqual(result,expected)
                        self.assertEqual(self.lib.count_translate(),1)
                        self.assertEqual(self.lib.translation_input(),c)
                        self.assertEqual(self.lib.count_get(),int(not special and channel!=255))
                        if not special and channel!=255:
                            self.assertEqual([self.lib.argument(i) for i in range(3)],[c,channel,7])

    def test_clamped_controller_write(self):
        v=Voice();v.set4B=7
        for channel in [0,255]:
            v.channel4A=channel
            for value in [-32768,-1,0,123,16383,16384,32767]:
                for c in [0,130,255]:
                    for translated in [0,160,161,255]:
                        self.lib.reset_calls();self.lib.set_translation(translated)
                        self.lib.func_8002165C(C.byref(v),c,value)
                        expected=translated not in [160,161] and channel!=255
                        self.assertEqual(self.lib.count_set(),int(expected))
                        self.assertEqual(self.lib.count_translate(),1)
                        self.assertEqual(self.lib.translation_input(),c)
                        if expected:
                            self.assertEqual([self.lib.argument(i) for i in range(4)],
                                             [c,channel,7,max(0,min(16383,value))])

    def test_identity_checked_flag_update(self):
        self.assertEqual(C.sizeof(Voice),416)
        for index in [0,1,31,255]:
            for flags in [0,0xFFFFFFFF,0x80000000]:
                C.memset(self.voices,0xA5,C.sizeof(self.voices))
                handle=0x12340000|index
                self.voices[index].id60=handle; self.voices[index].flags24=flags
                before=bytes(self.voices)
                self.assertEqual(self.lib.func_80021700(handle),0)
                self.assertEqual(self.voices[index].flags24,flags|8)
                expected=bytearray(before)
                expected[index*416+36:index*416+40]=bytes(C.c_uint32(flags|8))
                self.assertEqual(bytes(self.voices),bytes(expected))
                before=bytes(self.voices)
                self.assertEqual(self.lib.func_80021700(handle^0x100),-1)
                self.assertEqual(self.lib.func_80021700(0xFFFFFFFF),-1)
                self.assertEqual(bytes(self.voices),before)

    def test_cache_ownership_protocol(self):
        for index in [0,1,31]:
            for set_no in [0,7,255]:
                for channel in [0,15]:
                    v=Voice();v.id60=0x12340000|index;v.channel4A=channel;v.set4B=set_no
                    array=self.effects if set_no==255 else self.midi
                    slot=index if set_no==255 else set_no*16+channel
                    for owner in [255,index,index^1]:
                        C.memset(self.midi,255,128);C.memset(self.effects,255,32);array[slot]=owner
                        self.assertEqual(self.lib.func_80021764(C.byref(v)),int(owner==index))
                        before_m,before_f=bytearray(self.midi),bytearray(self.effects)
                        self.lib.func_800217E4(C.byref(v))
                        (before_f if set_no==255 else before_m)[slot]=index
                        self.assertEqual(bytes(self.midi),bytes(before_m));self.assertEqual(bytes(self.effects),bytes(before_f))
                        self.assertEqual(self.lib.func_80021764(C.byref(v)),1)
                        self.lib.func_80021844(C.byref(v));self.assertEqual(array[slot],255)
                        array[slot]=index^1; self.lib.func_80021844(C.byref(v));self.assertEqual(array[slot],index^1)
        for invalid_id,channel in [(0xFFFFFFFF,0),(0x12340000,255)]:
            v=Voice();v.id60=invalid_id;v.channel4A=channel;v.set4B=0
            before=(bytes(self.midi),bytes(self.effects))
            self.assertEqual(self.lib.func_80021764(C.byref(v)),0)
            self.lib.func_800217E4(C.byref(v));self.lib.func_80021844(C.byref(v))
            self.assertEqual((bytes(self.midi),bytes(self.effects)),before)

    def test_honest_receipt_and_source_hashes(self):
        r=json.loads((WORK/'scores.json').read_text());matches=[x for x in r['results'] if x['strict_match']]
        self.assertEqual(len(r['results']),32);self.assertEqual(len(matches),1)
        self.assertEqual(matches[0]['name'],'func_80021700');self.assertEqual(matches[0]['target_bytes'],100)
        self.assertTrue(matches[0]['full_relocated_equality'])
        for x in r['results']:
            self.assertEqual(hashlib.sha256((ROOT/x['source_path']).read_bytes()).hexdigest(),x['source_sha256'])
            if x['name']=='func_80021548':
                self.assertFalse(x['strict_match']);self.assertEqual(len(x['unverified']),2)


if __name__ == '__main__': unittest.main()
