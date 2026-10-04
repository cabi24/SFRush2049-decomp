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
    def test_allocator_external_channel_protocol(self):
        self.run_c('8001F898','''
VoiceState D_8004BEB8[4];static int slot,active,phase;static u8 selected_channel;
typedef char layout[sizeof(VoiceState)==416 && offsetof(VoiceState,external4C)==76 && offsetof(VoiceState,identifier60)==96 && offsetof(VoiceState,activeBD)==189?1:-1];
int func_8001F13C(u8 ch,u8 b,u16 c,u8 d){assert(phase++==0&&ch==selected_channel&&b==255&&c==65535&&d==1);return slot;}
void func_8001EB10(VoiceState *s){assert(phase++==1&&s==D_8004BEB8+slot&&s->activeBD==1&&s->external4C==1);s->identifier60=0;s->command00=0x12345678;}
u8 func_8001467C(int i){assert(phase++==2&&i==slot&&D_8004BEB8[i].identifier60==(0xFFFFFF00U|(u32)i));return active;}
void func_80014AF0(int i){assert(phase++==3&&i==slot&&active);}
void func_8001EF8C(VoiceState *s,u8 ch){assert(phase==3+active&&s==D_8004BEB8+slot&&ch==selected_channel&&s->command00==0);phase++;}
int main(void){int i,j,k;int slots[3]={-1,0,3};u8 channels[3]={0,127,255};for(i=0;i<3;i++)for(j=0;j<2;j++)for(k=0;k<3;k++){memset(D_8004BEB8,0xA5,sizeof(D_8004BEB8));slot=slots[i];active=j;selected_channel=channels[k];phase=0;assert(func_8001F898(selected_channel)==slot);assert(phase==(slot<0?1:4+active));}return 0;}
''')
    def test_chain_saved_successor_across_release(self):
        self.run_c('8001FA18','''
VoiceState D_8004BEB8[4];u8 D_8002C630;static int lookup,calls,reset_calls;
int func_8001EDF4(u32 key){assert(key==1234);return lookup;}
void func_8001F9D0(VoiceState *s){assert(s>=D_8004BEB8&&s<D_8004BEB8+4);s->next_identifier=0xFFFFFFFFU;reset_calls++;}
void func_80014B3C(int i){assert(i>=0&&i<4);D_8004BEB8[i].next_identifier=0xFFFFFFFFU;calls++;}
int main(void){int mask,i,enabled;for(mask=0;mask<16;mask++)for(enabled=0;enabled<2;enabled++){int expected;expected=0;D_8002C630=enabled;lookup=0x123400;calls=reset_calls=0;memset(D_8004BEB8,0,sizeof(D_8004BEB8));for(i=0;i<4;i++){D_8004BEB8[i].identifier60=0x123400U+i+((mask&(1<<i))?0:256);D_8004BEB8[i].next_identifier=i<3?0x123401U+i:0xFFFFFFFFU;D_8004BEB8[i].command00=1;if(mask&(1<<i))expected++;}assert(func_8001FA18(1234)==(enabled&&expected?0:-1));assert(calls==(enabled?expected:0)&&reset_calls==calls);}lookup=-1;assert(func_8001FA18(1234)==-1);return 0;}
''')
    def test_release_filter_and_live_count(self):
        self.run_c('8001FAE4','''
VoiceState D_8004BEB8[4];u8 D_8004FA18;static int released[4],reset[4],shrink;
void func_8001F9D0(VoiceState *s){int i;i=(int)(s-D_8004BEB8);reset[i]++;s->flags24=0xA7;}
void func_80014B3C(int i){released[i]++;if(shrink&&i==0)D_8004FA18=1;}
int main(void){int p,mask,i,skip;for(p=0;p<2;p++)for(mask=0;mask<16;mask++){memset(D_8004BEB8,0,sizeof(D_8004BEB8));memset(released,0,sizeof(released));memset(reset,0,sizeof(reset));D_8004FA18=4;for(i=0;i<4;i++){D_8004BEB8[i].command00=(mask&(1<<i))?1:0;D_8004BEB8[i].external4C=i&1;}func_8001FAE4(p?255:0);for(i=0;i<4;i++){skip=p&&(mask&(1<<i))&&(i&1);assert(released[i]==!skip&&reset[i]==(!skip&&!!(mask&(1<<i))));if(reset[i])assert(D_8004BEB8[i].command00==0&&D_8004BEB8[i].flags24==0xA4);}}memset(D_8004BEB8,0,sizeof(D_8004BEB8));memset(released,0,sizeof(released));D_8004FA18=4;shrink=1;func_8001FAE4(0);assert(released[0]==1&&released[1]==0);return 0;}
''')
    def test_timed_nodes_wrap_and_next_save(self):
        self.run_c('80018184','''
Context *D_8004BE80;static int stops,removes;
int func_8001B8C4(u32 id){assert(id==99);stops++;return 0;}
void func_800174D0(TimedNode *n){removes++;n->next=0;}
static long signed_word(unsigned long x){x&=0xFFFFFFFFUL;return x>=0x80000000UL?(long)x-4294967296L:(long)x;}
int main(void){Context c;TimedNode a,b;u32 values[5]={0,1,65535,0xFFFFFFFFU,0x80000000U};unsigned int i,j;unsigned long raw,whole;long carry;memset(&c,0,sizeof(c));D_8004BE80=&c;assert(func_80018184()==0);for(i=0;i<5;i++)for(j=0;j<5;j++){memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));a.next=&b;a.identifier=99;a.deadline=0;b.deadline=2147483647;b.whole=1;b.fraction=values[i];c.headF78=&a;c.half120=0;c.whole11C=-2;c.fraction118=values[j];raw=((unsigned long)values[i]+values[j])&0xFFFFFFFFUL;carry=signed_word(raw);carry=carry<0?-((-carry+65535L)/65536L):carry/65536L;whole=((unsigned long)1+(unsigned long)carry+(unsigned long)(u32)-2)&0xFFFFFFFFUL;stops=removes=0;assert(func_80018184()==1&&stops==1&&removes==1);assert(b.fraction==(raw&65535UL)&&b.whole==(s32)signed_word(whole));}return 0;}
''',False)
    def test_program_mapping_byte_recipe(self):
        self.run_c('80017720','''
int main(void){Context c,expected;ProgramEntry primary[7],secondary[7];int p,ch,i;unsigned int index;unsigned long word;memset(&c,0xA5,sizeof(c));c.primary=primary;c.secondary=secondary;for(i=0;i<7;i++){primary[i].high=65535-i;primary[i].middle=i*31;primary[i].low=255-i;secondary[i].high=i*997;secondary[i].middle=255-i;secondary[i].low=i*17;}for(p=0;p<128;p++){c.primary_map[p]=p%11? p%7:255;c.secondary_map[p]=p%13? (p+1)%7:255;}for(p=0;p<128;p++)for(ch=0;ch<16;ch++){for(i=0;i<16;i++)c.selected[i]=0xA5A5A5A5U;expected=c;index=ch==9?c.secondary_map[p]:c.primary_map[p];if(index!=255){ProgramEntry *e;e=ch==9?secondary+index:primary+index;word=(unsigned long)e->high*65536UL+e->middle*256UL+e->low;expected.selected[ch]=(u32)word;}func_80017720(&c,p,ch);assert(memcmp(&c,&expected,sizeof(c))==0);}return 0;}
''',False)
    def test_preset_ten_argument_contract(self):
        self.run_c('8001B1D0','''
static Preset data;static int missing,calls;static u8 expected_first,expected_second;static u16 lookup_id;
Preset *func_80017040(u16 id){assert(id==lookup_id);return missing?0:&data;}
u32 func_8001A270(u32 word,u8 flags,u8 first,u8 second,u8 a5,u8 a6,u16 a7,u16 a8,u8 a9,s16 a10){assert(word==0xABCDEF01U&&flags==0x91&&first==expected_first&&second==expected_second&&a5==255&&a6==255&&a7==0&&a8==255&&a9==0xF1&&a10==0);calls++;return 0xFEDCBA98U;}
int main(void){u8 values[3]={0,127,255};int i,j;data.high=0xABCD;data.middle=0xEF;data.low=1;data.flags=0x11;data.default_first=9;data.default_second=10;data.parameter=0xF1;lookup_id=0xFEDC;missing=1;assert(func_8001B1D0(lookup_id,0,0)==0xFFFFFFFFU&&calls==0);missing=0;for(i=0;i<3;i++)for(j=0;j<3;j++){expected_first=values[i]==255?9:values[i];expected_second=values[j]==255?10:values[j];calls=0;assert(func_8001B1D0(lookup_id,values[i],values[j])==0xFEDCBA98U&&calls==1);}return 0;}
''',False)
if __name__=='__main__':unittest.main()
