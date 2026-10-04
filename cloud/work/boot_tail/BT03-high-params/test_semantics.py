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
    def test_walk_live_identifiers(self):
        self.run_c('80018E6C','''
VoiceRecord D_80043EB8[8]; static unsigned int calls;
typedef char layout[sizeof(VoiceRecord)==4088 && offsetof(VoiceRecord,identifier)==0?1:-1];
void func_80018D40(u32 id) { assert(id==calls+100); calls++; if(calls<8) D_80043EB8[calls].identifier=calls+100; }
int main(void) { memset(D_80043EB8,0xA5,sizeof(D_80043EB8)); D_80043EB8[0].identifier=100; calls=0; func_80018E6C(); assert(calls==8); return 0; }
''')
    def test_tagged_state_mutation(self):
        self.run_c('80018FEC','''
VoiceRecord D_80043EB8[8]; static VoiceRecord expected[8]; static u32 result;
typedef char layout[sizeof(VoiceRecord)==4088 && offsetof(VoiceRecord,activeFC0)==4032 && offsetof(VoiceRecord,flagsFEE)==4078?1:-1];
u32 func_80017644(u32 id) { assert(id==1234); return result; }
int main(void) { int row,tag; for(row=0;row<8;row++) for(tag=0;tag<2;tag++) { memset(D_80043EB8,0xA5,sizeof(D_80043EB8)); memcpy(expected,D_80043EB8,sizeof(expected)); result=(u32)row|(tag?0x80000000U:0); if(tag) expected[row].flagsFEE &= ~8; else expected[row].activeFC0=1; func_80018FEC(1234); assert(memcmp(expected,D_80043EB8,sizeof(expected))==0); } result=0xFFFFFFFFU; func_80018FEC(1234); assert(memcmp(expected,D_80043EB8,sizeof(expected))==0); return 0; }
''')
    def test_rate_scaling_low_word(self):
        self.run_c('8001B154','''
s32 D_8004F808,D_8004F800; u32 D_8004BE90; static int calls;
void func_8001A658(void) { calls++; }
int main(void) { s32 nums[8]={0,1,-1,60,2147483647,(-2147483647-1),100000,-100000}; s32 dens[6]={1,-1,32000,44100,48000,-7}; int i,j; unsigned long low,out; long signed_value,q; for(i=0;i<8;i++) for(j=0;j<6;j++) { low=((unsigned long)(u32)nums[i]*32000UL)&0xFFFFFFFFUL; signed_value=(low&0x80000000UL)?(long)low-4294967296L:(long)low; if(signed_value==(-2147483647L-1)&&dens[j]==-1) continue; D_8004F808=nums[i];D_8004F800=dens[j];D_8004BE90=0xABCD1234U;calls=0;func_8001B154(); if(nums[i]==0) { assert(calls==0 && D_8004BE90==0xABCD1234U); } else { q=signed_value/dens[j];out=((unsigned long)q*8UL)&0xFFFFFFFFUL; assert(calls==1 && D_8004BE90==(u32)out); } } return 0; }
''')
    def test_five_real_controller_calls(self):
        self.run_c('8001B744','''
struct VoiceState { int unused; }; static struct VoiceState a,b; static int calls;
void func_80020DA8(u8 c,struct VoiceState *d,struct VoiceState *s) { static const u8 ids[5]={7,10,91,128,132};assert(c==ids[calls++] && d==&a && s==&b); }
int main(void) { func_8001B744(&a,&b);assert(calls==5);return 0; }
''')
    def test_halfword_exchange(self):
        self.run_c('8002021C','''
s16 D_8004BE98[16]; static int phase; static int selected;
void func_80014594(void) { assert(phase==0); phase=1;D_8004BE98[selected]=-12345; }
void func_800145DC(void) { assert(phase==1); phase=2;assert(D_8004BE98[selected]==23456); }
int main(void) { unsigned int i; for(i=0;i<256;i++) { selected=i&15;phase=0;assert(func_8002021C((u8)i,23456)==-12345);assert(phase==2); } return 0; }
''',False)
    def test_key_gate_and_result(self):
        self.run_c('80019ED0','''
static u8 enabled; static u32 result; static int calls; static unsigned int expected_key;
u8 func_80021028(u8 channel,u8 set) { assert(channel==3&&set==4); calls++;return enabled; }
u32 func_80019C8C(u8 key,u8 channel,u8 set) { assert(key==expected_key&&channel==3&&set==4);calls++;return result; }
int main(void) { unsigned int key;calls=0;assert(func_80019ED0(0,255,4)==0xFFFFFFFFU&&calls==0);for(key=0;key<256;key++) { expected_key=key&127;enabled=0;calls=0;assert(func_80019ED0((u8)key,3,4)==0xFFFFFFFFU&&calls==1);enabled=1;result=0xFFFFFFFEU;calls=0;assert(func_80019ED0((u8)key,3,4)==result&&calls==2);result=0xFFFFFFFFU;calls=0;assert(func_80019ED0((u8)key,3,4)==result&&calls==2); } return 0; }
''',False)
if __name__=='__main__': unittest.main()
