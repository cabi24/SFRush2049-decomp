#!/usr/bin/env python3
"""Replay all 20 bounded directed controls; source names identify one-change hypotheses."""
import hashlib,json,sys,tempfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
from verify import record
FAMILY={
'nested_success':'condition topology','assign_condition':'condition topology','flag_scoped':'condition topology','deferred_return':'condition topology','initial_return':'condition topology',
'masked_index':'pointer/index topology','byte_offset':'pointer/index topology','masked_byte_offset':'pointer/index topology','copy_through_request_pointer':'pointer/index topology',
'context_inner':'declaration scope','options_inner':'declaration scope',
'signed_results':'integer representation','stream_word':'integer representation','index_int':'integer representation','unsigned_mask':'integer representation',
'identifier_local':'identifier-only negative control','expanded':'statement layout/store order','pending_first':'statement layout/store order','library_memcpy':'aggregate copy form','result_local':'result-value carrier'}
def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';expected=json.loads((P/'variants.json').read_text());rows=[]
    assert set(FAMILY)=={r['variant'] for r in expected} and len(expected)==20
    with tempfile.TemporaryDirectory(prefix='sequence-controls-') as tmp:
        for row in expected:
            name=row['variant'];source=P/'controls'/('func_80019490_'+name+'.c');obj=Path(tmp)/(name+'.o')
            score.compile_single(source,score.DEFAULT_FLAGS,obj);actual=record(obj,'func_80019490')
            assert (actual['differing_words'],actual['elf_function_bytes'],actual['extra_nonzero_words'])==(row['differing'],row['size'],row['extra']),name
            assert not actual['strict_match']
            actual.update(variant=name,hypothesis=FAMILY[name],source=str(source.relative_to(ROOT)),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),flags=score.DEFAULT_FLAGS)
            rows.append(actual)
    counts={f:list(FAMILY.values()).count(f) for f in sorted(set(FAMILY.values()))}
    return {'result':'PASS','initial_source_forms':1,'directed_controls':20,'total_distinct_source_forms':21,'per_hypothesis':counts,'not_additional_source_forms':'Final retains byte_offset with explanatory comments only. O2/O1 replays of baseline/final are separate compiler measurements.','controls':rows}
if __name__=='__main__':print(json.dumps(run(),indent=2))
