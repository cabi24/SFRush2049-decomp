"""C89 ASan/UBSan checks with synthetic external contracts, not native callees."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
WORK = Path(__file__).resolve().parent

class Semantics(unittest.TestCase):
    def run_c(self, address, body):
        source = WORK / 'nonmatch' / ('func_' + address + '.c')
        with tempfile.TemporaryDirectory() as t:
            p = Path(t)
            (p / 'test.c').write_text('#include <assert.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True)
            subprocess.run([str(p / 'test')], check=True, capture_output=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))

    def test_unsigned_scaling_and_curve(self):
        self.run_c('800232A4', r'''
static u32 expected_volume; static u16 expected_curve; static int calls;
u32 func_8002321C(u32 volume, u16 curve) { assert(volume == expected_volume && curve == expected_curve); calls++; return volume ^ 0xA5A5A5A5U; }
int main(void) {
    static const u32 volumes[] = {0,1,0x7F0000U,0xFFFFFFFFU,0x80000000U,0xFFFF0000U,0x12345678U};
    static const unsigned int bytes[] = {0,1,127,128,255};
    MacroState state; MacroCommand command; unsigned int i,j,k,mode; unsigned long product,sum; u32 original;
    typedef char width[sizeof(u32)==4 && sizeof(unsigned long)>=8 ? 1 : -1];
    (void)sizeof(width);
    for(i=0;i<7;i++) for(j=0;j<5;j++) for(k=0;k<5;k++) for(mode=0;mode<2;mode++) {
        memset(&state,0,sizeof(state)); original=volumes[(i+1)%7]; state.volume30=volumes[i]; state.originalVolume34=original;
        command.word[0]=(0xA5U<<24)|(bytes[k]<<16)|(bytes[j]<<8)|0x42U; command.word[1]=(mode<<8)|0x5BU;
        product=((unsigned long)(mode?original:volumes[i])*bytes[j])&0xFFFFFFFFUL;
        sum=((product>>7)+((unsigned long)bytes[k]<<16))&0xFFFFFFFFUL;
        expected_volume=sum>0x7F0000UL?0x7F0000U:(u32)sum; expected_curve=0xA55B; calls=0;
        assert(func_800232A4(&state,&command)==0); assert(calls==1); assert(state.volume30==(expected_volume^0xA5A5A5A5U)); assert(state.originalVolume34==original);
        assert(command.word[0]==((0xA5U<<24)|(bytes[k]<<16)|(bytes[j]<<8)|0x42U)); assert(command.word[1]==((mode<<8)|0x5BU));
    }
    return 0;
}
''')

    def test_random_note_bounds_and_protocol(self):
        self.run_c('80023564', r'''
static u16 draws[2]; static int draw_count,call_count; static u32 expected_word; static MacroState *expected_state;
u16 func_8001E790(void) { assert(draw_count<2); return draws[draw_count++]; }
u8 func_800225FC(MacroState *state, MacroCommand *command) { assert(state==expected_state && command->word[0]==expected_word && command->word[1]==0); call_count++; return 173; }
static int halfword(int value) { unsigned int low=(unsigned int)value&65535U; return low>=32768U?(int)low-65536:(int)low; }
static int clamp(int value) { return value<0?0:value>127?127:value; }
int main(void) {
    static const unsigned int notes[]={0,127,255,32767,32768,65535};
    static const unsigned int bounds[]={0,1,127,255};
    static const unsigned int randoms[]={0,1,200,201,65535};
    MacroState state; MacroCommand command; unsigned int i,j,k,relative,random_detune,r; int low,high,swap,divisor,note,detune; unsigned int cases=0,excluded=0;
    for(i=0;i<6;i++) for(j=0;j<4;j++) for(k=0;k<4;k++) for(relative=0;relative<2;relative++) for(random_detune=0;random_detune<2;random_detune++) for(r=0;r<5;r++) {
        if(relative) { low=clamp(halfword((int)notes[i]-(int)bounds[j])); high=clamp(halfword((int)notes[i]+(int)bounds[k])); }
        else { low=(int)bounds[j]; high=(int)bounds[k]; if(low>high) { swap=low;low=high;high=swap; } }
        divisor=high-low+1; if(divisor==0) { excluded++;continue; }
        memset(&state,0,sizeof(state)); state.note50=(u16)notes[i]; expected_state=&state;
        command.word[0]=(bounds[k]<<24)|(0xE7U<<16)|(bounds[j]<<8)|0x31U;command.word[1]=(relative<<8)|random_detune;
        draws[0]=(u16)randoms[r];draws[1]=(u16)randoms[(r+1)%5];draw_count=0;call_count=0;
        detune=random_detune?((int)draws[0]%201)-100:0xE7;
        note=low+((int)draws[random_detune?1:0]%divisor);
        expected_word=((u32)((unsigned int)detune&255U)<<16)|0x19U|((u32)note<<8);
        assert(func_80023564(&state,&command)==0); assert(call_count==1);assert(draw_count==(random_detune?2:1));assert(state.note50==notes[i]); cases++;
    }
    assert(cases>1000);(void)excluded;return 0;
}
''')

if __name__ == '__main__':
    unittest.main()
