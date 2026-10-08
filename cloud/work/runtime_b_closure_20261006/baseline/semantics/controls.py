#!/usr/bin/env python3
"""In-memory verifier negative controls; no compiler or persisted code mutation."""
import argparse
import json
from pathlib import Path
import sys
sys.dont_write_bytecode=True
from adapter import native
from closure import Machine, ENTRY, MEMBERS, PLAYERS, bits
from verify import verify, run_frames


def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    args=p.parse_args();code,start,data,meta=native(args.reference_root);results=[]
    def refusal(name,action,needle):
        try:action()
        except (AssertionError,ZeroDivisionError) as exc:
            assert needle in str(exc),(name,str(exc));results.append({'name':name,'result':'rejected','reason':str(exc)})
        else:raise AssertionError(('control not rejected',name))
    missing=dict(code);del missing[0x8038e114]
    refusal('missing_genuine_internal_child',lambda:Machine(missing,start,data,{'count':0}).run(),'private child hook forbidden')
    unknown=dict(code);unknown[start]=0xffffffff
    refusal('unsupported_instruction',lambda:Machine(unknown,start,data,{'count':0}).run(),'unsupported opcode')
    root=dict(code)
    # Remove the genuine root epilogue s0 reload in memory only. The native words
    # and altered words never leave memory or enter a source/assembly artifact.
    loads=[pc for pc,w in root.items() if ENTRY<=pc<ENTRY+MEMBERS['FCE0'] and w>>26==35 and (w>>21)&31==29 and (w>>16)&31==16]
    assert len(loads)==1;root[loads[0]]=0
    refusal('root_preserved_register_corruption',lambda:Machine(root,start,data,{'pressed':0}).run(),'root nonvolatile GPR preservation')
    refusal('readonly_memory_write',lambda:Machine(code,start,data,{}).put(0x80394358,0),'unmapped/readonly write')
    refusal('unmapped_memory_read',lambda:Machine(code,start,data,{}).get(0x60000000),'unmapped read')
    refusal('F938_owner_outside_intersection',lambda:Machine(code,start,data,{'owner':4}),'owner intersection')
    refusal('D498_zero_distance_domain',lambda:Machine(code,start,data,{'count':2,'pressed':0,
        'players':[{}, {'position':(0.,0.,0.)}],'records':[{'kind':7}]}).run(),'division by zero')
    m=Machine(code,start,data,{})
    refusal('invalid_quad_pointer_lifecycle',lambda:(m.r.__setitem__(4,0x12340000),m.call(0x8008d0c0)),'not a live quad pointer')
    # Pairwise comparison must detect persistent bytes outside named fields.
    a=Machine(code,start,data,{'count':0});b=Machine(code,start,data,{'count':0})
    b.put(PLAYERS+0x100,b.get(PLAYERS+0x100)^1)
    assert run_frames(a,{'count':0})!=run_frames(b,{'count':0})
    results.append({'name':'full_nonstack_sentinel_mutation','result':'detected'})
    # An extra real external-call event must not disappear in final-state checks.
    c=Machine(code,start,data,{'count':0});d=Machine(code,start,data,{'count':0})
    ca=run_frames(c,{'count':0});da=run_frames(d,{'count':0});d.snapshot('frame_tail')
    assert ca!=da
    results.append({'name':'ordered_external_trace_mutation','result':'detected'})
    result={'status':'PASS','controls':results,'compiler_invocations':0,'persistent_native_bytes':False}
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))

if __name__=='__main__':main()
