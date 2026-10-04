"""C89 sanitizer tests of retained sources with synthetic external contracts."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
WORK=Path(__file__).resolve().parent
class Semantics(unittest.TestCase):
    def run_c(self,address,body):
        source=WORK/'nonmatch'/('func_'+address+'.c')
        with tempfile.TemporaryDirectory() as t:
            p=Path(t)
            (p/'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],check=True,capture_output=True)
            subprocess.run([str(p/'test')],check=True,capture_output=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
    def test_loop_counter_sentinel_and_gates(self):
        self.run_c('80021F84',r'''
static u16 random_value;static u8 active_value;static unsigned int random_calls,status_calls;
u16 func_8001E790(void){random_calls++;return random_value;}
u8 func_8001467C(u32 index){assert(index==0x34);status_calls++;return active_value;}
int main(void){static const unsigned int counters[]={0,1,2,65534,65535};static const unsigned int limits[]={0,1,2,65535};static const u32 flags[]={0,8,0x40000000U,0x40000008U,32,40,0x40000028U};MacroCommand program[8],command;MacroState state;unsigned int ci,li,r,g1,g2,f,a,o,d,count,jump,enabled,expected_random,expected_status,cases=0;
for(ci=0;ci<5;ci++)for(li=0;li<4;li++)for(r=0;r<2;r++)for(g1=0;g1<2;g1++)for(g2=0;g2<2;g2++)for(f=0;f<7;f++)for(a=0;a<2;a++)for(o=0;o<2;o++)for(d=0;d<2;d++){
if(counters[ci]==0&&r&&limits[li]==0)continue;
memset(&state,0,sizeof(state));state.start00=program;state.current04=program+1;state.flags24=flags[f];state.identifier60=0xABCDEF34U;state.loop68=(u16)counters[ci];command.word[0]=(g2<<24)|(r<<16)|(g1<<8);command.word[1]=(limits[li]<<16)|(o?7U:0U);random_value=d?65535:0;active_value=(u8)a;random_calls=status_calls=0;
count=counters[ci];expected_random=0;if(count==0){if(r){count=random_value%limits[li];expected_random=1;}else count=limits[li];enabled=count!=0;}else if(count==65535)enabled=1;else{count--;enabled=count!=0;}
jump=0;expected_status=0;if(enabled){if(g1&&(flags[f]&8)&&!(flags[f]&0x40000000U))count=0;else if(g2&&!(flags[f]&32)){expected_status=1;if(!a)count=0;else jump=1;}else jump=1;}
assert(func_80021F84(&state,&command)==0);assert(state.loop68==count&&state.current04==program+(jump?(o?7:0):1)&&state.start00==program);assert(random_calls==expected_random&&status_calls==expected_status);assert(state.flags24==flags[f]&&state.identifier60==0xABCDEF34U);cases++;}
assert(cases==8736);return 0;}
''')
    def test_all_signed_byte_timing_pairs(self):
        self.run_c('80022A98',r'''
static MacroState *expected_state;static u32 converted,pre_flags;static unsigned int conversion_calls;
void func_8001E930(u32 *time){assert(*time==123&&expected_state->flags24==pre_flags);*time=converted;expected_state->flags24^=64;conversion_calls++;}
void func_8001E940(u32 *time,MacroState *state){assert(state==expected_state);func_8001E930(time);}
int main(void){static const u32 periods[]={0,1,0xFFFFFFFFU};MacroState state;MacroCommand command;unsigned int k,c,mode,p,selector,expected_key,expected_cent,cases=0;int key,cent;u32 expected_flags,expected_time,expected_period,initial_flags;typedef char layout[offsetof(MacroState,currentTime6C)==108&&offsetof(MacroState,period70)==112&&offsetof(MacroState,keyRange78)==120&&offsetof(MacroState,centRange79)==121?1:-1];(void)sizeof(layout);
for(k=0;k<256;k++)for(c=0;c<256;c++)for(mode=0;mode<2;mode++)for(p=0;p<3;p++){memset(&state,0,sizeof(state));initial_flags=((k^c)&1)?0xFFFFFFFFU:0xA5123456U;state.flags24=initial_flags;state.period70=0x12345678U;state.currentTime6C=0x98765432U;state.keyRange78=0x3A;state.centRange79=0x7B;selector=(k+c)&3;command.word[0]=(selector<<24)|(c<<16)|(k<<8);command.word[1]=(123U<<16)|(mode<<8);expected_state=&state;converted=periods[p];conversion_calls=0;pre_flags=selector?(initial_flags|0x8000U):(initial_flags&~0x8000U);expected_flags=pre_flags^64;expected_time=state.currentTime6C;expected_period=state.period70;expected_key=state.keyRange78;expected_cent=state.centRange79;key=k>=128?(int)k-256:(int)k;cent=c>=128?(int)c-256:(int)c;
if(converted){expected_flags|=0x4000;expected_period=converted;if(key<0){expected_key=(unsigned int)-key;expected_cent=cent<0?(unsigned int)-cent:(unsigned int)cent;expected_time=converted/2;}else{expected_key=(unsigned int)key;if(cent<0){if(key==0){expected_cent=(unsigned int)-cent;expected_time=converted/2;}else{expected_key--;expected_cent=(unsigned int)(100-cent);expected_time=0;}}else{expected_cent=(unsigned int)cent;expected_time=0;}}}else expected_flags&=~0x4000U;
assert(func_80022A98(&state,&command)==0);assert(conversion_calls==1&&state.flags24==expected_flags&&state.period70==expected_period&&state.currentTime6C==expected_time&&state.keyRange78==expected_key&&state.centRange79==expected_cent);cases++;}
assert(cases==393216);return 0;}
''')
if __name__=='__main__':unittest.main()
