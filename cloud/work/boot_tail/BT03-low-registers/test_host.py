#!/usr/bin/env python3
"""Host behavior controls, separate from IDO/MIPS byte identity."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
COMMON = r'''
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
static unsigned int seed = 0x12345678;
static unsigned int rnd(void) { seed = seed * 1664525u + 1013904223u; return seed; }
'''
H1558 = r'''
u8 D_8002C630;
short D_80038390;
ResourceHeader *D_80038398[8];
static int locks, unlocks, calls, guarded;
static void *seen[5];
void func_80014594(void) { locks++; guarded++; }
void func_800145DC(void) { unlocks++; guarded--; }
int func_800178B0(void *a, void *b, Program *c, void *d, void *e) {
    seen[0]=a; seen[1]=b; seen[2]=c; seen[3]=d; seen[4]=e; calls++;
    return 0x12345678;
}
typedef union { unsigned int align; unsigned char bytes[1200]; } Blob;
static Blob blobs[5];
int main(void) {
    int test, b, j, n, expected, hit, unlocked;
    int success=0, guarded_hits=0, direct_hits=0;
    u16 id, key;
    ResourceHeader *h;
    Program *program, *selected;
    void *stream, *options;
    for (test=0; test<800; test++) {
        memset(blobs, 0, sizeof(blobs));
        D_8002C630=(u8)(test % 7 != 0);
        D_80038390=(short)(test % 6);
        selected=0; hit=-1;
        id=(u16)(rnd()%5); key=(u16)(rnd()%7); unlocked=(test&1)?255:0;
        stream=&blobs[0]; options=(test&2)?(void *)&blobs[1]:0;
        for (b=0; b<5; b++) {
            h=(ResourceHeader *)blobs[b].bytes;
            h->id=(u16)(rnd()%5); h->type=(u16)(rnd()%4 == 0);
            h->bank_a=40; h->bank_b=64; h->programs=96;
            D_80038398[b]=h;
            program=(Program *)(blobs[b].bytes+104); n=(int)(rnd()%6);
            for(j=0;j<n;j++) program[j].id=(u16)(rnd()%7);
            program[n].id=0xffff;
        }
        if (D_8002C630) {
            for (b=0;b<D_80038390;b++) {
                h=D_80038398[b];
                if (h->id==id) {
                    if (!h->type) {
                        program=(Program *)((u8 *)h+h->programs+8);
                        for (j=0;program[j].id!=0xffff;j++) {
                            if (program[j].id==key) { selected=&program[j];hit=b;break; }
                        }
                    }
                    break;
                }
            }
        }
        locks=unlocks=calls=guarded=0; memset(seen,0,sizeof(seen));
        expected=selected?0x12345678:-1;
        assert(func_8001558C(id,key,stream,options,(u8)unlocked)==expected);
        assert(calls==(selected!=0));
        assert(locks==(selected && !unlocked)); assert(unlocks==locks); assert(!guarded);
        if (selected) {
            success++;if(unlocked)direct_hits++;else guarded_hits++;
            h=D_80038398[hit];
            assert(seen[0]==(u8 *)h+h->bank_a+8);
            assert(seen[1]==(u8 *)h+h->bank_b+8);
            assert(seen[2]==selected && seen[3]==stream && seen[4]==options);
        }
    }
    assert(success>0 && guarded_hits>0 && direct_hits>0);
    puts("8001558C: 800 branch, sentinel, first-bank, five-argument and guard controls passed");
    return 0;
}
'''
H15C0 = r'''
int D_8003CE18;
RecordStorage D_8003CE20[32];
static int locks, unlocks;
void func_80014594(void) { locks++; }
void func_800145DC(void) { unlocks++; }
int main(void) {
    RecordStorage expected[32];
    int test,i,n,ret,expect;
    int removals=0,kept=0;
    u16 id;
    RecordFields *f;
    for(test=0;test<800;test++) {
        for(i=0;i<32;i++) {
            f=(RecordFields *)&D_8003CE20[i];
            f->payload=rnd();f->id=(u16)(rnd()%23);f->type=(u16)rnd();
            f->references=(u16)(rnd()%5);f->metadata=(u16)rnd();
        }
        D_8003CE18=(int)(rnd()%24);n=D_8003CE18;id=(u16)(rnd()%29);
        memcpy(expected,D_8003CE20,sizeof(expected));expect=0;
        for(i=0;i<n;i++) {
            f=(RecordFields *)&expected[i];
            if(f->id==id) {
                f->references=(u16)(f->references-1);
                if(f->references==0) {
                    memmove(&expected[i],&expected[i+1],(n-i-1)*sizeof(expected[0]));
                    n--;expect=1;
                }
                break;
            }
        }
        locks=unlocks=0;ret=func_80015C0C(id);
        if(ret)removals++;else kept++;
        assert(ret==expect && D_8003CE18==n && locks==1 && unlocks==1);
        assert(!memcmp(expected,D_8003CE20,sizeof(expected)));
    }
    assert(removals>0 && kept>0);
    puts("80015C0C: 800 absent, decrement-wrap, compaction and guard controls passed");
    return 0;
}
'''
H163A = r'''
u32 D_800385A0;
RegistryEntry D_800385A8[8];
static Resource storage[8][9];
static void *released[72];
static u32 released_sizes[72];
static int release_count, remove_count;
static Resource *removed;
void func_80014D08(void *data, u32 size) {
    released[release_count]=data;released_sizes[release_count++]=size;
}
int func_800161A0(Resource *r) { removed=r;remove_count++;return 1; }
int main(void) {
    Resource expected[8][9];
    int test,b,j,count,found,used,result,release_expected,remove_expected;
    int rb[72],rj[72],remove_bank;
    int releases=0,removals=0,misses=0;
    u16 id;
    for(test=0;test<800;test++) {
        memset(storage,0,sizeof(storage));
        D_800385A0=(rnd()%7);id=(u16)(rnd()%9);
        for(b=0;b<8;b++) {
            D_800385A8[b].resources=storage[b];count=(int)(rnd()%8);
            for(j=0;j<count;j++) {
                storage[b][j].id=(u16)(rnd()%9);
                storage[b][j].references=(u16)(rnd()%4);
                storage[b][j].size=rnd();storage[b][j].metadata=rnd();
            }
            storage[b][count].id=0xffff;
        }
        if(test%5<4) {
            D_800385A0=1;
            storage[0][0].id=id;
            storage[0][0].references=(test%5==1)?0:1;
            storage[0][1].id=0xffff;
            if(test%5>=2) {
                storage[0][1].id=(test%5==2)?id:(u16)(id+1);
                storage[0][1].references=1;
                storage[0][2].id=0xffff;
            }
        }
        memcpy(expected,storage,sizeof(storage));
        result=release_expected=remove_expected=0;remove_bank=-1;
        for(b=0;b<(int)D_800385A0;b++) {
            found=used=0;
            for(j=0;expected[b][j].id!=0xffff;j++) {
                if(expected[b][j].id==id) {
                    found=1;expected[b][j].references=(u16)(expected[b][j].references-1);
                    if(!expected[b][j].references) {
                        rb[release_expected]=b;rj[release_expected++]=j;
                    }
                }
                used|=expected[b][j].references!=0;
            }
            if(found) { result=1;if(!used){remove_expected=1;remove_bank=b;}break; }
        }
        release_count=remove_count=0;removed=0;
        assert(func_800163A8(id)==result);
        assert(!memcmp(storage,expected,sizeof(storage)));
        releases+=release_count;removals+=remove_count;misses+=!result;
        assert(release_count==release_expected && remove_count==remove_expected);
        for(j=0;j<release_count;j++) {
            assert(released[j]==storage[rb[j]][rj[j]].data);
            assert(released_sizes[j]==storage[rb[j]][rj[j]].size);
        }
        if(remove_expected)assert(removed==storage[remove_bank]);
    }
    assert(releases>0 && removals>0 && misses>0);
    puts("800163A8: 800 duplicate-key, sentinel, wrap, callback and entry-removal controls passed");
    return 0;
}
'''
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    rows=[]
    with tempfile.TemporaryDirectory(prefix='bt03-register-host-') as tmp:
        tmp=Path(tmp)
        for addr,harness in [('8001558C',H1558),('80015C0C',H15C0),('800163A8',H163A)]:
            src=(ROOT/'cloud/matches/boot_tail/func_800163A8.c') if addr=='800163A8' else HERE/('func_'+addr+'_NONMATCH.c')
            body=COMMON+src.read_text()+harness
            c=tmp/(addr+'.c');exe=tmp/addr;c.write_text(body)
            subprocess.run(['cc','-std=c89','-O2','-Wall','-Wextra','-Werror',str(c),'-o',str(exe)],check=True)
            result=subprocess.check_output([str(exe)],text=True).strip()
            rows.append(dict(function='func_'+addr,cases=800,result=result,source_sha256=hashlib.sha256(src.read_bytes()).hexdigest()))
    report={'schema_version':1,'result':'PASS','cases':2400,'scope':'Host semantic controls, not MIPS byte identity or cartridge coverage','results':rows}
    rendered=json.dumps(report,indent=2)+'\n'
    if args.output:args.output.write_text(rendered)
    else:print(rendered,end='')
if __name__=='__main__':main()
