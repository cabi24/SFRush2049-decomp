#!/usr/bin/env python3
"""Read-only exact combined-TU proof for six shared sample/emitter contracts.

Local temporary overlays only; no lock, promotion, remote or ROM build writes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.cloud import score
from tools.conveyor.pipeline.lock import body_sha
from tools.conveyor.seeds.extract_candidates import extract_functions
BASE = "cf10b3392d7f00ae42d75c008b79fdc2541aab6b"
WORK = Path(__file__).resolve().parent
HEADER = "include/boot_tail_sample_contract.h"
SOURCES = "cloud/work/boot_tail_promotion/sources/"
GROUPS = {
    "src/rom/lib_16320.c": ["func_800163A8"],
    "src/rom/lib_1cf90.c": ["func_8001C508", "func_8001C7F4", "func_8001D4CC", "func_8001DDE0", "func_8001E0C0"],
}
ERRORS = dict(zip(sum(GROUPS.values(), []), ["D_800385A0", "D_8004FA50", "D_8004FA50", "func_8001D084", "D_8004FD50", "D_8004FD50"]))
CFLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-Iinclude",
          "-Iinclude/PR", "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm", "-Isrc/rom"]
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


def compile_tu(text, out, name, expect_error=None):
    source, obj = out / (name + ".c"), out / (name + ".o")
    source.write_text(text)
    command = [sys.executable, REPO / "tools/asm-processor/build.py", score.ido("cc"),
               "--", "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude",
               "--", "-c", *CFLAGS, "-o", obj, source]
    result = run(command)
    log = result.stdout + result.stderr
    if expect_error:
        require(result.returncode != 0 and expect_error in log,
                "the original declaration conflict was not reproduced")
        return {"rejected": True, "reason": expect_error}
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


def overlay(tu, path, name, source):
    definitions = {n for n, _, _ in extract_functions(tu)}
    pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + Path(path).stem + '/' + name + '.s")'
    if name in definitions:
        require(pragma not in tu, name + ": both C definition and passthrough")
        return tu
    require(tu.count(pragma) == 1, name + ": expected one passthrough")
    return tu.replace(pragma, source, 1)


def native_layout(out):
    # A compile-time native-width proof, not a host sizeof claim.
    fields = {
        "SampleRecord": (28, {"identifier": 0, "references": 2, "offset": 4, "data": 8, "descriptor": 12}),
        "RegisteredSamples": (12, {"records": 0, "base": 4, "count": 8, "unknown0A": 10}),
        "SampleBuffer": (24, {"mode": 0, "callback": 4, "buffer": 8, "samples": 12, "position": 16, "context": 20}),
        "StateNode": (68, {"next": 0, "previous": 4, "flags08": 8, "unknown0C": 12, "identifier34": 52, "group": 56, "sound_id": 60, "counter": 62, "fade": 64}),
        "LinkNode": (8, {"next": 0, "previous": 4}),
    }
    text = '#include "boot_tail_sample_contract.h"\n#define OFF(T,m) ((unsigned int)&((T*)0)->m)\n'
    for typ, (size, members) in fields.items():
        conditions = ['sizeof(' + typ + ')==' + str(size)]
        conditions += ['OFF(' + typ + ',' + member + ')==' + str(offset) for member, offset in members.items()]
        text += 'typedef char check_' + typ + '[' + '&&'.join(conditions) + '?1:-1];\n'
    probe = out / 'native_layout.c'
    probe.write_text(text)
    score.compile_single(probe, score.DEFAULT_FLAGS + ' -I' + str(REPO / 'include'), out / 'native_layout.o')
    return {typ: {'size': size, 'offsets': members} for typ, (size, members) in fields.items()}


def mutation_controls(out, targets, addresses):
    controls = [
        ('registry_sentinel', 'func_800163A8', 'resource->identifier != 0xffff', 'resource->identifier != 0xfffe'),
        ('registry_signed_compare', 'func_800163A8', 'unsigned int i;', 'int i;'),
        ('buffer_wrong_samples', 'func_8001C508', '.samples', '.position'),
        ('emitter_invented_default', 'func_8001DDE0', 'float volume;', 'float volume = 0.0f;'),
    ]
    results = []
    for label, name, old, new in controls:
        source = (REPO / SOURCES / (name + '.c')).read_text()
        require(old in source, 'mutation anchor missing: ' + label)
        source = source.replace(old, new)
        if label == 'registry_signed_compare':
            source = source.replace('i < (unsigned int)D_800385A0', 'i < D_800385A0')
        path, obj = out / (label + '.c'), out / (label + '.o')
        path.write_text(source)
        score.compile_single(path, score.DEFAULT_FLAGS + ' -I' + str(REPO / 'include'), obj)
        symbols, _ = function_symbols(obj)
        expected = len(targets[name]) * 4
        mismatch = symbols[name]['size'] != expected
        if not mismatch:
            got = relocated_bytes(obj, name, expected, addresses)
            mismatch = got != struct.pack('>' + 'I' * len(targets[name]), *targets[name])
        require(mismatch, label + ': incorrect control was accepted')
        results.append({'name': label, 'rejected': True, 'elf_bytes': symbols[name]['size']})
    return results


def _verify(out):
    out = Path(out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    score.ASM_DIR = REPO / 'asm/us/boot_tail'
    targets, addresses = score.targets(), score.image_symbols()
    base_locks = json.loads(checked(['git', 'show', BASE + ':matched.lock.json']))
    locks = json.loads((REPO / 'matched.lock.json').read_text())
    contexts = {r['function']: r for r in map(json.loads, checked(['git', 'show', BASE + ':cloud/work/boot_tail_promotion/context.jsonl']).splitlines())}
    stages, negative, standalone = {}, [], []
    for tu, candidates in GROUPS.items():
        original = checked(['git', 'show', BASE + ':' + tu])
        current = (REPO / tu).read_text()
        baseline = sorted(k.split(':')[1] for k in base_locks if k.startswith(tu + ':'))
        accepted = sorted(k.split(':')[1] for k in locks if k.startswith(tu + ':'))
        require(set(baseline) <= set(accepted), tu + ': existing locks lost')
        for name in accepted:
            require(body_sha(REPO / tu, name) == locks[tu + ':' + name]['body_sha256'], name + ': locked body changed')
        combined = current
        for name in candidates:
            row = contexts[name]
            old = checked(['git', 'show', BASE + ':' + row['source']])
            (_, start, end), = list(extract_functions(old))
            legacy = '\n'.join(row['preamble']) + '\n' + old[start:end]
            negative.append(compile_tu(overlay(original, tu, name, legacy), out, name + '_refusal', expect_error="redeclaration of '" + ERRORS[name] + "'"))
            source = (REPO / SOURCES / (name + '.c')).read_text()
            combined = overlay(combined, tu, name, source)
            obj = out / (name + '_standalone.o')
            score.compile_single(REPO / SOURCES / (name + '.c'), score.DEFAULT_FLAGS + ' -I' + str(REPO / 'include'), obj)
            want = struct.pack('>' + 'I' * len(targets[name]), *targets[name])
            require(relocated_bytes(obj, name, len(want), addresses) == want, name + ': standalone differs')
            standalone.append({'function': name, 'bytes': len(want), 'exact': True})
        group = {}
        for label, text, required in [('baseline', original, baseline), ('repaired', current, accepted), ('combined', combined, sorted(set(accepted) | set(candidates)))]:
            obj = compile_tu(text, out, Path(tu).stem + '_' + label)
            group[label] = check_object(obj, required, out, targets, addresses)
        require(len({stage['linked_text_sha256'] for stage in group.values()}) == 1, tu + ': linked TU changed')
        stages[tu] = group
    paths = [HEADER, *GROUPS, *[SOURCES + n + '.c' for n in sum(GROUPS.values(), [])]]
    result = {'base_commit': BASE, 'flags': CFLAGS, 'stages': stages,
              'original_conflict_controls': negative, 'standalone_candidates': standalone,
              'native_layout': native_layout(out), 'semantic_mutation_controls': mutation_controls(out, targets, addresses),
              'source_sha256': {p: sha((REPO / p).read_bytes()) for p in paths},
              'verification_script_sha256': sha(Path(__file__).read_bytes()),
              'semantic_test_sha256': {p: sha((REPO / p).read_bytes()) for p in ('tests/cloud/test_sample_buffer_contract.py', 'cloud/work/boot_tail_promotion/sample_buffer_contract/replay_emitter.py', 'cloud/work/boot_tail/BT03-emitter-handle/test_semantics.py', 'cloud/work/boot_tail/BT03-emitter-handle/model.py')},
              'protected_inputs_sha256': {p: sha((REPO / p).read_bytes()) for p in ('asm/us/boot_tail/SHA256SUMS', 'asm/us/boot_tail/extents.json', 'asm/us/boot_tail/symbols.json', 'matched.lock.json', 'src/rom/rom_tu.h', 'tools/cloud/score.py', 'tools/asm-processor/build.py', 'tools/asm-processor/asm_processor.py')},
              'ido_sha256': {n: sha(Path(score.ido(n)).read_bytes()) for n in ('cc', 'cfe', 'uopt', 'ugen', 'as1')},
              'gnu_ld_version': checked(['mips-linux-gnu-ld', '--version']).splitlines()[0],
              'rom_gate': 'not run; maintainer promotion and full-ROM transaction required',
              'semantic_limit': '1DDE0 retains five uninitialized output locals. Every consumed output must already have been written in the same invocation; universal producer/listener invariants remain unproved.'}
    (out / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


def verify(out):
    old = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _verify(out)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = old


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=REPO / 'build/sample_buffer_contract')
    args = parser.parse_args()
    result = verify(args.out)
    for tu, stages in result['stages'].items():
        combined = stages['combined']
        print(tu + ': ' + str(len(combined['functions'])) + ' exact C extents; ' + str(combined['all_tu_slots_verified']) + ' complete native slots MATCH')
    print('Six original refusals and four incorrect-body controls rejected; native layout assertions pass.')
    print('Full-ROM gate not run; maintainer transaction required.')

if __name__ == '__main__':
    main()
