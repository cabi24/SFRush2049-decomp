#!/usr/bin/env python3
"""Replay the bounded source-context investigation; publish counts, never target bytes."""
import argparse
import dataclasses
import json
from pathlib import Path
import tempfile
import time
import verify

HERE=Path(__file__).resolve().parent

def cleanup(source,body):
    start=source.index('void func_8039D05C(void)\n{');end=source.index('\ntypedef struct Color4',start)
    return source[:start]+body+source[end:]

def body(source):
    start=source.index('void func_8039D05C(void)\n{');end=source.index('\ntypedef struct Color4',start)
    return source[start:end]

def variants():
    prior=(verify.BASELINE/'group.c').read_text();final=(HERE/'group.c').read_text()
    exact=cleanup(final,body(prior))
    fields=exact.replace('(!(*node)->file || menu_item_value_get(node))','(!record->file || menu_item_value_get(node))')
    bounds=body(exact).replace('i<44','i!=44').replace('i<11','i!=11').replace('part->handle=part->auxiliary=-1;', 'part->handle=-1;part->auxiliary=-1;')
    indexed=bounds.replace('for(i=0,part=D_803B2618[player];i!=11;i++,part++) {','for(i=0;i!=11;i++) {\n                part=&D_803B2618[player][i];').replace('if(player<=0) {','if(player>0)continue;\n        {')
    yield 'prior_source',prior
    yield 'list_fields',fields
    yield 'navigation_exact',exact
    yield 'cleanup_bounds_and_stores',cleanup(exact,bounds)
    yield 'cleanup_indexed_and_continue',cleanup(exact,indexed)
    first={
        'count_both_loops':indexed.replace('s32 player,i;','s32 player,i,count;').replace('player<D_8014A108','player<(count=D_8014A108)'),
        'selected_local':indexed.replace('s32 player,i;','s32 player,i;\n    s8 selected;').replace('for(i=0;i!=44;i++) {','selected=player;\n        for(i=0;i!=44;i++) {').replace('(s8)player','selected'),
        'outer_while':indexed.replace('for(player=0;player<D_8014A108;player++) {','player=0;while(player<D_8014A108) {').replace('if(player>0)continue;', 'if(player>0) {player++;continue;}').replace('    }\n    if(D_803B28AC)', '        player++;\n    }\n    if(D_803B28AC)'),
        'separate_final_index':indexed.replace('s32 player,i;', 's32 player,i,j;').replace('for(player=0;player<D_8014A108;player++)scheduler_recv(D_803BA1D0[player]);', 'for(j=0;j<D_8014A108;j++)scheduler_recv(D_803BA1D0[j]);'),
    }
    for label,b in first.items():yield label,cleanup(exact,b)
    b=indexed.replace('s32 player,i;','s32 player,i,count;').replace('for(player=0;player<D_8014A108;player++) {','for(player=0;player<(count=D_8014A108);player++) {')
    later={
        'outer_count':b,
        'outer_count_declare':b.replace('s32 player,i,count;', 's32 count,player,i;'),
        'outer_count_select':b.replace('s32 player,i,count;', 's32 player,i,count;\n    s8 selected;').replace('for(i=0;i!=44;i++) {','selected=player;\n        for(i=0;i!=44;i++) {').replace('(s8)player','selected'),
        'player_preinit':b.replace('s32 player,i,count;', 's32 player=0,i,count;').replace('for(player=0;player<(count=D_8014A108);player++) {', 'for(;player<(count=D_8014A108);player++) {'),
        'outer_do':b.replace('for(player=0;player<(count=D_8014A108);player++) {', 'player=0; if((count=D_8014A108)>0) do {').replace('if(player>0)continue;', 'if(player>0)goto next_player;').replace('    }\n    if(D_803B28AC)', 'next_player:;\n    } while(++player<(count=D_8014A108));\n    if(D_803B28AC)'),
        'explicit_bound':b.replace('s32 player,i,count;', 's32 player,i,count,slot_count=44,part_count=11;').replace('i!=44','i!=slot_count').replace('i!=11','i!=part_count'),
        'part_separate_index':b.replace('s32 player,i,count;', 's32 player,i,count,j;').replace('for(i=0;i!=11;i++)', 'for(j=0;j!=11;j++)').replace('part=&D_803B2618[player][i]','part=&D_803B2618[player][j]'),
        'count_halfword':b.replace('s32 player,i,count;', 's32 player,i; s16 count;'),
        'selected_int':b.replace('s32 player,i,count;', 's32 player,i,count,selected;').replace('for(i=0;i!=44;i++) {','selected=(s8)player;\n        for(i=0;i!=44;i++) {').replace('[(s8)player]', '[selected]'),
        'direct_slot':b.replace('slot=&D_803AF9A8[(s8)player][i];\n            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}', 'if(D_803AF9A8[(s8)player][i].handle!=-1) {sound_call_minimal(D_803AF9A8[(s8)player][i].handle);D_803AF9A8[(s8)player][i].handle=-1;}').replace('    Slot *slot;\n',''),
        'pre_increment':b.replace('player++) {', '++player) {').replace('player++)scheduler','++player)scheduler'),
        'index_u32':b.replace('s32 player,i,count;', 's32 player,count;u32 i;'),
        'index_s16':b.replace('s32 player,i,count;', 's32 player,count;s16 i;'),
        'index_u16':b.replace('s32 player,i,count;', 's32 player,count;u16 i;'),
        'index_u32_lt':b.replace('s32 player,i,count;', 's32 player,count;u32 i;').replace('i!=44','i<44').replace('i!=11','i<11'),
        'local_bound_split':b.replace('for(player=0;player<(count=D_8014A108);player++) {', 'count=D_8014A108;for(player=0;player<count;count=D_8014A108) {').replace('if(player>0)continue;', 'if(player>0) {player++;continue;}').replace('    }\n    if(D_803B28AC)', '        player++;\n    }\n    if(D_803B28AC)'),
    }
    for label,value in later.items():yield label,cleanup(exact,value)

def reproduce():
    start=time.monotonic();verify.score.ASM_DIR=verify.ROOT/'asm/us/ovl_a';verify.score.targets();results={}
    with tempfile.TemporaryDirectory(prefix='record-research-') as td:
        root=Path(td)
        for label,source in variants():
            t=time.monotonic();directory=root/label;directory.mkdir();(directory/'group.c').write_text(source)
            (directory/'group.json').write_text((HERE/'group.json').read_text());obj=directory/'candidate.o';verify.score.compile_group(directory,obj)
            scores={n:dataclasses.asdict(verify.score.compare(obj,n,show=0)) for n in ('func_8039E200','func_8039D05C')}
            results[label]={'source_sha256':verify.sha(source.encode()),'scores':scores,'elapsed_seconds':round(time.monotonic()-t,3)}
    return {'variant_count':len(results),'distinct_source_count':len({r['source_sha256'] for r in results.values()}),
        'elapsed_seconds':round(time.monotonic()-start,3),'results':results}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path);args=parser.parse_args()
    result=json.dumps(reproduce(),indent=2)+'\n'
    if args.output:args.output.write_text(result)
    print(result)
