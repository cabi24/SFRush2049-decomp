"""Actual retained C89 sources with bounded synthetic helper contracts."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
class Semantics(unittest.TestCase):
    def run_c(self, address, body, match=False):
        source = ROOT/'cloud/matches/boot_tail'/('func_'+address+'.c') if match else WORK/'nonmatch'/('func_'+address+'.c')
        with tempfile.TemporaryDirectory() as t:
            p=Path(t)
            (p/'test.c').write_text('#include <assert.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body)
            subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],check=True,capture_output=True)
            subprocess.run([str(p/'test')],check=True,capture_output=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
    def test_packed_note_update(self):
        self.run_c('80022678',r'''
static MacroState *expected_state;static MacroCommand *expected_command;static unsigned int expected_note;static int enabled,phase;
int func_80021764(MacroState *s) { assert(s==expected_state&&s->note50==expected_note&&phase==0);phase=1;return enabled; }
void func_80020F4C(u8 channel,u8 set,u8 note) { assert(channel==3&&set==4&&note==expected_note&&phase==1);phase=2; }
u8 func_8002193C(MacroState *s,MacroCommand *c) { assert(s==expected_state&&c==expected_command&&c->word[0]==4&&phase==(enabled?2:1));phase=3;return 173; }
int main(void) { static const unsigned int notes[]={0,127,128,32767,32768,65535};static const unsigned int deltas[]={0,1,127,128,255};MacroState s;MacroCommand c;unsigned int i,j,mode,en,low;int delta,signed_note;
for(i=0;i<6;i++)for(j=0;j<5;j++)for(mode=0;mode<2;mode++)for(en=0;en<2;en++) { memset(&s,0xA5,sizeof(s));s.note50=(u16)notes[i];s.originalNote4E=(u16)notes[(i+1)%6];s.channel4A=3;s.set4B=4;c.word[0]=(mode<<24)|(0xE7U<<16)|(deltas[j]<<8);c.word[1]=0x12345678U;delta=deltas[j]>=128?(int)deltas[j]-256:(int)deltas[j];low=((mode?s.originalNote4E:s.note50)+delta)&65535U;signed_note=low>=32768?(int)low-65536:(int)low;expected_note=signed_note<0?0:low>127?127:low;expected_state=&s;expected_command=&c;enabled=en;phase=0;assert(func_80022678(&s,&c)==173);assert(phase==3&&s.note50==expected_note&&s.detuneC0==(s8)0xE7&&c.word[1]==0x12345678U); }
return 0; }
''',True)
    def test_envelope_real_third_input(self):
        self.run_c('800233B0',r'''
static MacroState *expected_state;static u32 converted,result_word,expected_target;static u16 expected_curve;static int phase;
void func_8001E930(u32 *v) { assert(v==&expected_state->timeB4&&*v==123&&phase==0);*v=converted;phase=1; }
void func_8001E940(u32 *v,MacroState *s) { assert(s==expected_state);func_8001E930(v); }
u32 func_8001E9A0(u32 time) { assert(time==converted&&phase==1);phase=2;return time>>8; }
u32 func_8002321C(u32 target,u16 curve) { assert(target==expected_target&&curve==expected_curve&&phase==2);phase=3;return result_word; }
int main(void) { static const u32 values[]={0,1,0x7F0000U,0xFFFFFFFFU,0x80000000U};static const u32 times[]={0,256,0xFFFFFF00U};union Aligned {u32 align;MacroState state;} storage;MacroState *s=&storage.state;MacroCommand c;unsigned int i,j,k,mode;unsigned long low,sum;long signed_difference,divisor;u32 start;
for(i=0;i<5;i++)for(j=0;j<5;j++)for(k=0;k<3;k++)for(mode=0;mode<2;mode++) { memset(s,0,sizeof(*s));s->volume30=values[i];s->flags24=0xA5150000U;start=values[j];converted=times[k];result_word=values[(i+j)%5];expected_state=s;phase=0;c.word[0]=(0xA5U<<24)|(127U<<16)|(255U<<8);c.word[1]=(123U<<16)|(mode<<8)|0x5BU;low=((unsigned long)values[i]*255UL)&0xFFFFFFFFUL;sum=((low>>7)+(127UL<<16))&0xFFFFFFFFUL;expected_target=sum>0x7F0000UL?0x7F0000U:(u32)sum;expected_curve=0xA55B;low=((unsigned long)result_word-(unsigned long)start)&0xFFFFFFFFUL;signed_difference=low>=0x80000000UL?(long)low-4294967296L:(long)low;divisor=(long)(converted>>8);if(!divisor)divisor=1;assert(func_800233B0(s,&c,start)==0);assert(phase==3&&s->timeB4==converted&&s->target88==result_word&&s->delta84==signed_difference/divisor&&s->volume30==start&&s->flags24==(0xA5150000U|0x20000U)); }
return 0; }
''')
    def test_duration_two_mode_contract(self):
        self.run_c('800239A4',r'''
static MacroState *expected_state;static u32 converted;static int phase,controller_calls,state_calls;
void func_8001E930(u32 *v) { assert(*v==321&&phase==0);*v=converted;phase=1; }
void func_8001E940(u32 *v,MacroState *s) { assert(s==expected_state);func_8001E930(v); }
void func_80020FDC(u8 channel,u8 set,u8 value) { assert(channel==3&&set==4&&value==0&&phase==1);controller_calls++; }
void func_80019BE4(MacroState *s) { assert(s==expected_state&&phase==1&&s->time90==converted);s->flags24^=0x80;state_calls++; }
int main(void) { static const u32 flags[]={0,0x800,0xFFFFFFFFU};static const unsigned int modes[]={0,1,2,255};MacroState s;MacroCommand c;unsigned int i,j,channel,timing;u32 expected;
for(i=0;i<3;i++)for(j=0;j<4;j++)for(channel=0;channel<2;channel++)for(timing=0;timing<2;timing++) { memset(&s,0,sizeof(s));s.flags24=flags[i];s.channel4A=channel?255:3;s.set4B=4;c.word[0]=(0xB7U<<16)|(modes[j]<<8);c.word[1]=(321U<<16)|(timing<<8);converted=0x12345678U;expected_state=&s;phase=controller_calls=state_calls=0;expected=flags[i];if(modes[j]==0)expected=(expected&~0x800U)|0x1000U;else if(modes[j]==1){if(!(expected&0x800))expected^=0x80;expected|=0x1800;}assert(func_800239A4(&s,&c)==0);assert(phase==1&&s.time90==converted&&s.mode98==0xB7&&s.flags24==expected);assert(controller_calls==(modes[j]==0&&!channel));assert(state_calls==(modes[j]==1&&!(flags[i]&0x800))); }
return 0; }
''')
if __name__=='__main__':unittest.main()
