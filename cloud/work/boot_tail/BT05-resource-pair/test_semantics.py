"""Actual C89 callers with synthetic, width-checked external contracts."""
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
    def test_eleven_inputs_and_child_link_protocol(self):
        self.run_c('80022160',r'''
MacroState D_8004BEB8[4];static MacroState *parent;static u32 child_result,expected_macro,existing_child;static u8 expected_key;static int phase,expected_clone;
u32 func_80024988(u32 macro,u16 allocation,u8 key,u8 volume,u8 pan,u8 channel,u8 set,u16 offset,u16 section,u8 zero,u8 group){assert(phase++==0&&parent->activeBD==1);assert(macro==expected_macro&&allocation==0xFEDC&&key==expected_key&&volume==0xCD&&pan==0x34&&channel==3&&set==4&&offset==0x99AA&&section==0x37&&zero==0&&group==0x45);parent->identifier60=0x12345600U;parent->external4C^=1;if(child_result!=0xFFFFFFFFU){D_8004BEB8[2].identifier60=child_result;D_8004BEB8[2].child10=0xFFFFFFFFU;}return child_result;}
void func_8001B744(MacroState *destination,MacroState *source){assert(phase==1&&expected_clone&&source==parent&&destination==&D_8004BEB8[2]);assert(parent->activeBD==0&&parent->child10==child_result&&destination->parent14==parent->identifier60);if(existing_child!=0xFFFFFFFFU)assert(destination->child10==existing_child&&D_8004BEB8[1].parent14==child_result);phase++;}
int main(void){static const unsigned int notes[]={0,127,128,32767,32768,65535};static const unsigned int deltas[]={0,1,127,128,255};MacroCommand command;unsigned int i,j,external,old,fail,cases=0;int delta,key;typedef char layout[sizeof(MacroState)==416&&offsetof(MacroState,parent14)==20&&offsetof(MacroState,allocationBA)==186&&offsetof(MacroState,activeBD)==189&&offsetof(MacroState,groupBE)==190?1:-1];(void)sizeof(layout);
for(i=0;i<6;i++)for(j=0;j<5;j++)for(external=0;external<2;external++)for(old=0;old<2;old++)for(fail=0;fail<2;fail++){memset(D_8004BEB8,0xA5,sizeof(D_8004BEB8));parent=&D_8004BEB8[0];parent->originalNote4E=(u16)notes[i];parent->allocationBA=0xFEDC;parent->volume30=0xABCD0000U;parent->panning38=0x12340000U;parent->channel4A=3;parent->set4B=4;parent->section2F=0x37;parent->groupBE=0x45;parent->external4C=(u8)external;existing_child=old?0x222201U:0xFFFFFFFFU;parent->child10=existing_child;command.word[0]=0xAABB0000U|(deltas[j]<<8)|0x42U;command.word[1]=0x778899AAU;expected_macro=0xAABB8877U;delta=deltas[j]>=128?(int)deltas[j]-256:(int)deltas[j];key=(int)notes[i]+delta;if(key<0)key=0;else if(key>127)key=127;expected_key=(u8)(key|(external?128:0));child_result=fail?0xFFFFFFFFU:0x333302U;expected_clone=!fail&&!external;phase=0;assert(func_80022160(parent,&command)==0);assert(parent->activeBD==0&&phase==1+expected_clone);if(fail){assert(parent->child10==existing_child);}else{assert(parent->child10==child_result&&D_8004BEB8[2].parent14==parent->identifier60);if(old)assert(D_8004BEB8[2].child10==existing_child&&D_8004BEB8[1].parent14==child_result);else assert(D_8004BEB8[2].child10==0xFFFFFFFFU);}assert(command.word[0]==(0xAABB0000U|(deltas[j]<<8)|0x42U)&&command.word[1]==0x778899AAU);cases++;}
assert(cases==240);return 0;}
''')
    def test_descriptor_offset_and_playback_protocol(self):
        self.run_c('80022874',r'''
SampleInfo D_80056208;static MacroState *expected_state;static u32 length_value,expected_offset;static int fail,phase;static u8 expected_reset;
int func_80016CF0(u16 sample,SampleInfo *info){assert(sample==0xBEEF&&info==&D_80056208&&phase++==0);if(fail)return -1;info->info=0x12345678U;info->address=0x80001000U;info->offset=0;info->length=length_value;info->loopStart=3;info->loopLength=4;info->type=5;return 0;}
void func_800146B4(u32 index,SampleInfo *info,u8 reset){assert(phase++==1&&index==0x14&&info==&D_80056208&&reset==expected_reset&&info->offset==expected_offset);assert(info->length==length_value&&info->loopStart==3&&info->loopLength==4&&info->type==5);info->info=0x87654321U;info->address=0x80002000U;expected_state->flags24^=0x40;}
int main(void){static const u32 values[]={0,1,0x7F0000U,0x80000000U,0xFFFF0000U};static const u32 lengths[]={0,1,64,0xFFFFFFFFU};static const u32 offsets[]={0,1,0xFFFFFFFFU,0x12345678U};static const unsigned int modes[]={0,1,2,255};MacroState state,prior;MacroCommand command;unsigned int v,l,o,m,reset,failed,cases=0;unsigned long product,volume_high;u32 initial_flags;typedef char layout[offsetof(SampleInfo,offset)==8&&offsetof(SampleInfo,length)==12&&offsetof(SampleInfo,type)==24?1:-1];(void)sizeof(layout);
for(v=0;v<5;v++)for(l=0;l<4;l++)for(o=0;o<4;o++)for(m=0;m<4;m++)for(reset=0;reset<2;reset++)for(failed=0;failed<2;failed++){memset(&state,0xA5,sizeof(state));initial_flags=reset?0:0x200;state.flags24=initial_flags;state.volume30=values[v];state.identifier60=0xCAFE1214U;prior=state;expected_state=&state;length_value=lengths[l];expected_reset=(u8)reset;fail=failed;phase=0;command.word[0]=(modes[m]<<24)|(0xBEEFU<<8)|0x42U;command.word[1]=offsets[o];volume_high=values[v]>>16;if(modes[m]==0)expected_offset=offsets[o];else if(modes[m]==1){product=((unsigned long)offsets[o]*((127UL-volume_high)&0xFFFFFFFFUL))&0xFFFFFFFFUL;expected_offset=(u32)(product/127UL);}else if(modes[m]==2){product=((unsigned long)offsets[o]*volume_high)&0xFFFFFFFFUL;expected_offset=(u32)(product/127UL);}else expected_offset=0;if(expected_offset>=length_value)expected_offset=length_value-1U;assert(func_80022874(&state,&command)==0);if(fail)assert(phase==1&&memcmp(&state,&prior,sizeof(state))==0);else assert(phase==2&&state.sampleInfo5C==0x87654321U&&state.sampleAddress58==0x80002000U&&state.flags24==((initial_flags^0x40)|0x20)&&state.volume30==values[v]);cases++;}
assert(cases==1280);return 0;}
''')
if __name__=='__main__':unittest.main()
