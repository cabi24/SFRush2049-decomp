#!/usr/bin/env python3
"""Bounded SDK packet-scope controls; preserve compiler, target and scorer.

Objects, listings and linked bytes stay under ignored build/. Results are
source and scalar/hash research, not accepted matching submissions.
"""
import contextlib
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
BUILD = ROOT / 'build/87110_neighbors'
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


replay = module('neighbor_replay', ROOT / 'cloud/work/texture_rect_verification/replay.py')
fresh = module('neighbor_fresh', ROOT / 'cloud/work/texture_rect_fresh_behavior/verify_behavior.py')
FLAGS = replay.FLAGS
BASELINE = ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c'
GROUP = ROOT / 'src/blob/groups/gfx_modes'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def comparison(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        result = score.compare(obj, name, show=0)
    return {key: getattr(result, key) for key in
            ('differing', 'total', 'unresolved', 'unverified', 'errors', 'extra_words')}


def normalized_listing(text):
    """Diagnostic fingerprint only; never supplied to a compiler/assembler."""
    return '\n'.join(line for line in text.splitlines()
        if line.strip() and not re.match(r'\s*(?:\.(?:loc|file)\b|#)', line))


def expected_source(baseline, neighbor, arms):
    """Reconstruct exactly the predeclared SDK-call decomposition."""
    start = neighbor.index('#define gDPLoadTileGeneric(')
    end = neighbor.index('\n#define ', start + 1)
    macro = neighbor[start:end] + '\n#define G_TEXRECT 0xE4\n'
    lines = baseline.splitlines(True)
    calls = [i for i, line in enumerate(lines)
             if line.lstrip().startswith('gSPTextureRectangle(')]
    assert len(calls) == 8
    for arm in arms:
        i = calls[arm]
        args = lines[i].strip()[len('gSPTextureRectangle('):-2].split(',')
        assert len(args) == 10
        pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy = args
        lines[i] = (
            f'            gDPLoadTileGeneric({pkt},G_TEXRECT,{tile},{xh},{yh},{xl},{yl});\n'
            f'            gImmp1({pkt},0xE1,(_SHIFTL({s},16,16)|_SHIFTL({t},0,16)));\n'
            f'            gImmp1({pkt},0xF1,(_SHIFTL({dsdx},16,16)|_SHIFTL({dtdy},0,16)));\n')
    return ''.join(lines).replace('void func_80087110(', macro+'void func_80087110(')


def validate_sources():
    baseline = BASELINE.read_text()
    neighbor = (GROUP/'object_render.c').read_text()
    for name, arms in [('sdk_sibling_both', [4]), ('sdk_sibling_all', range(8))]:
        assert (HERE/(name+'.c')).read_text() == expected_source(baseline, neighbor, arms), name


def main():
    validate_sources()
    BUILD.mkdir(parents=True, exist_ok=True)
    ido = Path(os.environ.get('IDO_DIR', ROOT/'tools/cloud/ido')).resolve()
    result = {'accepted': False, 'flags': FLAGS,
              'baseline_source_sha256': sha(BASELINE),
              'tool_sha256': {n:sha(ido/n) for n in ('cc','uopt','ugen','as1')},
              'neighbors': {}, 'rectangle_controls': {}}
    spec = score.compile_group(GROUP, BUILD/'neighbors.o')
    result['neighbors'] = {'group_source_sha256': sha(GROUP/'group.json'),
        'source_sha256': {name:sha(GROUP/name) for name in spec['files']},
        'comparison': {name:comparison(BUILD/'neighbors.o', name) for name in spec['members']}}
    for name, control in result['neighbors']['comparison'].items():
        assert not control['differing'] and not control['extra_words'], name
        assert not any(control[key] for key in ('unresolved', 'unverified', 'errors')), name
    sources = [('baseline', BASELINE),
               ('sdk_sibling_both', HERE/'sdk_sibling_both.c'),
               ('sdk_sibling_all', HERE/'sdk_sibling_all.c')]
    for name, source in sources:
        out = BUILD/name
        out.mkdir(exist_ok=True)
        (out/'source.c').write_bytes(source.read_bytes())
        subprocess.run([str(ido/'cc'), '-c', '-K', *shlex.split(FLAGS),
                        '-o', 'source.o', 'source.c'], cwd=out, check=True,
                       capture_output=True)
        proof, native, linked, symbols = replay.inspect(out/'source.o', out)
        semantic = replay.semantics(source, out, native, linked, symbols)
        extra = list(fresh.extra_cases())
        for category, args, state, unused in extra:
            expected = replay.oracle(args, state)
            words, advance, events, offsets = replay.machine.execute(
                linked, symbols[replay.NAME], symbols, args, state)
            nwords, nadvance, nevents, noffsets = replay.machine.execute(
                native, symbols[replay.NAME], symbols, args, state)
            assert words == nwords == expected and advance == nadvance == len(expected)*4
            assert events == nevents
        listing = (out/'u.out.s').read_text()
        # These source controls preserve the baseline's label numbering. The
        # text fragment is checked privately; no raw listing is published.
        marker = '\t.alias\t$3,$sp\n$46:'
        boundary = listing.count(marker)
        result['rectangle_controls'][name] = {
            'source': str(source.relative_to(ROOT)), 'source_sha256': sha(source),
            'object_sha256': sha(out/'source.o'), 'proof': proof,
            'semantics': semantic, 'additional_semantic_cases': len(extra),
            'baseline_metadata_boundary_count': boundary,
            'ugen_listing_sha256': sha(out/'u.out.s'),
            'ugen_nonlocation_sha256': hashlib.sha256(normalized_listing(listing).encode()).hexdigest()}
        print(name, proof['project_scorer_differing_words'],
              proof['elf_function_bytes'], 'boundary', boundary, flush=True)
    baseline = result['rectangle_controls']['baseline']
    assert baseline['proof']['project_scorer_differing_words'] == 4
    assert baseline['proof']['elf_function_bytes'] == 1780
    for name in ('sdk_sibling_both', 'sdk_sibling_all'):
        control = result['rectangle_controls'][name]
        control['linked_body_equals_baseline'] = (
            control['proof']['linked_body_sha256'] == baseline['proof']['linked_body_sha256'])
        control['ugen_nonlocation_equals_baseline'] = (
            control['ugen_nonlocation_sha256'] == baseline['ugen_nonlocation_sha256'])
        assert control['linked_body_equals_baseline']
        assert control['ugen_nonlocation_equals_baseline']
        assert control['baseline_metadata_boundary_count'] == 1
    (BUILD/'verification.json').write_text(json.dumps(result, indent=2)+'\n')
    print('Receipt:', BUILD/'verification.json')


if __name__ == '__main__':
    main()
