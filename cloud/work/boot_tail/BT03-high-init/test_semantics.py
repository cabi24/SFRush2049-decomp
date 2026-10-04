"""Bounded C89 source behavior; native pointer layout is proved by IDO replay."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
class Semantics(unittest.TestCase):
    def run_c(self, address, body, match=True):
        source = ROOT/'cloud/matches/boot_tail'/('func_'+address+'.c') if match else WORK/'nonmatch'/('func_'+address+'.c')
        with tempfile.TemporaryDirectory() as t:
            p = Path(t)
            (p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],check=True,capture_output=True)
            subprocess.run([str(p/'test')],check=True,capture_output=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
    def test_sample_dispatch_and_live_count(self):
        self.run_c('8001C508', '''
u8 D_8004FA18; SampleBuffer D_8004FA50[32];
static short samples[32]; static int calls, shrink;
void func_80014C60(short *p, u32 n) { assert(p == samples + n); calls++; if(shrink) D_8004FA18=1; }
int main(void) { int i,n,expected; for(n=0;n<=32;n++) { D_8004FA18=n; expected=0; calls=0; for(i=0;i<32;i++) { D_8004FA50[i].mode=i%3; D_8004FA50[i].buffer=samples+i; D_8004FA50[i].samples=i; if(i<n && i%3==1) expected++; } func_8001C508(); assert(calls==expected); } shrink=1; D_8004FA18=32; D_8004FA50[0].mode=1; calls=0; func_8001C508(); assert(calls==1); return 0; }
''')
    def test_channel_reset(self):
        self.run_c('8001EE34', '''
u8 D_8004FA18; ChannelValue D_800504C8[32]; u8 D_80050548[256]; u16 D_80050A48;
typedef char check_size[sizeof(ChannelValue)==4?1:-1];
int main(void) { int n,i; for(n=0;n<=32;n++) { memset(D_800504C8,0x5A,sizeof(D_800504C8)); memset(D_80050548,0,sizeof(D_80050548)); D_8004FA18=n; D_80050A48=0; func_8001EE34(); for(i=0;i<32;i++) { assert(D_800504C8[i].unknown==0x5A5A); assert(D_800504C8[i].value==(i<n?0:0x5A5A)); } for(i=0;i<256;i++) assert(D_80050548[i]==255); assert(D_80050A48==65535); } return 0; }
''')
    def test_nonempty_link_initialization(self):
        self.run_c('8001F7EC', '''
u8 D_8004FA18; ChannelLink D_80050440[32]; u8 D_800504C0,D_800504C1;
typedef char check_size[sizeof(ChannelLink)==4?1:-1];
int main(void) { int n,i; for(n=1;n<=32;n++) { memset(D_80050440,0x5A,sizeof(D_80050440)); D_8004FA18=n; func_8001F7EC(); for(i=0;i<n;i++) { assert(D_80050440[i].previous==(i?i-1:255)); assert(D_80050440[i].next==(i==n-1?255:i+1)); assert(D_80050440[i].flags==1); } for(i=n;i<32;i++) assert(D_80050440[i].flags==0x5A5A); assert(D_800504C0==0 && D_800504C1==n-1); } return 0; }
''')
    def test_state_reset_call_order(self):
        self.run_c('8001F9D0', '''
static int phase; static u32 bits; static VoiceState expected;
typedef char check_size[sizeof(VoiceState)==416?1:-1];
typedef char check_fields[offsetof(VoiceState,flags)==36 && offsetof(VoiceState,value28)==40 && offsetof(VoiceState,identifier)==96 && offsetof(VoiceState,valueBD)==189?1:-1];
void func_8001EB10(VoiceState *s) { assert(phase==0); phase=1; s->flags=bits; }
void func_8001F6EC(VoiceState *s) { assert(phase==1); phase=2; assert(memcmp(s,&expected,sizeof(expected))==0); }
int main(void) { VoiceState s; unsigned int i; for(i=0;i<256;i++) { bits=0xA5A50000U|i; memset(&s,0x5A,sizeof(s)); expected=s; expected.flags=bits&~3U; expected.value28=0; phase=0; func_8001F9D0(&s); assert(phase==2); } return 0; }
''')
    def test_index_dispatch(self):
        self.run_c('8001F954', '''
VoiceState D_8004BEB8[32]; static int selected,calls,active;
u8 func_8001467C(int i) { assert(i==selected); calls++; return active; }
void func_80014AF0(int i) { assert(i==selected); calls++; }
void func_8001F6EC(VoiceState *s) { assert(s==D_8004BEB8+selected); assert(s->identifier==(u32)selected); calls++; s->valueBD=255; }
int main(void) { int i; calls=0; func_8001F954(-1); assert(calls==0); for(i=0;i<32;i++) for(active=0;active<2;active++) { selected=i; calls=0; func_8001F954(i); assert(calls==2+active && D_8004BEB8[i].valueBD==0); } return 0; }
''',False)
    def test_callback_record_copy(self):
        self.run_c('80020598', '''
CallbackWords D_80038000;
int main(void) { CallbackWords s; unsigned int i; for(i=0;i<8;i++) s.words[i]=0x80000000U+i; func_80020598(&s); assert(memcmp(&s,&D_80038000,sizeof(s))==0); func_80020598(&D_80038000); assert(memcmp(&s,&D_80038000,sizeof(s))==0); return 0; }
''',False)
if __name__=='__main__': unittest.main()
