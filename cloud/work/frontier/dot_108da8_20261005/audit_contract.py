#!/usr/bin/env python3
"""Authenticate archived count references and an independent call/store-free loop.

Only metadata is emitted. The archived linear-reference census is not a proof
that no indirect/DMA/asynchronous writer exists. Native loads are evidence for,
not literal recovery of, the original volatile source qualifier.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
COUNT=0x801543ca

def sha(data):return hashlib.sha256(data).hexdigest()
def audit():
    targets=score.targets();symbols=score.image_symbols()
    refs=[r for r in json.loads((ROOT/'cloud/work/frontier/type_model/refs.json').read_text()) if r['addr']==COUNT]
    native_hashes={}
    for r in refs:
        base=symbols[r['f']];words=targets[r['f']];offset=r['pc']-base
        assert 0<=offset<4*len(words) and offset%4==0
        w=words[offset//4]
        assert w>>26=={'form':9,'load':33,'store':41}[r['kind']]
        if r['kind']=='form':assert w&65535==COUNT&65535
        else:assert r['off']==0 and r['base']==COUNT and w&65535==0
        native_hashes[r['f']]=sha(struct.pack('>%dI'%len(words),*words))
    # Independent loop: the taken backwards edge observes the count again.
    fn='func_800EB90C';base=symbols[fn];words=targets[fn]
    def word(pc):return words[(pc-base)//4]
    load=word(0x800eb9fc)
    assert load>>26==33 and load>>21&31==4 and load&65535==0
    form=word(0x800eb9ac)
    assert form>>26==9 and form>>21&31==4 and form>>16&31==4 and form&65535==COUNT&65535
    edge=word(0x800eb9f8)
    assert edge>>26==1 and edge>>16&31==2 # BLTZL
    displacement=struct.unpack('>h',struct.pack('>H',edge&65535))[0]
    assert 0x800eb9f8+4+4*displacement==0x800eb9bc
    # Loop-path words exclude the branch-likely delay slot that stores only
    # when exiting; checking that opposite edge is part of the witness.
    exit_branch=word(0x800eb9ec)
    assert exit_branch>>26==21 and word(0x800eb9f0)>>26==40
    path=list(range(0x800eb9bc,0x800eb9f0,4))+[0x800eb9f4,0x800eb9f8,0x800eb9fc]
    for pc in path:
        w=word(pc);op=w>>26
        assert op not in (3,40,41,42,43,46,57,61)
        assert not (op==0 and w&63==9)
        # The formed pointer survives the complete cycle.
        if op in (9,15,32,33,35):assert w>>16&31!=4
        if op==0:assert w>>11&31!=4 or (w&63) in (24,25,26,27)
    lock=json.loads((ROOT/'blob_matched.lock.json').read_text())['func_800EC914']
    accepted=ROOT/lock['source'];assert sha(accepted.read_bytes())==lock['source_sha256']
    assert 'extern volatile s16 D_801543CA;' in accepted.read_text()
    widths={'load':sum(r['kind']=='load' for r in refs),'store':sum(r['kind']=='store' for r in refs),'form':sum(r['kind']=='form' for r in refs)}
    return {'global':'D_801543CA','signed_width_bits':16,'reference_counts':widths,
            'referencing_functions':sorted(native_hashes),'native_function_sha256':native_hashes,
            'writer_sites':[{'function':r['f'],'address':hex(r['pc'])} for r in refs if r['kind']=='store'],
            'writers_interpretation':'Four native halfword stores across initialization/setup routines. No asynchronous/device writer established.',
            'independent_read_semantics':{'function':fn,'first_load':'0x800EB9B8','loop_reload':'0x800EB9FC',
                'backedge':'0x800EB9F8','backedge_destination':'0x800EB9BC',
                'loop_cycle_has_calls':False,'loop_cycle_has_stores':False,
                'opposite_exit_edge_store':'0x800EB9F0','qualifier_inference':'Corroborates a repeatedly observable count read independent of 108DA8 compilation; it is not original source text.'},
            'accepted_declaration':{'path':lock['source'],'source_sha256':lock['source_sha256'],'verified':lock['verified'],'verified_at':lock['verified_at']},
            'shared_generated_header':'include/game_globals.h declares ordinary s16; it is generated width evidence, not authoritative qualifier provenance.',
            'limit':'Archived linear-reference census revalidated against current protected words. It may miss indirect, DMA or asynchronous writers.'}
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    data=audit();a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data,indent=2))
