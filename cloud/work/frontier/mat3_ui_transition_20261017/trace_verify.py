"""Optional stock/trace compiler equivalence and procedure-scoped pool evidence.

Set IDO_TRACE_DIR to already-built workbench uopt/ugen/as1 instruments. This
script never builds/patches compilers, persists objects, or emits raw assembly.
"""
import json
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile
import verify

sys.path.insert(0, str(verify.ROOT/'third_party/n64-decomp-workbench/src'))
from decomp_workbench.ucode import parse_ucode
from decomp_workbench.procedure_identity import procedure_names


def trace_group(group, directory, tracer):
    score = verify.score
    stock = directory/'stock.o'
    traced = directory/'trace.o'
    score.compile_group(group, stock)
    run = score._run
    logs = {}
    names = ()
    def instrument(cmd, **kw):
        nonlocal names
        cmd = list(cmd)
        stage = Path(cmd[0]).name
        if stage in ('uopt', 'ugen', 'as1'):
            cmd[0] = str(tracer/stage)
        if stage == 'ugen':
            kw['env'] = dict(os.environ, DKWB_UGEN_TRACE='1')
        if stage == 'as1':
            names = procedure_names(parse_ucode((Path(kw['cwd'])/'opt').read_bytes()))
        result = run(cmd, **kw)
        if stage == 'ugen':
            logs['ugen'] = result.stderr
        return result
    score._run = instrument
    try:
        score.compile_group(group, traced)
    finally:
        score._run = run
    verify.require(stock.read_bytes() == traced.read_bytes(), 'trace compiler changed complete object')
    proc = names.index(verify.NAME)
    pattern = r'DKWB-FREELIST (REMOVE|ALLOC_GP_RESULT) proc=%d reg=(\d+) emitted=(\d+) line=(\d+)'%proc
    records = [(event, int(reg), int(emitted)) for event,reg,emitted,_ in re.findall(pattern, logs['ugen'])]
    return {'complete_object_identical': True,
            'initial_removed_registers': [r for e,r,n in records if e == 'REMOVE' and n == 1],
            'allocated_registers': [r for e,r,n in records if e == 'ALLOC_GP_RESULT']}


def main():
    tracer = Path(os.environ['IDO_TRACE_DIR']).resolve()
    for name in ('uopt', 'ugen', 'as1'):
        verify.require((tracer/name).is_file(), 'missing trace executable '+name)
    with tempfile.TemporaryDirectory(prefix='rush-ui-transition-trace-') as temp:
        d = Path(temp)
        result = {'genuine_internal_allocator': trace_group(verify.GROUP, d, tracer)}
        external = d/'external'
        shutil.copytree(verify.GROUP, external)
        spec = json.loads((external/'group.json').read_text())
        old = json.loads((verify.ROOT/'cloud/work/frontier/superseded/frontier_slot18_alloc/group.json').read_text())
        removed = set(old['members'] + old['context'])
        spec['files'] = [f for f in spec['files'] if f not in old['files']]
        for key in ('members', 'claims', 'keep'):
            spec[key] = [n for n in spec[key] if n not in removed]
        (external/'group.json').write_text(json.dumps(spec, indent=2)+'\n')
        result['external_allocator_control'] = trace_group(external, d, tracer)
        verify.require(result['genuine_internal_allocator']['initial_removed_registers'] == [2,3,8,9,10,11,12,13], 'internal allocator pool reservation')
        verify.require(result['external_allocator_control']['initial_removed_registers'] == [2,3], 'external allocator pool reservation')
        print(json.dumps(result, indent=2))

if __name__ == '__main__':
    main()
