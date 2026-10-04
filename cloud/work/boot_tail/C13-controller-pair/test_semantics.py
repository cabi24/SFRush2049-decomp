"""Source-bound C89 tests for registered controller rows and live call order."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
WORK = Path(__file__).resolve().parent


class Semantics(unittest.TestCase):
    def run_c(self, address, body):
        source = WORK / 'nonmatch' / ('func_' + address + '.c')
        with tempfile.TemporaryDirectory(prefix='c13-controller-tests-') as tmp:
            p = Path(tmp)
            (p / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True)
            subprocess.run([str(p / 'test')], check=True, capture_output=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))

    def test_clear_whole_row_then_ordered_controller_defaults(self):
        self.run_c('80020820', r'''
u8 D_80050D00[8][16][134];u8 D_80055000[32][134];
static u8 expected_regular[8][16][134],expected_external[32][134];
static u8 commands[10]={7,10,128,129,64,65,91,131,132,133};
static u8 values[10]={127,64,64,0,0,0,0,0,64,0};
static u8 wanted_channel,wanted_set;static int calls;
void func_80020610(u8 command,u8 channel,u8 set,u8 value) {
    u8 *row;int i;
    assert(calls<10&&command==commands[calls]&&value==values[calls]);
    assert(channel==wanted_channel&&set==wanted_set);
    row=set==255?D_80055000[channel]:D_80050D00[set][channel];
    if(calls==0){for(i=0;i<134;i++)assert(row[i]==0);row[0]=0xC5;}
    else assert(row[0]==0xC5);
    row[command]=value&127;calls++;
}
void func_80020F4C(u8 channel,u8 set,u8 value) {assert(calls++==10&&channel==wanted_channel&&set==wanted_set&&value==255);}
void func_80020FDC(u8 channel,u8 set,u8 value) {assert(calls++==11&&channel==wanted_channel&&set==wanted_set&&value==0);}
int main(void) {
    int group,channel,pattern,i,cases;u8 *row;
    cases=0;
    for(group=0;group<9;group++)for(channel=0;channel<(group==8?32:16);channel++)for(pattern=0;pattern<2;pattern++){
        wanted_set=(u8)(group==8?255:group);wanted_channel=(u8)channel;calls=0;
        memset(D_80050D00,pattern?0xA5:0xFF,sizeof(D_80050D00));memset(D_80055000,pattern?0xA5:0xFF,sizeof(D_80055000));
        memcpy(expected_regular,D_80050D00,sizeof(D_80050D00));memcpy(expected_external,D_80055000,sizeof(D_80055000));
        row=group==8?expected_external[channel]:expected_regular[group][channel];memset(row,0,134);row[0]=0xC5;
        for(i=0;i<10;i++)row[commands[i]]=values[i];
        func_80020820(wanted_channel,wanted_set);
        assert(calls==12&&memcmp(D_80050D00,expected_regular,sizeof(D_80050D00))==0&&memcmp(D_80055000,expected_external,sizeof(D_80055000))==0);cases++;
    }
    assert(cases==320);return 0;
}
''')

    def test_controller_copy_widths_all_valid_commands_and_self_alias(self):
        self.run_c('80020DA8', r'''
u8 D_80055000[32][134];static u8 expected[32][134];
typedef char layout[offsetof(VoicePrefix,identifier60)==96?1:-1];
int main(void) {
    VoicePrefix dst,src,before_dst,before_src;int d,s,c,p,i,j,index,cases;
    cases=0;
    for(d=0;d<32;d++)for(s=0;s<32;s++)for(c=0;c<134;c++)for(p=0;p<2;p++){
        for(i=0;i<32;i++)for(j=0;j<134;j++)D_80055000[i][j]=(u8)(i*11+j*7+p*127);
        memset(&dst,0xA5,sizeof(dst));memset(&src,0x5A,sizeof(src));dst.identifier60=0xABCD1200U+d;src.identifier60=0x12345600U+s;
        before_dst=dst;before_src=src;memcpy(expected,D_80055000,sizeof(expected));
        if(c<64){index=c&31;expected[d][index]=expected[s][index];expected[d][index+32]=expected[s][index+32];}
        else if(c==128||c==129||c==132||c==133){index=c&254;expected[d][index]=expected[s][index];expected[d][index+1]=expected[s][index+1];}
        else expected[d][c]=expected[s][c];
        func_80020DA8((u8)c,&dst,&src);
        assert(memcmp(D_80055000,expected,sizeof(expected))==0&&memcmp(&dst,&before_dst,sizeof(dst))==0&&memcmp(&src,&before_src,sizeof(src))==0);cases++;
    }
    assert(cases==274432);
    before_dst=dst;memcpy(expected,D_80055000,sizeof(expected));func_80020DA8(132,&dst,&dst);
    assert(memcmp(&dst,&before_dst,sizeof(dst))==0&&memcmp(D_80055000,expected,sizeof(expected))==0);return 0;
}
''')


if __name__ == '__main__':
    unittest.main()
