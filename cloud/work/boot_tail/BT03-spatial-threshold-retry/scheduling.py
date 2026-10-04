#!/usr/bin/env python3
"""Source-free stock-scheduler receipt; generated objects and traces are temporary."""
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / 'third_party/n64-decomp-workbench/src'))
from tools.cloud import score
from decomp_workbench.as1_reorganize import parse_as1_reorganize_trace

NAME = 'func_8001DC08'


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    target = score.targets()[NAME]
    rows = []
    with tempfile.TemporaryDirectory(prefix='dc08-schedule-') as temp:
        temp = Path(temp)
        def canonical_object(words, stem):
            source = temp / (stem+'.s')
            obj = temp / (stem+'.o')
            source.write_text('.text\n.globl '+NAME+'\n.type '+NAME+', @function\n'+NAME+':\n'+
                              ''.join('.word 0x%08x\n'%w for w in words)+
                              '.size '+NAME+', .-'+NAME+'\n')
            subprocess.run(['mips-linux-gnu-as', '-mips2', '-o', str(obj), str(source)], check=True)
            return obj
        target_obj = canonical_object(target, 'target')
        paths = [ROOT / 'cloud/work/boot_tail/BT03-high-spatial-control/nonmatch/func_8001DC08.c',
                 WORK / 'nonmatch/func_8001DC08.c']
        for index, source in enumerate(paths):
            obj = temp / ('source%d.o'%index)
            trace_obj = temp / ('trace%d.o'%index)
            score.compile_single(source, score.DEFAULT_FLAGS, obj)
            r = subprocess.run([score.ido('cc'), '-c', *score.DEFAULT_FLAGS.split(), score.R4300_CC,
                                '-Wa,-R', '-o', str(trace_obj), str(source)], capture_output=True,
                               text=True, check=True)
            assert obj.read_bytes() == trace_obj.read_bytes()
            trace = r.stdout + r.stderr
            selections, _, _ = parse_as1_reorganize_trace(trace)
            words = score.text_words(obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, len(words)*4, score.image_symbols())
            assert not (masks or unresolved or unverified or errors) and not any(relocated[118:])
            relocated = relocated[:118]
            assert sorted(relocated) == sorted(target)
            candidate_obj = canonical_object(relocated, 'relocated%d'%index)
            report = subprocess.run([sys.executable, str(ROOT / 'tools/workbench.py'), 'diagnose',
                                     str(target_obj), str(candidate_obj), '--function', NAME,
                                     '--objdump', os.environ.get('MIPS_OBJDUMP', 'mips-linux-gnu-objdump')],
                                    capture_output=True, text=True, check=True)
            verdict = re.search(r'^verdict=([^ ]+)', report.stdout, re.M).group(1)
            assert verdict == 'schedule-mismatch'
            rows.append(dict(source_path=str(source.relative_to(ROOT)),
                             source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                             stock_trace_whole_object_identical=True,
                             canonical_relocated_workbench_verdict=verdict,
                             exact_instruction_multiset=True,
                             differing_offsets=[i*4 for i, (a,b) in enumerate(zip(target, relocated)) if a!=b],
                             trace_selection_count=len(selections),
                             parser_deciding_keys=dict(sorted(Counter(s.tie for s in selections).items())),
                             preheader_decisions=[dict(ordinal=s.ordinal, block_offset=s.addr,
                                                       source_line=s.winner.lineno, parser_key=s.tie)
                                                  for s in selections if s.block==3 and s.winner]))
    callers = []
    for name, words in score.targets().items():
        if any(w>>26==3 and 0x80000000|((w&0x03ffffff)<<2)==0x8001DC08 for w in words):
            callers.append(name)
    assert callers == ['func_8001DDE0']
    return dict(result='PASS', rows=rows, direct_boot_tail_callers=callers,
                note='Parser keys are heuristic: disagreement labels are retained, not hidden. '
                     'The whole-object trace gate and token-identical measured 3-to-2 change are direct evidence. '
                     'No native words, disassembly, traces or objects are included in this receipt.')


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
