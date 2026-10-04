"""C89 source behavior checks with test-only declared-callee contracts.

Host behavior is not N64 execution or strict equality. Pointer-bearing state uses
host pointer width here; a separate 32-bit syntax check verifies native offsets.
"""
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


class Command(C.Structure):
    _fields_ = [('word0', C.c_uint32), ('word1', C.c_uint32)]


class Voice(C.Structure):
    _pack_ = 1
    _fields_ = [('program', C.POINTER(Command)), ('current', C.POINTER(Command)),
        ('unknown08', C.c_ubyte*28), ('flags24', C.c_uint32),
        ('unknown28', C.c_ubyte*16), ('pan38', C.c_uint32), ('panDelta3C', C.c_uint32),
        ('unknown40', C.c_ubyte*14), ('keyGroup4E', C.c_uint16),
        ('unknown50', C.c_ubyte*16), ('id60', C.c_uint32), ('unknown64', C.c_ubyte*68),
        ('panTimeA8', C.c_uint32), ('panTargetAC', C.c_uint32),
        ('unknownB0', C.c_ubyte*208), ('variables180', C.c_int16*16)]


class PacketTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory(prefix='bt05-macro-five-host-')
        p = Path(cls.tmp.name)
        cc = shutil.which('cc')
        if not cc: raise RuntimeError('host C compiler required')
        source = (WORK/'nonmatch/func_80022324.c').read_text()
        prefix = source.split('#pragma pack(0)')[0] + '#pragma pack(0)\n'
        harness = prefix + '''
VoiceState D_8004BEB8[256];
u8 D_8004FA18;
s16 D_8004BE98[16];
static u32 seen[256], calls, seen_controller, seen_value, get_calls, set_calls;
static u16 controller_result;
static int grow_count;
int func_80021700(u32 id) {
    seen[calls++] = id;
    if (grow_count) { ++D_8004FA18; grow_count = 0; }
    return 0;
}
void func_8001E930(u32 *time) { *time *= 256U; }
u16 func_800215A8(VoiceState *voice, u8 controller) {
    (void)voice; ++get_calls; seen_controller = controller; return controller_result;
}
void func_8002165C(VoiceState *voice, u8 controller, s16 value) {
    (void)voice; ++set_calls; seen_controller = controller; seen_value = (u16)value;
}
void reset_calls(void) { calls = get_calls = set_calls = grow_count = 0; }
void grow_once(void) { grow_count = 1; }
u32 call_count(void) { return calls; }
u32 call_id(u32 index) { return seen[index]; }
u32 get_count(void) { return get_calls; }
u32 set_count(void) { return set_calls; }
u32 controller_seen(void) { return seen_controller; }
u32 value_seen(void) { return seen_value; }
void set_result(u32 result) { controller_result = result; }
'''
        (p/'harness.c').write_text(harness)
        sources = list(sorted((WORK/'nonmatch').glob('*.c')))
        subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra',
            '-shared','-fPIC',str(p/'harness.c'),*map(str,sources),'-o',str(p/'test.so')],
            check=True,capture_output=True,text=True)
        layout = prefix + '''
#include <stddef.h>
typedef char pointer_size[sizeof(void *) == 4 ? 1 : -1];
typedef char voice_size[sizeof(VoiceState) == 416 ? 1 : -1];
typedef char command_size[sizeof(MacroCommand) == 8 ? 1 : -1];
'''
        for field, offset in [('program',0),('current',4),('flags24',36),('pan38',56),
            ('panDelta3C',60),('keyGroup4E',78),('id60',96),('panTimeA8',168),
            ('panTargetAC',172),('variables180',384)]:
            layout += 'typedef char offset_%s[offsetof(VoiceState,%s)==%d?1:-1];\n' % (field,field,offset)
        (p/'layout.c').write_text(layout)
        subprocess.run([cc,'-m32','-std=c89','-pedantic-errors','-fsyntax-only',str(p/'layout.c')],
            check=True,capture_output=True,text=True)
        cls.lib=C.CDLL(str(p/'test.so'))
        cls.voices=(Voice*256).in_dll(cls.lib,'D_8004BEB8')
        cls.count=C.c_ubyte.in_dll(cls.lib,'D_8004FA18')
        cls.variables=(C.c_int16*16).in_dll(cls.lib,'D_8004BE98')
        for addr in ['80022324','800230B0']:
            f=getattr(cls.lib,'func_'+addr);f.argtypes=[C.POINTER(Voice),C.POINTER(Command)];f.restype=C.c_ubyte
        cls.lib.func_80023AD4.argtypes=[C.POINTER(Voice),C.c_ubyte,C.c_ubyte];cls.lib.func_80023AD4.restype=C.c_int16
        cls.lib.func_80023B50.argtypes=[C.POINTER(Voice),C.c_ubyte,C.c_ubyte,C.c_int16];cls.lib.func_80023B50.restype=None
        cls.lib.func_80023DB8.argtypes=[C.POINTER(Voice),C.POINTER(Command),C.c_ubyte];cls.lib.func_80023DB8.restype=C.c_ubyte
        cls.lib.call_id.restype=C.c_uint32

    @classmethod
    def tearDownClass(cls): cls.tmp.cleanup()

    def test_id_selection_and_live_count(self):
        for group in [0,1,65535]:
            for tag in [0,123,65535]:
                for offset in [0,1,255]:
                    v=Voice();v.keyGroup4E=group;c=Command((tag<<16)|(offset<<8),0)
                    base=(((group+offset)<<8)|(tag<<16))&0xFFFFFFFF
                    self.count.value=4;self.lib.reset_calls()
                    for i in range(5):self.voices[i].id60=(base|i) if i!=2 else ((base|i)^0x100)
                    self.lib.grow_once()
                    self.assertEqual(self.lib.func_80022324(C.byref(v),C.byref(c)),0)
                    self.assertEqual(self.lib.call_count(),4)
                    self.assertEqual([self.lib.call_id(i) for i in range(4)],[base,base|1,base|3,base|4])
        self.count.value=0;self.lib.reset_calls();self.lib.func_80022324(C.byref(v),C.byref(c));self.assertEqual(self.lib.call_count(),0)

    def test_pan_signed_offset_unsigned_division(self):
        for time in [0,1,2,255,65535]:
            for pan in [0,127,255]:
                for offset in [-128,-1,0,1,127]:
                    v=Voice();v.flags24=0xA5;c=Command((time<<16)|(pan<<8),offset&255)
                    self.assertEqual(self.lib.func_800230B0(C.byref(v),C.byref(c)),0)
                    delta=(offset<<16)&0xFFFFFFFF
                    self.assertEqual(v.flags24,0x100A5);self.assertEqual(v.panTimeA8,time*256)
                    self.assertEqual(v.pan38,pan<<16);self.assertEqual(v.panTargetAC,((pan<<16)+delta)&0xFFFFFFFF)
                    self.assertEqual(v.panDelta3C,delta//time if time else delta)

    def test_local_global_variable_and_controller_routes(self):
        v=Voice()
        for index in range(256):
            for value in [-32768,-1,0,32767]:
                self.lib.reset_calls()
                self.lib.func_80023B50(C.byref(v),0,index,value)
                self.assertEqual(self.lib.func_80023AD4(C.byref(v),0,index),value)
                self.assertEqual(self.lib.get_count(),0);self.assertEqual(self.lib.set_count(),0)
                expected=v.variables180[index&31] if index&31<16 else self.variables[(index&31)-16]
                self.assertEqual(expected,value)
                self.lib.func_80023B50(C.byref(v),255,index,value)
                self.assertEqual(self.lib.set_count(),1);self.assertEqual(self.lib.controller_seen(),index)
                self.assertEqual(self.lib.value_seen(),value&65535)
            self.lib.set_result(0xFFFF)
            self.assertEqual(self.lib.func_80023AD4(C.byref(v),1,index),-1)
            self.assertEqual(self.lib.controller_seen(),index)

    def test_branch_valid_comparison_domain(self):
        v=Voice();program=(Command*8)();v.program=program
        for left in [-32768,-1,0,1,32767]:
            for right in [-32768,-1,0,1,32767]:
                v.variables180[0]=left;self.variables[0]=right
                for comparison in [0,1]:
                    for invert in [0,1,255]:
                        v.current=C.cast(C.addressof(program)+8,C.POINTER(Command))
                        c=Command(0,(3<<16)|(invert<<8)|16)
                        self.assertEqual(self.lib.func_80023DB8(C.byref(v),C.byref(c),comparison),0)
                        result=left==right if comparison==0 else left<right
                        if invert:result=not result
                        self.assertEqual(C.addressof(v.current.contents),C.addressof(program)+(24 if result else 8))

    def test_receipt_binding_and_no_verified_claim(self):
        receipt=json.loads((WORK/'verification.json').read_text())
        self.assertEqual(receipt['functions'],5);self.assertEqual(receipt['native_bytes'],888)
        self.assertEqual(receipt['matching_functions'],0);self.assertEqual(receipt['verified_body_bytes'],0)
        for row in receipt['results']:
            self.assertEqual(hashlib.sha256((ROOT/row['source_path']).read_bytes()).hexdigest(),row['source_sha256'])
            self.assertFalse(row['strict_match']);self.assertFalse(row['full_relocated_equality'])


if __name__ == '__main__': unittest.main()
