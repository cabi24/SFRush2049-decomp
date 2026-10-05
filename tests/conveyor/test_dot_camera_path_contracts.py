"""Contract metadata and bounded helper semantics, not matching/ROM tests."""
import ctypes
import importlib.util
import json
from pathlib import Path
import random
import shutil
import struct
import subprocess

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_camera_path_contracts'
spec=importlib.util.spec_from_file_location('camera_contract_audit',PACKET/'audit.py')
audit=importlib.util.module_from_spec(spec);spec.loader.exec_module(audit)


def test_complete_native_contracts():
    native=audit.native();saved=json.loads((PACKET/'verification.json').read_text())
    assert native==saved['native']
    h=native['func_800D348C'];p=native['stunt_combo_display']
    assert (h['bytes'],h['frame_bytes'])==(1684,200)
    assert (p['bytes'],p['frame_bytes'])==(4700,400)
    assert h['direct_callers']==[{'caller':'stunt_combo_display','site':'0x800d40a4'}]
    words=audit.score.targets();base=int(p['start'],16)
    site=(0x800d40a4-base)//4
    # s0 = car, s2 = &node, s3 = &point in the genuine parent.
    before=words['stunt_combo_display'][site-5:site]
    assert any(w>>26==0 and w&63 in (0x21,0x25) and (w>>11)&31==16 for w in before)
    assert any(w>>26==9 and (w>>21)&31==29 and (w>>16)&31==18 and w&0xffff==356 for w in before)
    assert any(w>>26==9 and (w>>21)&31==29 and (w>>16)&31==19 and w&0xffff==348 for w in before)
    tail=words['func_800D348C'][-8:]
    assert {((w>>21)&31) for w in tail if w>>26==0x2b}=={18,19}


def test_honest_geometry_and_no_claims():
    receipt=json.loads((PACKET/'verification.json').read_text())
    assert not receipt['claims']
    assert not json.loads((PACKET/'group.json').read_text())['claims']
    for family in ('natural_group','diagnostic_seed_group'):
        for row in receipt[family].values():
            assert not row['strict_match']
            assert row['elf_end_exclusive']-row['elf_start']==row['elf_bytes']
            assert row['nonzero_alignment_words']==0
            assert not row['full_body_relocations']['unresolved']
            assert not row['full_body_relocations']['errors']
    assert receipt['natural_group']['func_800D348C']['elf_bytes']==2088
    assert receipt['diagnostic_seed_group']['func_800D348C']['elf_bytes']==1672
    assert receipt['diagnostic_seed_group']['func_800D348C']['frame_bytes']==304
    for name,want in receipt['source_hashes'].items():
        assert audit.digest((PACKET/name).read_bytes())==want
    source=(PACKET/'group.c').read_text()
    for forbidden in ('M2C_ERROR','M2C_BITWISE','__standin','asm(','asm ('):
        assert forbidden not in source
    assert 'math_utility(sp68, (u8 *) arg0 + 0x6E0);' in source
    assert 'viDeadlinePassed(void)' in source


HARNESS=r'''
#include <string.h>
Header16 D_801407F0;
CameraCar D_8014A250[4];
Record80 D_80151CE8[4];
s16 D_8014A108;
s32 D_8014A110,D_8011735C;
static Position points[12],nodepoints[3][12];
static Descriptor16 nodes[3];
static s32 overlay_calls,dispatch_calls,last_overlay,dispatch_node,dispatch_point;
void func_8038CA24(s32 player) { overlay_calls++;last_overlay=player; }
s32 func_800D3430(s32 n,s32 p,s32 *outn,s32 *outp,s32 flag) {
 dispatch_calls++;dispatch_node=n;dispatch_point=p;
 *outn=2;*outp=1;return flag;
}
void reset_case(s32 mode,u32 seed,s32 count,s32 cars) {
 memset(D_8014A250,0,sizeof(D_8014A250));
 memset(D_80151CE8,0,sizeof(D_80151CE8));
 memset(nodes,0,sizeof(nodes));
 D_8014A110=mode;D_8011735C=(s32)seed;D_8014A108=cars;
 D_801407F0.count=count;D_801407F0.points=points;
 D_801407F0.paths=3;D_801407F0.descriptors=nodes;
 overlay_calls=dispatch_calls=last_overlay=dispatch_node=dispatch_point=0;
}
void set_point(s32 node,s32 i,s32 x,s32 y,s32 z) {
 Position *p=node<0?&points[i]:&nodepoints[node][i];p->x=x;p->y=y;p->z=z;
}
void set_node(s32 node,s32 count,s32 type,s32 section) {
 nodes[node].points=nodepoints[node];nodes[node].count=count;
 nodes[node].type=type;nodes[node].section=section;
}
void set_car(s32 i,s32 state,s32 player,s32 point,s32 section,s32 next,s32 x,s32 y,s32 z) {
 CameraCar *c=&D_8014A250[i];c->cameraState=state;c->player=player;c->pathPoint=point;
 c->section=section;c->nextSection=next;
 c->currentPosition[0]=x;c->currentPosition[1]=y;c->currentPosition[2]=z;
 c->position[0]=x+7;c->position[1]=y+7;c->position[2]=z+7;
 c->cameraPosition[0]=x-7;c->cameraPosition[1]=y-7;c->cameraPosition[2]=z-7;
}
void set_section(s32 section,s32 node,s32 point) {
 HALF(&D_80151CE8[section],node<0?0x2E:0x38+node*2)=point;
}
void run_case(s32 *out) {
 func_800D348C(&D_8014A250[0],out,out+1);
 out[2]=D_8011735C;out[3]=overlay_calls;out[4]=last_overlay;
 out[5]=dispatch_calls;out[6]=dispatch_node;out[7]=dispatch_point;
}
'''

@pytest.fixture(scope='module')
def helper(tmp_path_factory):
    compiler=shutil.which('cc')
    if compiler is None:pytest.skip('host C compiler unavailable')
    directory=tmp_path_factory.mktemp('camera_helper')
    source=(PACKET/'group.c').read_text().split('void camera_follow_path(s32 pathMode')[0]
    (directory/'helper.c').write_text(source+HARNESS)
    subprocess.run([compiler,'-shared','-fPIC','-O2','-fwrapv','-fsanitize=undefined',
                    '-fno-sanitize-recover=all','-o',str(directory/'helper.so'),str(directory/'helper.c')],check=True,capture_output=True)
    lib=ctypes.CDLL(str(directory/'helper.so'))
    lib.reset_case.argtypes=[ctypes.c_int,ctypes.c_uint,ctypes.c_int,ctypes.c_int]
    return lib


def f32(value):return struct.unpack('f',struct.pack('f',value))[0]

def distance(point,position,xz=False):
    d=[f32(float(a)-b) for a,b in zip(point,position)]
    if xz:return f32(f32(d[0]*d[0])+f32(d[2]*d[2]))
    return f32(f32(f32(d[0]*d[0])+f32(d[1]*d[1]))+f32(d[2]*d[2]))


def oracle(case):
    mode,seed,points,nodes,cars,sections=case
    car=cars[0];_,player,saved,current,next_,pos=car
    outnode=-1;outpoint=0;best=f32(1e20);dispatch=[]
    if mode==6:
        seed=(seed*1103515245+12345)&0xffffffff
        start=int(f32(f32(float((seed>>16)&32767)*len(points))/32768.0))
        farthest=0.0
        for step in range(len(points)):
            at=(start+step)%len(points);nearest=f32(1e20)
            for i,(state,*rest) in enumerate(cars):
                if i==player:continue
                position=rest[-1];position=[v+(-7 if state>=0 else 7) for v in position]
                nearest=min(nearest,distance(points[at],position,True))
            if farthest<nearest:outpoint=at;farthest=nearest
    elif mode==5:outpoint=saved
    else:
        for i,p in enumerate(points):
            d=distance(p,pos)
            if d<best:best=d;outpoint=i
        if mode not in (1,4,5):
            if best>1600.0:
                for n,(kind,section,ps) in enumerate(nodes):
                    for i,p in enumerate(ps):
                        d=distance(p,pos)
                        if d<best:best=d;outnode=n;outpoint=i
            if outnode>=0 and nodes[outnode][0]==0:
                dispatch=[outnode,outpoint];outnode=2;outpoint=1
            if outnode<0 or nodes[outnode][0]!=2:
                if outnode>=0:
                    low=sections[current][outnode+1];high=sections[next_][outnode+1]
                    if low>=0:
                        if high>=0:
                            if outpoint<low or outpoint>high:
                                best=f32(1e20)
                                for i in range(low,high+1):
                                    d=distance(nodes[outnode][2][i],pos)
                                    if d<best:best=d;outpoint=i
                        elif outpoint<low:outpoint=low
                    elif high<0 and current!=nodes[outnode][1]:outnode=outpoint=-1
                if outnode<0:
                    high=len(points) if next_<current else sections[next_][0]
                    low=sections[current][0]
                    if outpoint<low or outpoint>=high:
                        best=f32(1e20)
                        for i in range(low,high):
                            d=distance(points[i],pos)
                            if d<best:best=d;outpoint=i
    signed_seed=ctypes.c_int(seed).value
    return [outnode,outpoint,signed_seed,int(mode==6),player if mode==6 else 0,
            int(bool(dispatch)),*(dispatch or [0,0])]


def execute(lib,case):
    mode,seed,points,nodes,cars,sections=case
    lib.reset_case(mode,seed,len(points),len(cars))
    for i,p in enumerate(points):lib.set_point(-1,i,*p)
    for n,(kind,section,ps) in enumerate(nodes):
        lib.set_node(n,len(ps),kind,section)
        for i,p in enumerate(ps):lib.set_point(n,i,*p)
    for i,(state,player,point,section,next_,pos) in enumerate(cars):
        lib.set_car(i,state,player,point,section,next_,*pos)
    for s,values in enumerate(sections):
        for n,value in enumerate(values):lib.set_section(s,n-1,value)
    result=(ctypes.c_int*8)();lib.run_case(result);return list(result)


def test_helper_mode_and_section_semantics(helper):
    rng=random.Random(0xD348C)
    kinds=set();dispatches=0
    for iteration in range(1400):
        mode=iteration%7;count=rng.randrange(2,9)
        points=[tuple(rng.randrange(-180,181) for _ in range(3)) for _ in range(count)]
        nodes=[(rng.choice([0,1,2]),rng.randrange(3),
                [tuple(rng.randrange(-90,91) for _ in range(3)) for _ in range(6)]) for _ in range(3)]
        cars=[(rng.choice([-1,0]),i,rng.randrange(count),rng.randrange(3),rng.randrange(3),
               tuple(rng.randrange(-120,121) for _ in range(3))) for i in range(rng.randrange(1,5))]
        sections=[[rng.randrange(count+1)]+[rng.randrange(-1,6) for _ in range(3)] for _ in range(3)]
        case=(mode,rng.randrange(2**32),points,nodes,cars,sections)
        expected=oracle(case);got=execute(helper,case)
        assert got==expected,(iteration,case,expected,got)
        kinds.add((mode,got[0]>=0));dispatches+=got[5]
    assert len(kinds)>=10 and dispatches>20


def test_helper_ties_empty_scan_and_single_car(helper):
    nodes=[(2,0,[(0,0,0)]*3)]*3
    for mode in (1,4,5,6):
        for points in ([],[(0,0,0)]*4):
            case=(mode,0xdeadbeef,points,nodes,[(0,0,2,0,1,(0,0,0))],[[0,0,0,0]]*3)
            assert execute(helper,case)==oracle(case)
