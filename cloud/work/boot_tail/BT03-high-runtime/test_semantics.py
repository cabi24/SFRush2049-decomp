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
    def test_live_vector_matrix_assembly(self):
        self.run_c('8001D5C0','''
static SpatialState *current;static int phase;
typedef char fields[offsetof(SpatialState,position)==12 && offsetof(SpatialState,forward)==36 && offsetof(SpatialState,side)==48 && offsetof(SpatialState,up)==60 && offsetof(SpatialState,inverse)==72?1:-1];
void func_80024D04(Vector *dst,const Vector *a,const Vector *b){assert(phase==0&&dst==&current->side&&a==&current->up&&b==&current->forward);dst->x=101;dst->y=102;dst->z=103;phase=1;}
void func_80024D74(float *dst,const float *m){float expected[12];int i;assert(phase==1&&dst==current->inverse);expected[0]=101;expected[3]=102;expected[6]=103;expected[1]=current->up.x;expected[4]=current->up.y;expected[7]=current->up.z;expected[2]=current->forward.x;expected[5]=current->forward.y;expected[8]=current->forward.z;expected[9]=current->position.x;expected[10]=current->position.y;expected[11]=current->position.z;for(i=0;i<12;i++){assert(m[i]==expected[i]);dst[i]=m[i];}phase=2;}
int main(void){SpatialState s;int k;for(k=0;k<16;k++){memset(&s,0,sizeof(s));s.position.x=k;s.position.y=k+1;s.position.z=k+2;s.up.x=k+3;s.up.y=k+4;s.up.z=k+5;s.forward.x=k+6;s.forward.y=k+7;s.forward.z=k+8;current=&s;phase=0;func_8001D5C0(&s);assert(phase==2);}return 0;}
''')
    def test_handle_chain_keeps_stale_successors(self):
        self.run_c('8001B8C4','''
VoiceState D_8004BEB8[4];u8 D_8002C630;static int lookup,calls;
typedef char layout[sizeof(VoiceState)==416 && offsetof(VoiceState,next_identifier)==16 && offsetof(VoiceState,flags)==36 && offsetof(VoiceState,identifier)==96?1:-1];
int func_8001EDF4(u32 key){assert(key==1234);calls++;return lookup;}
int main(void){int enabled,pattern,i,expected;for(enabled=0;enabled<2;enabled++)for(pattern=0;pattern<16;pattern++){memset(D_8004BEB8,0,sizeof(D_8004BEB8));for(i=0;i<4;i++){D_8004BEB8[i].identifier=0x123400U+i+((pattern&(1<<i))?0:256);D_8004BEB8[i].next_identifier=i<3?0x123401U+i:0xFFFFFFFFU;D_8004BEB8[i].flags=0xA0;}D_8002C630=enabled;lookup=0x123400;calls=0;expected=enabled&&pattern?0:-1;assert(func_8001B8C4(1234)==expected && calls==enabled);for(i=0;i<4;i++)assert(D_8004BEB8[i].flags==(u32)(0xA0|((enabled&&(pattern&(1<<i)))?8:0)));}lookup=-1;calls=0;assert(func_8001B8C4(1234)==-1&&calls==1);return 0;}
''')
    def test_note_reset_flag_classes(self):
        self.run_c('80019BE4','''
static int calls;static u8 expected_channel;
void func_80020FDC(u8 ch,u8 set,u8 on){assert(ch==expected_channel&&set==9&&on==1);calls++;}
int main(void){VoiceState s,expected;int skip,flag,mode,ch;for(skip=0;skip<2;skip++)for(flag=0;flag<2;flag++)for(mode=0;mode<3;mode++)for(ch=0;ch<2;ch++){memset(&s,0xA5,sizeof(s));s.flags=(skip?0x80000U:0)|(flag?0x2000U:0);s.mode98=mode;s.stored90=0x76543210U;s.lastC1=255;s.channel=ch?255:7;s.set=9;expected=s;if(!skip){expected.current8C=mode==1&&!flag?0:s.stored90;expected.fixed94=0xFF0000U;}expected_channel=s.channel;calls=0;func_80019BE4(&s);assert(calls==!ch&&memcmp(&s,&expected,sizeof(s))==0);}return 0;}
''')
    def test_event_cursor_and_context_replacement(self):
        self.run_c('80018A30','''
Context *D_8004BE80;u32 D_8004BE78;static Context a,b;static int calls,replace;
void func_80019A60(u32 value,u8 channel){assert(channel==3);assert(value==D_8004BE80->rate124);if(replace){assert(value==(calls?30U:10U));if(!calls)D_8004BE80=&b;}calls++;}
int main(void){TimedValue one[3]={{0,10},{5,20},{0xFFFFFFFFU,0}},two[3]={{0,99},{0,30},{0xFFFFFFFFU,0}};memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));D_8004BE78=0xAABBCC03U;a.activeF68=1;a.highF74=5;a.currentF6C=one;D_8004BE80=&a;calls=0;func_80018A30();assert(calls==2&&a.currentF6C==one+2);a.currentF6C=one;b.currentF6C=two;b.highF74=5;D_8004BE80=&a;replace=1;calls=0;func_80018A30();assert(calls==2&&a.currentF6C==one&&b.currentF6C==two+2);replace=0;calls=0;a.currentF6C=one+1;a.highF74=0xFFFFFFFFU;a.half120=2;D_8004BE80=&a;func_80018A30();assert(calls==0&&a.currentF6C==one+1);a.activeF68=0;a.currentF6C=0;func_80018A30();assert(calls==0);return 0;}
''')
    def test_fixed_point_low_word_steps(self):
        self.run_c('8001897C','''
Context *D_8004BE80;s32 D_8004F808,D_8004F800;
int main(void){Context c;s32 nums[5]={0,1,-1,60000,(-2147483647-1)};s32 dens[4]={1,-7,32000,48000};u32 rates[4]={0,60,1000,0xFFFFFFFFU};u16 scales[3]={0,256,65535};int i,j,k,l;unsigned long low,step,v;long signed_value;memset(&c,0,sizeof(c));D_8004BE80=&c;for(i=0;i<5;i++)for(j=0;j<4;j++)for(k=0;k<4;k++)for(l=0;l<3;l++){D_8004F808=nums[i];D_8004F800=dens[j];c.rate124=rates[k];c.scaleFC2=scales[l];low=((unsigned long)(u32)nums[i]<<16)&0xFFFFFFFFUL;signed_value=(low&0x80000000UL)?(long)low-4294967296L:(long)low;step=(unsigned long)(signed_value/dens[j])&0xFFFFFFFFUL;v=(((unsigned long)rates[k]*3072UL)&0xFFFFFFFFUL)/60UL;v=((v*step)&0xFFFFFFFFUL)>>3;v=((v*scales[l])&0xFFFFFFFFUL)>>8;func_8001897C();assert(c.fraction118==(v&65535UL)&&c.whole11C==(s32)(v>>16)&&c.half120==(s32)(v>>17));}return 0;}
''',False)
    def test_active_query_live_count_and_flag(self):
        self.run_c('800202C4','''
u8 D_8002C630,D_8004BE94,D_8004FA18;static int override,calls,shrink;static int active_at;
int func_800146AC(void){assert(D_8004BE94==1);return override;}
u8 func_8001467C(int i){assert(D_8004BE94==1&&i==calls);calls++;if(shrink&&i==0)D_8004FA18=2;return i==active_at;}
int main(void){int n;D_8002C630=0;D_8004BE94=77;assert(func_800202C4()==1&&D_8004BE94==77);D_8002C630=1;for(n=0;n<33;n++){D_8004FA18=n;calls=0;override=0;active_at=n? n-1:-1;assert(func_800202C4()==(n==0)&&calls==n&&D_8004BE94==0);}override=1;calls=0;assert(func_800202C4()==0&&calls==0&&D_8004BE94==0);override=0;shrink=1;active_at=-1;D_8004FA18=32;calls=0;assert(func_800202C4()==1&&calls==2&&D_8004BE94==0);return 0;}
''',False)
if __name__=='__main__':unittest.main()
