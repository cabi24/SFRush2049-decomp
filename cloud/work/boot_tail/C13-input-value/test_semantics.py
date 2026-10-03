"""Triple-check supported input formats: canonical MIPS, host C89 and integer model."""
import ctypes as C
import importlib.util
import itertools
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('c13_native',WORK/'native_replay.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
BIPOLAR={128,1,10,160,161,131,132}
MASK=0xFFFFFFFF


class Voice(C.Structure):
    _pack_=1
    _fields_=[('unknown00',C.c_ubyte*36),('flags24',C.c_uint32),('unknown28',C.c_ubyte*34),
              ('channel4A',C.c_ubyte),('set4B',C.c_ubyte),('unknown4C',C.c_ubyte*20),
              ('id60',C.c_uint32),('unknown64',C.c_ubyte*268),('lfo170',C.c_int16),
              ('unknown172',C.c_ubyte*10),('lfo17C',C.c_int16),('unknown17E',C.c_ubyte*34)]


class Source(C.Structure):
    _pack_=1
    _fields_=[('controller',C.c_ubyte),('combine',C.c_ubyte),('scale',C.c_int16)]


class Input(C.Structure):
    _pack_=1
    _fields_=[('source',Source*4),('count',C.c_ubyte),('unknown11',C.c_ubyte)]


def encoded(v,control):
    voice=bytearray(bytes(v))
    struct.pack_into('>h',voice,368,v.lfo170);struct.pack_into('>h',voice,380,v.lfo17C)
    data=bytearray(18)
    for i,s in enumerate(control.source):
        data[i*4:i*4+4]=bytes([s.controller,s.combine])+struct.pack('>h',s.scale)
    data[16],data[17]=control.count,control.unknown11
    return bytes(voice),bytes(data)


def helper_contract(seed,mode):
    def helper(args,memory,n):
        controller,channel,set_no=args
        result=(controller*73+channel*31+set_no*17+seed+n*97)&65535
        if n==0:
            if mode==1: memory(native.INPUT+16,1,0)
            elif mode==2:
                memory(native.INPUT+2,1,255);memory(native.INPUT+3,1,128)
                memory(native.INPUT+1,1,2)
                memory(native.VOICE+74,1,23);memory(native.VOICE+75,1,5)
            elif mode==3: memory(native.INPUT+16,1,4)
        return result
    return helper


def model(voice_bytes,input_bytes,helper):
    voice,data=bytearray(voice_bytes),bytearray(input_bytes)
    calls=[]
    def memory(address,width,value=None):
        region,offset=(voice,address-native.VOICE) if native.VOICE<=address<native.VOICE+416 else (data,address-native.INPUT)
        if not 0<=offset<=len(region)-width: raise ValueError('model memory bound')
        if value is None:return int.from_bytes(region[offset:offset+width],'big')
        region[offset:offset+width]=(value&((1<<(8*width))-1)).to_bytes(width,'big')
    value=0;i=0
    while i<data[16]:
        if i>=4:raise ValueError('unsupported count')
        controller=data[4*i]
        bipolar=controller in BIPOLAR
        if bipolar and controller in (160,161):
            tmp=struct.unpack_from('>h',voice,368 if controller==160 else 380)[0]*2
        else:
            args=(controller,voice[74],voice[75]);tmp=helper(args,memory,len(calls));calls.append(args)
            if bipolar:tmp-=8192
        scale=struct.unpack_from('>h',data,4*i+2)[0]
        tmp=native.signed(scale*tmp)>>8
        combine=data[4*i+1]
        if combine not in (0,1,2):raise ValueError('unsupported combine mode')
        if bipolar:
            tmp=max(-8192,min(8191,tmp))
            if combine==0:combined=tmp
            elif combine==1:combined=max(-8192,min(8191,native.signed(value+tmp-8192)))
            else:combined=max(-8192,min(8191,native.signed((value-8192)*tmp)>>13))
            value=(combined+8192)&MASK
        else:
            tmp=min(16383,tmp)
            if combine==0:value=tmp&MASK
            elif combine==1:value=min((value+tmp)&MASK,16383)
            else:value=min(((value*tmp)&MASK)>>14,16383)
        i+=1
    return {'result':value&65535,'voice':bytes(voice),'input':bytes(data),'calls':calls}


class SemanticsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cc=shutil.which('cc')
        if not cc:raise RuntimeError('host C compiler required')
        cls.temp=tempfile.TemporaryDirectory(prefix='c13-input-host-')
        p=Path(cls.temp.name)
        harness='#include "'+str(WORK/'func_80021150_NONMATCH.c')+'"\n'+'''
#include <stddef.h>
static VoiceState *context_voice;
static ControlInput *context_input;
static u32 context_seed, context_mode, call_count;
static u32 call_args[4][3];
void set_context(VoiceState *v, ControlInput *input, u32 seed, u32 mode) {
    context_voice=v;context_input=input;context_seed=seed;context_mode=mode;call_count=0;
}
u16 func_80020A04(u8 controller, u8 channel, u8 set) {
    u32 n, result;
    n=call_count++;
    if(n<4) {call_args[n][0]=controller;call_args[n][1]=channel;call_args[n][2]=set;}
    result=controller*73U+channel*31U+set*17U+context_seed+n*97U;
    if(n==0) {
        if(context_mode==1) context_input->count=0;
        else if(context_mode==2) {
            context_input->source[0].scale=-128;
            context_input->source[0].combine=2;
            context_voice->channel4A=23;context_voice->set4B=5;
        } else if(context_mode==3) context_input->count=4;
    }
    return result;
}
u32 calls(void) {return call_count;}
u32 call_argument(u32 n,u32 field) {return call_args[n][field];}
typedef char word_size_check[sizeof(u32)==4 && sizeof(s32)==4 ? 1 : -1];
typedef char source_size_check[sizeof(ControlSource)==4 ? 1 : -1];
typedef char input_size_check[sizeof(ControlInput)==18 ? 1 : -1];
typedef char count_check[offsetof(ControlInput,count)==16 ? 1 : -1];
typedef char voice_size_check[sizeof(VoiceState)==416 ? 1 : -1];
'''
        (p/'h.c').write_text(harness)
        subprocess.run([cc,'-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-shared','-fPIC',str(p/'h.c'),'-o',str(p/'test.so')],check=True,capture_output=True,text=True)
        cls.lib=C.CDLL(str(p/'test.so'))
        cls.lib.set_context.argtypes=[C.POINTER(Voice),C.POINTER(Input),C.c_uint32,C.c_uint32]
        cls.lib.func_80021150.argtypes=[C.POINTER(Voice),C.POINTER(Input)];cls.lib.func_80021150.restype=C.c_uint16
        cls.lib.calls.restype=C.c_uint32;cls.lib.call_argument.argtypes=[C.c_uint32,C.c_uint32];cls.lib.call_argument.restype=C.c_uint32
        score.ASM_DIR=ROOT/'asm/us/boot_tail';cls.words=score.targets()['func_80021150']
        cls.fixtures=0

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def check_fixture(self,entries,count,lfo=(0,0),channel=2,set_no=3,seed=0,mode=0):
        self.assertTrue(0<=count<=4 and len(entries)==4)
        self.assertTrue(all(x[1] in (0,1,2) for x in entries))
        voice=Voice();C.memset(C.byref(voice),0xA5,C.sizeof(voice))
        voice.channel4A=channel;voice.set4B=set_no;voice.lfo170,voice.lfo17C=lfo
        inp=Input();inp.count=count;inp.unknown11=0x55
        for i,(controller,combine,scale) in enumerate(entries):
            inp.source[i].controller=controller;inp.source[i].combine=combine;inp.source[i].scale=scale
        vb,ib=encoded(voice,inp)
        expected=model(vb,ib,helper_contract(seed,mode))
        for fill in (0x5A,0x91):
            result=native.execute(self.words,vb,ib,helper_contract(seed,mode),stack_fill=fill)
            for field in ['result','voice','input','calls']:self.assertEqual(result[field],expected[field],(entries,count,lfo,mode,field))
            self.assertEqual(bool(result['uninitialized_stack_reads']),count!=0)
        self.lib.set_context(C.byref(voice),C.byref(inp),seed,mode)
        actual=self.lib.func_80021150(C.byref(voice),C.byref(inp))
        self.assertEqual(actual,expected['result'],(entries,count,lfo,mode))
        self.assertEqual(encoded(voice,inp),(expected['voice'],expected['input']))
        self.assertEqual([tuple(self.lib.call_argument(i,j) for j in range(3)) for i in range(self.lib.calls())],expected['calls'])
        type(self).fixtures+=1

    def test_zero_count(self):self.check_fixture([(7,0,256)]*4,0)

    def test_single_source_edges(self):
        for controller,combine,scale,lfo,seed in itertools.product(
            [128,1,10,160,161,131,132,7,64,255],range(3),[-32768,-256,-1,0,1,256,32767],
            [(-32768,32767),(0,-1),(4095,-4096)],[0,0x12345678,0xFFFFFFFF]):
            self.check_fixture([(controller,combine,scale)]+[(7,0,256)]*3,1,lfo,seed=seed)

    def test_random_sequences(self):
        rng=random.Random(21150)
        for _ in range(500):
            entries=[(rng.choice([0,1,7,10,64,128,130,131,132,160,161,162,255]),rng.randrange(3),rng.randrange(-32768,32768)) for _ in range(4)]
            self.check_fixture(entries,rng.randrange(5),(rng.randrange(-32768,32768),rng.randrange(-32768,32768)),rng.randrange(256),rng.randrange(256),rng.randrange(1<<32))

    def test_live_helper_mutations(self):
        for mode,controller,lfo,seed in itertools.product([1,2,3],[7,128],[(-32768,32767),(4095,-4096)],[0,1234,0xFFFFFFFF]):
            self.check_fixture([(controller,0,256),(160,1,-128),(161,2,32767),(7,1,0)],1 if mode==3 else 4,lfo,seed=seed,mode=mode)

    def test_unsupported_mode_is_not_claimed(self):
        voice=bytes(416);control=bytearray(18);control[:4]=bytes([128,3,1,0]);control[16]=1
        results=[native.execute(self.words,voice,control,lambda a,m,n:10000,stack_fill=x)['result'] for x in (0x5A,0x91)]
        self.assertNotEqual(*results)
        with self.assertRaisesRegex(ValueError,'unsupported combine'):model(voice,control,lambda a,m,n:10000)

    def test_native_replay_fails_closed(self):
        voice=bytes(416);control=bytearray(18);control[16]=1
        with self.assertRaisesRegex(ValueError,'unsupported'):native.execute([0xFFFFFFFF]+self.words[1:],voice,control,lambda a,m,n:0)
        with self.assertRaisesRegex(ValueError,'bound'):native.execute(self.words,voice,control,lambda a,m,n:0,max_steps=1)
        with self.assertRaisesRegex(ValueError,'unmapped'):native.execute(self.words,voice,bytes([0]*16+[5,0]),lambda a,m,n:0)


if __name__ == '__main__':unittest.main()
