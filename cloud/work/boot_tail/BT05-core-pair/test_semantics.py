"""Actual-source C89 contracts; host pointer widths are not N64 layout proof."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
WORK = Path(__file__).resolve().parent


class Semantics(unittest.TestCase):
    def run_c(self, address, body):
        source = WORK / 'nonmatch' / ('func_' + address + '.c')
        with tempfile.TemporaryDirectory(prefix='bt05-core-test-') as tmp:
            path = Path(tmp)
            (path / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(path / 'test.c'), '-o', str(path / 'test')], check=True, capture_output=True)
            subprocess.run([str(path / 'test')], check=True, capture_output=True,
                           env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))

    def test_wait_guards_sentinel_and_wrapping_clocks(self):
        self.run_c('8002193C', r'''
static u16 random_value;
static u8 active;
static int gate_calls,random_calls,milli_calls,beat_calls;
u8 func_8001467C(u32 slot) { assert(slot==7); gate_calls++; return active; }
u16 func_8001E790(void) { random_calls++; return random_value; }
void func_8001E930(u32 *time) { milli_calls++; *time <<= 8; }
void func_8001E940(u32 *time,MacroState *state) { assert(state->identifier60==0x12345607U); beat_calls++; *time *= 3U; }
typedef char layout[(offsetof(MacroState,flags24)==36&&offsetof(MacroState,identifier60)==96&&offsetof(MacroState,deadline9C)==156&&offsetof(MacroState,clockA4)==164)?1:-1];
static u8 model(MacroState *s,MacroCommand *c,int *calls) {
    u32 t;int mode;
    t=c->word[1]>>16;
    if(!t)return 0;
    if(c->word[0]&0x100U){if(s->flags24&8)return 0;s->flags24|=4;}else s->flags24&=~4U;
    if(c->word[0]&0x1000000U){if(!(s->flags24&32)){calls[0]++;if(!active)return 0;}s->flags24|=0x100000;}else s->flags24&=~0x100000U;
    if(c->word[0]&0x10000U){calls[1]++;t=random_value%t;}
    if(t==65535){s->deadline9C=0xFFFFFFFFU;return 1;}
    mode=(c->word[1]&0x100U)!=0;
    if(mode){calls[2]++;t<<=8;s->deadline9C=s->clockA4+t;}
    else{calls[3]++;t*=3U;s->deadline9C=s->baseA0+t;if(s->deadline9C<=s->clockA4){s->baseA0=s->deadline9C;s->deadline9C=0;}}
    return s->deadline9C!=0;
}
int main(void) {
    u16 times[9]={0,1,2,127,128,255,32768,65534,65535};
    u16 randoms[3]={0,32768,65535};
    u32 clocks[4]={0,1,0xFFFFFF00U,0xFFFFFFFFU};
    MacroState state,expected;MacroCommand command,before;
    int i,f,c,a,m,k,r,expected_calls[4],cases;u8 got,want;
    cases=0;
    for(i=0;i<9;i++)for(f=0;f<8;f++)for(c=0;c<8;c++)for(a=0;a<2;a++)for(m=0;m<2;m++)for(k=0;k<4;k++)for(r=0;r<3;r++){
        memset(&state,0xA5,sizeof(state));
        state.flags24=0x80000004U|((f&1)?8:0)|((f&2)?32:0)|((f&4)?0x100000:0);
        state.identifier60=0x12345607U;state.baseA0=clocks[k];state.clockA4=clocks[(k+1)%4];
        command.word[0]=((c&1)?0x100U:0)|((c&2)?0x10000U:0)|((c&4)?0x1000000U:0);
        command.word[1]=((u32)times[i]<<16)|(m?0x100U:0)|0x5AU;
        before=command;expected=state;random_value=randoms[r];active=(u8)a;
        gate_calls=random_calls=milli_calls=beat_calls=0;memset(expected_calls,0,sizeof(expected_calls));
        want=model(&expected,&command,expected_calls);got=func_8002193C(&state,&command);
        assert(got==want&&memcmp(&state,&expected,sizeof(state))==0&&memcmp(&command,&before,sizeof(command))==0);
        assert(gate_calls==expected_calls[0]&&random_calls==expected_calls[1]&&milli_calls==expected_calls[2]&&beat_calls==expected_calls[3]);cases++;
    }
    assert(cases==27648);return 0;
}
''')

    def test_eleven_input_constructor_and_two_return_domains(self):
        self.run_c('80024988', r'''
MacroState D_8004BEB8[4];
static MacroCommand program[5];
static u32 packed_value,sequence_result;
static u16 allocation_value;
static u8 original_key,active;
static int missing,allocation_slot,phase,external_calls,priority_calls,sequence_calls,change_identifier;
MacroCommand *func_80016C20(u16 macro) { assert(phase++==0&&macro==(packed_value>>16)); return missing?0:program; }
int func_8001F13C(u8 priority,u8 group,u16 allocation,u8 external) { assert(phase++==1&&priority==(u8)(packed_value>>8)&&group==(u8)packed_value&&allocation==allocation_value&&external==((original_key&128)!=0));return allocation_slot; }
void func_8001EB10(MacroState *s) { assert(phase++==2&&s==D_8004BEB8+allocation_slot); memset(s,0xA5,sizeof(*s));s->flags24=0x12345678U; }
u8 func_8001467C(u32 index) { assert(phase++==3&&index==(u32)allocation_slot);assert(D_8004BEB8[index].flags24==0x12);return active; }
void func_80020820(u8 slot,u8 category) { assert(phase++==4&&slot==allocation_slot&&category==255);external_calls++;D_8004BEB8[slot].flags24|=0x80; }
void func_8001EF8C(MacroState *s,u8 priority) { assert(phase++==(original_key&128?5:4)&&s==D_8004BEB8+allocation_slot&&priority==(u8)(packed_value>>8));priority_calls++;if(change_identifier)s->identifier60=0x56789A03U; }
u32 func_8001ECE0(MacroState *s) { assert(phase++==(original_key&128?6:5)&&s==D_8004BEB8+allocation_slot);sequence_calls++;return sequence_result; }
int main(void) {
    u8 keys[4]={0,127,128,255};u32 packed[2]={0x00017F55U,0xFFFFFFFFU};
    u32 sequence[3]={0xFFFFFFFFU,0,0x98765432U};
    int k,p,a,v,o,start,mut,q,on,cases;u32 result,identifier;MacroState *s;
    cases=0;
    for(k=0;k<4;k++)for(p=0;p<2;p++)for(a=0;a<2;a++)for(v=0;v<2;v++)for(o=0;o<2;o++)for(start=0;start<2;start++)for(mut=0;mut<2;mut++)for(q=0;q<3;q++)for(on=0;on<2;on++){
        memset(D_8004BEB8,0,sizeof(D_8004BEB8));phase=external_calls=priority_calls=sequence_calls=0;
        missing=0;allocation_slot=3;packed_value=packed[p];allocation_value=(u16)(a?65535:0);original_key=keys[k];active=(u8)on;sequence_result=sequence[q];change_identifier=mut;
        result=func_80024988(packed_value,allocation_value,original_key,(u8)(v?255:0),255,4,7,(u16)(o?3:0),0x1234,(u8)start,255);
        s=D_8004BEB8+3;identifier=(packed_value&0xFFFF0000U)|((u32)(original_key&127)<<8)|3;
        if(mut)identifier=0x56789A03U;
        assert(result==(start?sequence_result:identifier)&&s->identifier60==identifier);
        assert(priority_calls==1&&sequence_calls==start&&external_calls==((original_key&128)!=0));
        assert(s->flags24==(u32)(0x12|on|((original_key&128)?0x80:0))&&s->deadline9C==0);
        assert(s->external4C==((original_key&128)!=0)&&s->channel54==(original_key&128?3:4)&&s->set55==(original_key&128?255:7));
        assert(s->macro64==(packed_value>>16)&&s->allocationBA==allocation_value&&s->age28==0x75300000U&&s->ageSpeed2C==1024);
        assert(s->start00==program&&s->current04==program+(o?3:0)&&s->saved08==0);
        assert(s->originalNote4E==(original_key&127)&&s->note50==(original_key&127)&&s->detuneC0==0);
        assert(s->volume52==(v?255:0)&&s->panning53==255&&s->section56==0x34&&s->groupBF==255);
        assert(s->child10==0xFFFFFFFFU&&s->parent14==0xFFFFFFFFU);cases++;
    }
    assert(cases==1536);
    phase=0;missing=1;assert(func_80024988(packed_value,allocation_value,original_key,0,0,0,0,0,0,0,0)==0xFFFFFFFFU&&phase==1);
    phase=0;missing=0;allocation_slot=-1;assert(func_80024988(packed_value,allocation_value,original_key,0,0,0,0,0,0,0,0)==0xFFFFFFFFU&&phase==2);
    return 0;
}
''')


if __name__ == '__main__':
    unittest.main()
