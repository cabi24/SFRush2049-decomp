#!/usr/bin/env python3
"""Local actual-TU audio-record proof. No promotion, locks, or remote mutation.

Products stay in --out. Reports contain hashes/counts only, never target bytes.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

REPO = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(REPO))
from tools.cloud import score
from tools.conveyor.pipeline.lock import body_sha, normalize_body
from tools.conveyor.seeds.extract_candidates import extract_functions

BASE = "cf10b3392d7f00ae42d75c008b79fdc2541aab6b"
TU = "src/rom/lib_11640.c"
CANDIDATES = tuple("func_800" + a for a in (
    "10D74", "11894", "11C84", "13964", "13C84", "146B4", "149DC",
    "14A74", "14AF0", "14B3C", "14BB0", "14C18"))
DEFERRED = "func_80013964"
CLEANUP = "func_8001144C"
CFLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-Iinclude",
          "-Iinclude/PR", "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm",
          "-Isrc/rom"]
DATA_SECTIONS = {".data", ".rodata", ".rdata", ".bss", ".sdata", ".sbss", ".lit4", ".lit8"}

class VerificationError(RuntimeError):
    pass


def require(condition, message):
    if not condition:
        raise VerificationError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(args):
    return subprocess.run([str(a) for a in args], cwd=REPO,
                          capture_output=True, text=True)


def checked(args):
    result = run(args)
    require(result.returncode == 0, "command failed: " + " ".join(map(str, args))
            + "\n" + result.stdout + result.stderr)
    return result.stdout


def compile_tu(text, out, name, expect_error=False):
    source, obj = out / (name + ".c"), out / (name + ".o")
    source.write_text(text)
    command = [sys.executable, REPO / "tools/asm-processor/build.py", score.ido("cc"),
               "--", "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude",
               "--", "-c", *CFLAGS, "-o", obj, source]
    result = run(command)
    log = result.stdout + result.stderr
    if expect_error:
        require(result.returncode != 0 and "redeclaration" in log,
                "the original declaration conflict was not reproduced")
        return {"rejected": True, "reason": "original incompatible declarations"}
    require(result.returncode == 0 and obj.is_file(), "actual-TU compilation failed:\n" + log)
    return obj


def function_symbols(obj):
    data, sections = score._elf(obj)
    text = score._text_index(sections)
    all_symbols = [sym for i, section in enumerate(sections) if section["type"] == 2
                   for sym in score._symbol_table(data, sections, i)]
    functions = {s["name"]: s for s in all_symbols if s["section"] == text and s["type"] == 2}
    require(not any(s["size"] and s["name"] in DATA_SECTIONS for s in sections),
            "unexpected allocated data section in the actual TU")
    return functions, all_symbols


def exact_extent(symbols, name, expected):
    require(name in symbols, name + ": missing function symbol")
    require(symbols[name]["size"] == expected, name + ": STT_FUNC extent differs from target")
    start = symbols[name]["value"]
    following = min((s["value"] for s in symbols.values() if s["value"] > start), default=None)
    require(following is None or start + expected <= following,
            name + ": function extends into its neighbor")


def relocated_bytes(obj, name, expected, addresses):
    symbols, _ = function_symbols(obj)
    exact_extent(symbols, name, expected)
    start = symbols[name]["value"]
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + expected, addresses)
    require(not (masks or unresolved or unverified or errors),
            name + ": incomplete relocation verification")
    return struct.pack(">" + "I" * (expected // 4), *resolved[start // 4:(start + expected) // 4])


def link_tu(obj, out, addresses):
    """Independently resolve the complete real-TU object using GNU ld."""
    functions, all_symbols = function_symbols(obj)
    names = sorted(n for n in functions if re.fullmatch(r"func_[0-9A-F]{8}", n))
    base = min(addresses[n] for n in names)
    for name in names:
        require(functions[name]["value"] + base == addresses[name],
                name + ": actual TU layout no longer has the native offset")
    assigns = []
    for sym in all_symbols:
        if sym["section"] != 0 or not sym["name"]:
            continue
        name = sym["name"]
        address = addresses.get(name, score.address_named(name))
        require(address is not None and re.fullmatch(r"[A-Za-z_]\w*", name),
                "unresolved linker symbol: " + name)
        assigns.append(f"{name} = 0x{address:08x};")
    script = out / (obj.stem + ".ld")
    script.write_text("\n".join(assigns) + "\nSECTIONS { . = 0x%08x; .text : { *(.text) } }\n" % base)
    elf, binary = out / (obj.stem + ".elf"), out / (obj.stem + ".bin")
    checked(["mips-linux-gnu-ld", "-EB", "-T", script, "-o", elf, obj])
    checked(["mips-linux-gnu-objcopy", "-O", "binary", "--only-section=.text", elf, binary])
    return base, binary.read_bytes(), names


def check_object(obj, required, out, targets, addresses):
    functions, _ = function_symbols(obj)
    base, linked, all_names = link_tu(obj, out, addresses)
    rows = []
    for name in required:
        want = struct.pack(">" + "I" * len(targets[name]), *targets[name])
        got = relocated_bytes(obj, name, len(want), addresses)
        start = functions[name]["value"]
        independent = linked[start:start + len(want)]
        require(got == want, name + ": full relocated bytes differ from target")
        require(independent == want, name + ": GNU-linked bytes differ from target")
        rows.append({"function": name, "target_bytes": len(want),
                     "function_bytes": functions[name]["size"], "full_word_differences": 0,
                     "unresolved": 0, "unverified": 0, "relocation_errors": 0,
                     "target_sha256": sha(want), "relocated_sha256": sha(got),
                     "gnu_linked_sha256": sha(independent)})
    # Check every slot, including assembly passthroughs, with GNU relocation.
    for name in all_names:
        want = struct.pack(">" + "I" * len(targets[name]), *targets[name])
        start = addresses[name] - base
        require(linked[start:start + len(want)] == want, name + ": complete TU slot differs")
    end = max(addresses[n] - base + len(targets[n]) * 4 for n in all_names)
    require(not any(linked[end:]), "nonzero bytes beyond complete TU target extent")
    return {"functions": rows, "all_tu_slots_verified": len(all_names),
            "linked_text_sha256": sha(linked), "linked_text_bytes": len(linked),
            "trailing_zero_padding_bytes": len(linked) - end}



def function_body(text, name):
    bodies = [text[start:end] for fn, start, end in extract_functions(text) if fn == name]
    require(len(bodies) == 1, name + ": expected one function body")
    return bodies[0]


def accepted_functions(text, locks):
    accepted = sorted(k.split(":")[1] for k in locks if k.startswith(TU + ":"))
    for name in accepted:
        observed = sha(normalize_body(function_body(text, name)).encode())
        require(observed == locks[TU + ":" + name]["body_sha256"], name + ": current body lock differs")
    return accepted


def splice(text, sources, contexts, accepted):
    """Overlay pending candidates; accept real future C only with its current lock."""
    for name, source in sources.items():
        if name in accepted:
            continue
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/%s.s")' % name
        require(text.count(pragma) == 1, name + ": missing pending slot/current lock")
        have = {' '.join(line.split()) for line in text[:text.index(pragma)].splitlines()}
        declarations = [s for s in contexts[name]['preamble']
                        if s.startswith('#') or ' '.join(s.split()) not in have]
        text = text.replace(pragma, '\n'.join(declarations) + '\n' + function_body(source, name), 1)
    return text


def adapted_cleanup(text, source):
    """True two-cell table, isolated until the maintainer relocks the cleanup."""
    current = function_body(text, CLEANUP)
    wanted = function_body(source, CLEANUP)
    if normalize_body(current) == normalize_body(wanted):
        require('extern unsigned short *D_800382D8[2];' in text, 'missing table declaration')
        return text
    require('D_8003801C(D_800382D8);' in current and 'D_8003801C(D_800382DC);' in current,
            'cleanup has a new form requiring explicit review')
    text = text.replace('extern void *D_800382D8;', 'extern unsigned short *D_800382D8[2];')
    text = text.replace('extern void *D_800382DC;\n', '')
    return text.replace(current, wanted, 1)


def compiler_identity():
    expected = json.loads((REPO / 'cloud/work/boot_tail/packet2/preflight.json').read_text())['compiler_files_sha256']
    observed = {n: sha((Path(score.ido('cc')).parent / n).read_bytes()) for n in expected}
    require(observed == expected, 'pinned IDO identity mismatch')
    return observed


def layout_check(out):
    offsets = {
        'active':0, 'pending':1, 'flags':2, 'rate':4, 'sample_position':8,
        'current':0x10, 'position':0x14, 'initial_count':0x20, 'value20':0x20,
        'value22':0x22, 'value24':0x24, 'release_count':0x28, 'data':0x2c,
        'length':0x30, 'loop_start':0x34, 'loop_length':0x38, 'tail':0x3c,
        'value40':0x40, 'value42':0x42, 'value44':0x44, 'value46':0x46,
        'count':0x48, 'value':0x4c, 'step':0x50, 'scale':0x54,
        'saved_value':0x58, 'state':0x5c, 'format':0x5d, 'unknown60':0x60, 'release_pending':0x61,
    }
    assertions = ['typedef char size_ok[sizeof(AudioState) == 0x68 ? 1 : -1];',
                  'typedef char pointer_ok[sizeof(void *) == 4 ? 1 : -1];']
    assertions += ['typedef char offset_%s[((unsigned int)&((AudioState *)0)->%s) == %s ? 1 : -1];' % (n,n,v)
                   for n,v in offsets.items()]
    src = out / 'layout.c'
    src.write_text('#include "boot_tail_audio_record.h"\n' + '\n'.join(assertions))
    checked([score.ido('cc'), '-c', *CFLAGS, '-o', out / 'layout.o', src])
    sample_offsets = {'frequency':0, 'data':4, 'offset':8, 'length':12,
                      'loopStart':16, 'loopLength':20, 'format':24}
    sample = out / 'sample_layout.c'
    sample_assertions = ['typedef char sample_size_ok[sizeof(SampleInfo_800146B4) == 25 ? 1 : -1];']
    sample_assertions += ['typedef char sample_offset_%s[((unsigned int)&((SampleInfo_800146B4 *)0)->%s) == %s ? 1 : -1];' % (n,n,v)
                          for n,v in sample_offsets.items()]
    sample.write_text('#include "' + str(HERE/'sources/func_800146B4.c') + '"\n' + '\n'.join(sample_assertions))
    checked([score.ido('cc'), '-c', *CFLAGS, '-o', out/'sample_layout.o', sample])
    return {'record_size':104, 'pointer_size':4, 'field_offsets':offsets,
            'sample_descriptor_size':25, 'sample_descriptor_offsets':sample_offsets}


def _verify(out):
    out = Path(out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    identity = compiler_identity()
    original = checked(['git', 'show', BASE + ':' + TU])
    current = (REPO / TU).read_text()
    base_locks = json.loads(checked(['git','show',BASE + ':matched.lock.json']))
    baseline = accepted_functions(original, base_locks)
    require(len(baseline) == 35, 'historical baseline must have 35 locked bodies')
    locks = json.loads((REPO / 'matched.lock.json').read_text())
    accepted = accepted_functions(current, locks)
    require(set(baseline) <= set(accepted), 'a historical accepted body lost its lock')
    spec = importlib.util.spec_from_file_location('audio_contract_context', REPO / 'cloud/work/boot_tail_promotion/context_check.py')
    context = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(context)
    context.CC = Path(score.ido('cc'))
    sources, contexts = {}, {}
    for name in (*CANDIDATES, CLEANUP):
        path = HERE / 'sources' / (name + '.c')
        result = context.check(name, out, path)
        require(result['status'] == 'ok', name + ': standalone/context refusal: ' + str(result))
        sources[name] = path.read_text()
        contexts[name] = result
    score.ASM_DIR = REPO / 'asm/us/boot_tail'
    targets, addresses = score.targets(), score.image_symbols()
    standalone = []
    for name in (*CANDIDATES, CLEANUP):
        obj = out / (name + '.orig.o')
        want = struct.pack('>' + 'I' * len(targets[name]), *targets[name])
        got = relocated_bytes(obj, name, len(want), addresses)
        require(got == want, name + ': standalone full bytes differ')
        standalone.append({'function':name, 'function_bytes':len(want), 'match':True})
    independent = {n:sources[n] for n in CANDIDATES if n != DEFERRED}
    combined = splice(current, independent, contexts, accepted)
    complete = combined
    if DEFERRED not in accepted:
        complete = adapted_cleanup(complete, sources[CLEANUP])
        complete = splice(complete, {DEFERRED:sources[DEFERRED]}, contexts, accepted)
    stages = {}
    for name, text, required in (
        ('baseline',original,baseline), ('repaired',current,accepted),
        ('eleven_candidate_overlay',combined,sorted(set(accepted) | set(independent))),
        ('twelve_with_cleanup_relock_overlay',complete,sorted(set(accepted) | set(CANDIDATES))),
    ):
        obj = compile_tu(text, out, name)
        stages[name] = check_object(obj, required, out, targets, addresses)
    require(len({s['linked_text_sha256'] for s in stages.values()}) == 1,
            'whole GNU-linked TU text differs between stages')
    # Original isolated declarations fail even before checking emitted code.
    old = (REPO / 'cloud/matches/boot_tail/func_80011894.c').read_text()
    pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011894.s")'
    old_rejection = compile_tu(original.replace(pragma, old), out, 'old_conflict', True)
    # Wrong selected buffer is source-valid and compiles but must fail strict bytes.
    bad = complete.replace('D_800382D0 = D_800382D8[D_8003829E];', 'D_800382D0 = D_800382D8[0];')
    require(bad != complete, 'negative control no longer modifies buffer selection')
    bad_obj = compile_tu(bad, out, 'wrong_buffer')
    rejected = False
    try:
        check_object(bad_obj, [DEFERRED], out, targets, addresses)
    except VerificationError:
        rejected = True
    require(rejected, 'wrong-buffer negative control was accepted')
    wrong_header = out / 'wrong_stride.h'
    header = (REPO / 'include/boot_tail_audio_record.h').read_text()
    require('unknown62[6]' in header, 'stride-control input changed')
    wrong_header.write_text(header.replace('unknown62[6]', 'unknown62[14]'))
    bad_stride = complete.replace('#include "boot_tail_audio_record.h"', '#include "' + str(wrong_header) + '"')
    bad_stride_obj = compile_tu(bad_stride, out, 'wrong_stride')
    stride_rejected = False
    try:
        check_object(bad_stride_obj, ['func_80011C84'], out, targets, addresses)
    except VerificationError:
        stride_rejected = True
    require(stride_rejected, 'wrong-record-stride negative control was accepted')
    # The table adaptation cannot enter the production TU while the old scalar
    # cleanup contract remains. Use the fixed historical body for this control.
    conflict = original.replace(
        '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80013964.s")', sources[DEFERRED])
    table_rejection = compile_tu(conflict, out, 'scalar_table_conflict', True)
    paths = [TU, 'include/boot_tail_audio_record.h'] + [str((HERE/'sources'/(n+'.c')).relative_to(REPO)) for n in (*CANDIDATES,CLEANUP)]
    return {
        'result':'PASS', 'base_commit':BASE, 'translation_unit':TU,
        'baseline_existing_locked_functions':35,
        'independent_candidates':11, 'independent_candidate_bytes':sum(len(targets[n])*4 for n in independent),
        'cleanup_dependent_candidates':1, 'cleanup_dependent_bytes':len(targets[DEFERRED])*4,
        'cleanup_requires_maintainer_relock':DEFERRED not in accepted,
        'standalone':standalone, 'stages':stages, 'layout':layout_check(out),
        'negative_controls':{'original_declarations':old_rejection,'wrong_buffer_rejected':rejected, 'wrong_stride_rejected':stride_rejected, 'scalar_table_conflict':table_rejection},
        'compiler_files_sha256':identity, 'flags':CFLAGS,
        'source_sha256':{p:sha((REPO/p).read_bytes()) for p in paths},
        'verification_script_sha256':sha(Path(__file__).read_bytes()),
        'protected_inputs_sha256':{p:sha((REPO/p).read_bytes()) for p in (
            'asm/us/boot_tail/SHA256SUMS','asm/us/boot_tail/symbols.json','matched.lock.json',
            'src/rom/rom_tu.h','tools/cloud/score.py','tools/asm-processor/build.py','tools/asm-processor/asm_processor.py')},
        'rom_gate':'not run; maintainer transaction required',
    }


def verify(out):
    old = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _verify(out)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = old


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=REPO/'build/audio-record-contracts/proof')
    args = parser.parse_args()
    try:
        report = verify(args.out)
    except (VerificationError, RuntimeError, ValueError) as error:
        print(str(error), file=sys.stderr)
        return 1
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
