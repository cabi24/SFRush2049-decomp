#!/usr/bin/env python3
"""Replay bounded loop/source hypotheses; retain scores, never target bytes."""
import dataclasses
import json
from pathlib import Path
import tempfile
import verify

def variants():
    s=(verify.SOURCE/'group.c').read_text()
    a=s.index('void func_8039D05C(void)\n{');z=s.index('\ntypedef struct Color4',a);b=s[a:z]
    all_variants={}
    vs={
    'base':b,
    'guard_le':b.replace('if(player>0)continue;\n        {','if(player<=0) {'),
    'guard_zero':b.replace('if(player>0)continue;\n        {','if(player==0) {'),
    'box_index_player':b.replace('for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {','for(player=0;player<4;player++) {\n        box=&D_803B9FC0[player];'),
    'box_index_i':b.replace('for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {','for(i=0;i<4;i++) {\n        box=&D_803B9FC0[i];'),
    'box_direct_player':b.replace('for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {\n        if(*box) {sound_handles_array_clear(*box);*box=0;}','for(player=0;player<4;player++) {\n        if(D_803B9FC0[player]) {sound_handles_array_clear(D_803B9FC0[player]);D_803B9FC0[player]=0;}'),
    'player_u32':b.replace('s32 player,i,count;','u32 player;s32 i,count;'),
    'player_s16':b.replace('s32 player,i,count;','s16 player;s32 i,count;'),
    'player_s8':b.replace('s32 player,i,count;','s8 player;s32 i,count;'),
    'outer_unsigned_bound':b.replace('player<(count=D_8014A108)','(u32)player<(count=D_8014A108)'),
    'for_condition_reverse':b.replace('player<(count=D_8014A108)','(count=D_8014A108)>player'),
    'outer_exit_break':b.replace('for(player=0;player<(count=D_8014A108);player++) {','for(player=0;;player++) {\n        if(player>=(count=D_8014A108))break;'),
    'outer_exit_goto':b.replace('for(player=0;player<(count=D_8014A108);player++) {','for(player=0;;player++) {\n        if(player>=(count=D_8014A108))goto cleanup_done;').replace('    if(D_803B28AC)', 'cleanup_done:\n    if(D_803B28AC)'),
    'guard_else_continue':b.replace('if(player>0)continue;\n        {','if(player<=0) {').replace('    }\n    if(D_803B28AC)', '        else continue;\n    }\n    if(D_803B28AC)'),
    }
    all_variants.update(vs)
    vs={
    'register_player':b.replace('s32 player,i,count;','register s32 player; s32 i,count;'),
    'outer_for_noinit':b.replace('for(player=0;player<(count=D_8014A108);player++) {','player=0; for(;player<(count=D_8014A108);player++) {'),
    'outer_posttest':b.replace('for(player=0;player<(count=D_8014A108);player++) {','player=0; if ((count=D_8014A108)>0) do {').replace('    }\n    if(D_803B28AC)','    } while (++player<(count=D_8014A108));\n    if(D_803B28AC)'),
    'outer_posttest_index':b.replace('for(player=0;player<(count=D_8014A108);player++) {','player=0; if ((count=D_8014A108)>0) do {').replace('if(player>0)continue;','if(player>0)goto next_player;').replace('    }\n    if(D_803B28AC)','next_player: player++;\n    } while (player<(count=D_8014A108));\n    if(D_803B28AC)'),
    'inner_posttest':b.replace('for(i=0;i!=44;i++) {','i=0; do {').replace('        D_803B9B90', '        D_803B9B90').replace('slot->handle=-1;}\n        }','slot->handle=-1;}\n        } while(++i!=44);').replace('for(i=0;i!=11;i++) {','i=0; do {').replace('part->auxiliary=-1;}\n            }','part->auxiliary=-1;}\n            } while(++i!=11);'),
    'inner_pointer_end':b.replace('for(i=0;i!=44;i++) {\n            slot=&D_803AF9A8[(s8)player][i];','for(slot=D_803AF9A8[(s8)player]; slot != D_803AF9A8[(s8)player]+44; slot++) {'),
    'panel_pointer_end':b.replace('for(i=0;i!=11;i++) {\n                part=&D_803B2618[player][i];','for(part=D_803B2618[player]; part != D_803B2618[player]+11; part++) {'),
    'player_guard_do':b.replace('for(player=0;player<(count=D_8014A108);player++) {','for(player=0;player<(count=D_8014A108);player++) { do {').replace('if(player>0)continue;', 'if(player>0)break;').replace('    }\n    if(D_803B28AC)','    } while(0); }\n    if(D_803B28AC)'),
    'player_computed_index':b.replace('slot=&D_803AF9A8[(s8)player][i];','slot=&D_803AF9A8[0][((s8)player)*44+i];'),
    'panel_computed_index':b.replace('part=&D_803B2618[player][i];','part=&D_803B2618[0][player*11+i];'),
    'player_increment_nested':b.replace('for(player=0;player<(count=D_8014A108);player++) {','for(player=0;player<(count=D_8014A108);) {').replace('if(player>0)continue;', 'if(player>0) {player++;continue;}').replace('    }\n    if(D_803B28AC)','        player++;\n    }\n    if(D_803B28AC)'),
    }
    all_variants.update(vs)
    slot=b.replace('for(i=0;i!=44;i++) {','for(i=0;i!=sizeof(D_803AF9A8[0]);i+=sizeof(Slot)) {').replace('slot=&D_803AF9A8[(s8)player][i];','slot=(Slot*)((u8*)D_803AF9A8[(s8)player]+i);')
    part=b.replace('for(i=0;i!=11;i++) {','for(i=0;i!=sizeof(D_803B2618[0]);i+=sizeof(PanelPart)) {').replace('part=&D_803B2618[player][i];','part=(PanelPart*)((u8*)D_803B2618[player]+i);')
    vs={'slot_bytes':slot,'panel_bytes':part,'both_bytes':slot.replace('for(i=0;i!=11;i++) {','for(i=0;i!=sizeof(D_803B2618[0]);i+=sizeof(PanelPart)) {').replace('part=&D_803B2618[player][i];','part=(PanelPart*)((u8*)D_803B2618[player]+i);')}
    for key in list(vs):
     vs[key+'_signed']=vs[key].replace('i!=sizeof(', 'i!=(s32)sizeof(').replace('i+=sizeof(','i+=(s32)sizeof(')
    all_variants.update(vs)
    return {k:s[:a]+v+s[z:] for k,v in all_variants.items()}

def reproduce():
    verify.base.score.ASM_DIR=verify.ROOT/'asm/us/ovl_a';result={}
    with tempfile.TemporaryDirectory(prefix='cleanup-research-') as td:
        for name,source in variants().items():
            d=Path(td)/name;d.mkdir();(d/'group.c').write_text(source);(d/'group.json').write_text((verify.SOURCE/'group.json').read_text())
            obj=d/'candidate.o';verify.base.score.compile_group(d,obj)
            result[name]={'source_sha256':verify.base.sha(source.encode()),'score':dataclasses.asdict(verify.base.score.compare(obj,verify.NAME,show=0))}
    return {'results':result,'variant_count':len(result),'best_differing':min(r['score']['differing'] for r in result.values()),'retained_source_changed':False}

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path);a=p.parse_args();s=json.dumps(reproduce(),indent=2)+'\n'
    if a.output:a.output.write_text(s)
    print(s)
