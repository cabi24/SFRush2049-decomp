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
    def test_identifier_retry_wrap_and_inactive_slots(self):
        self.run_c('800175B4','''
VoiceRecord D_80043EB8[8];u32 D_8004BE84;
int main(void) { int slot,mode,i;u32 expected,result,counter;for(slot=0;slot<8;slot++)for(mode=0;mode<4;mode++){memset(D_80043EB8,0,sizeof(D_80043EB8));counter=mode==0?100U:mode==1?0x7FFFFFFCU:mode==2?0xFFFFFFFFU:0U;D_8004BE84=counter;for(i=0;i<8;i++){D_80043EB8[i].identifier=(counter+i)&0x7FFFFFFFU;D_80043EB8[i].inactiveFC1=mode==3;}expected=mode==3?0U:mode==2?7U:(counter+8)&0x7FFFFFFFU;result=func_800175B4((u32)slot);assert(result==expected && D_80043EB8[slot].identifier==expected && D_8004BE84==((expected+1)&0x7FFFFFFFU));}return 0;}
''')
    def test_tagged_halfword_state(self):
        self.run_c('80018F20','''
VoiceRecord D_80043EB8[8];static VoiceRecord expected[8];static u32 result;
typedef char layout[sizeof(VoiceRecord)==4088 && offsetof(VoiceRecord,valueFC2)==4034 && offsetof(VoiceRecord,valueFEC)==4076?1:-1];
u32 func_80017644(u32 id){assert(id==1234);return result;}
int main(void){int row,tag;for(row=0;row<8;row++)for(tag=0;tag<2;tag++){memset(D_80043EB8,0xA5,sizeof(D_80043EB8));memcpy(expected,D_80043EB8,sizeof(expected));result=row|(tag?0x80000000U:0);if(tag){expected[row].valueFEC=0xCAFE;expected[row].flagsFEE|=0x20;}else expected[row].valueFC2=0xCAFE;func_80018F20(1234,0xCAFE);assert(memcmp(expected,D_80043EB8,sizeof(expected))==0);}result=0xFFFFFFFFU;func_80018F20(1234,0);assert(memcmp(expected,D_80043EB8,sizeof(expected))==0);return 0;}
''')
    def test_tagged_word_pair_state(self):
        self.run_c('800190AC','''
VoiceRecord D_80043EB8[8];static VoiceRecord expected[8];static u32 result;
typedef char layout[sizeof(VoiceRecord)==4088 && offsetof(VoiceRecord,first110)==272 && offsetof(VoiceRecord,firstFE4)==4068?1:-1];
u32 func_80017644(u32 id){assert(id==1234);return result;}
int main(void){int row,tag;for(row=0;row<8;row++)for(tag=0;tag<2;tag++){memset(D_80043EB8,0xA5,sizeof(D_80043EB8));memcpy(expected,D_80043EB8,sizeof(expected));result=row|(tag?0x80000000U:0);if(tag){expected[row].firstFE4=0xABCDEF01U;expected[row].secondFE8=0x98765432U;expected[row].flagsFEE|=0x10;}else{expected[row].first110=0xABCDEF01U;expected[row].second114=0x98765432U;}func_800190AC(1234,0xABCDEF01U,0x98765432U);assert(memcmp(expected,D_80043EB8,sizeof(expected))==0);}result=0xFFFFFFFFU;func_800190AC(1234,0,0);assert(memcmp(expected,D_80043EB8,sizeof(expected))==0);return 0;}
''')
    def test_packed_handle_getter(self):
        self.run_c('8001B968','''
VoiceState D_8004BEB8[32];static int result;
typedef char layout[sizeof(VoiceState)==416 && offsetof(VoiceState,valueC2)==194?1:-1];
int func_8001EDF4(u32 key){assert(key==0xDEADBEEFU);return result;}
int main(void){int row,flags,stale;result=-1;assert(func_8001B968(0xDEADBEEFU)==0);for(row=0;row<32;row++)for(flags=0;flags<4;flags++)for(stale=0;stale<2;stale++){result=0x123400|row;D_8004BEB8[row].identifier=(u32)result+stale;D_8004BEB8[row].flags=flags;D_8004BEB8[row].valueC2=0xCDEF;assert(func_8001B968(0xDEADBEEFU)==(stale||(flags&2)?0:0xCDEF));}return 0;}
''')
    def test_gated_five_argument_calls(self):
        self.run_c('80020494','''
u8 D_8002C630;static int phase,calls;static int first_on,second_on;
void func_80014594(void){assert(phase==0);phase=1;}
void func_800145DC(void){assert(phase==1);phase=2;}
void func_8001B9F8(u8 ch,u16 duration,u8 command,u8 arg4,unsigned int arg5){assert(phase==1&&ch==9&&duration==0xFEDC&&arg4==0&&arg5==0);assert(command==(calls==0&&first_on?21:22));calls++;}
int main(void){int enabled;for(enabled=0;enabled<2;enabled++)for(first_on=0;first_on<2;first_on++)for(second_on=0;second_on<2;second_on++){D_8002C630=enabled;phase=calls=0;func_80020494(9,0xFEDC,first_on?255:0,second_on?255:0);assert(phase==(enabled?2:0)&&calls==(enabled?first_on+second_on:0));}return 0;}
''')
    def test_interpolation_external_contract(self):
        self.run_c('8001A5D8','''
static u32 first_result;static u16 next;static u8 expected_note;static int calls;
u32 func_8001E50C(u8 note,u32 pitch){assert(note==expected_note&&pitch==0xABCDEF01U);calls++;return first_result;}
u16 func_8001E440(u16 whole){assert(whole==(u16)first_result);calls++;return next;}
int main(void){VoicePrefix state;u32 seeds[5]={0,1,0xFFFF,0xFFFFFFFFU,0x12345678U};u32 fractions[5]={0,1,32768,65534,65535};unsigned int i,j,k;unsigned long base,diff,expected;u32 key;state.pitch5C=0xABCDEF01U;for(i=0;i<5;i++)for(j=0;j<5;j++)for(k=0;k<3;k++){first_result=seeds[i];next=k==0?0:k==1?32768:65535;expected_note=(u8)(i*61);key=((u32)expected_note<<16)|fractions[j];base=first_result&65535U;diff=((unsigned long)next-base)&0xFFFFFFFFUL;expected=((base<<16)+diff*fractions[j])&0xFFFFFFFFUL;calls=0;assert(func_8001A5D8(&state,key)==(u32)expected && calls==(fractions[j]?2:1));}return 0;}
''',False)
if __name__=='__main__':unittest.main()
