#!/usr/bin/env python3
"""w17g: make TU-context conflicts disappear by mechanical, codegen-neutral source edits (Pi; compiles on the
builder scratch with IDO 7.1). For each function (in TU order) the source in w17g/sources is spliced (splice71
rules, prefix dedup) into the TU text accumulated so far and the TU is compiled (cfe only cares about
declarations). On `redeclaration of 'T'` pointing into this function's inserted block:
  * T is a typedef/struct tag the source defines -> rename T to T_<addr> in the whole source (word boundary);
  * T is a function/object the TU already declared before the slot -> drop the source's own declaration.
Edits are written back to the source with a header note; codegen is re-proved afterwards by run71.sh (LOCK/TU)
and tusplice.sh (whole TU identical)."""
import json, re, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
sys.path.insert(0, str(Path.cwd() / 'cloud/work/boot_tail_promotion'))
from tools.conveyor.seeds.extract_candidates import extract_functions, extract_named_function  # noqa
import context_check as cc  # noqa  (split_statements)
W = Path('cloud/work/frontier/w17g'); SRC = W / 'sources'
CF = "-G 0 -mips2 -O2 -non_shared -Iinclude -Iinclude/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -Isrc/rom"
norm = lambda s: ' '.join(s.split())

def remote_cc(text):
    p = subprocess.run(['ssh', 'watchman2', 'cd ~/rush2049/scratch/frontier/w17g && cat > hz.c && '
                        f'tk71/ido/cc -c {CF} -o hz.o hz.c 2>&1; echo RC=$?'], input=text, capture_output=True, text=True)
    return p.stdout

def preamble_and_body(fn):
    text = (SRC / f'{fn}.c').read_text()
    (_, st, en), = [x for x in extract_functions(text) if x[0] == fn]
    return text, [s[2] for s in cc.split_statements(text[:st])], text[st:en]

def block_for(fn, tutext, pragma):
    _, stmts, body = preamble_and_body(fn)
    before = {norm(l) for l in tutext[:tutext.index(pragma)].splitlines() if l.strip()}
    decls = [s for s in stmts if (s.startswith('#') and not (s.startswith('#include') and norm(s) in before))
             or (not s.startswith('#') and norm(s) not in before)]
    # rom_tu.h-provided typedefs (u8, s32 ...) the source repeats: drop like context_check would
    decls = [d for d in decls if not re.match(r'typedef (unsigned |signed )?(char|short|int|long( long)?|float|double) (u8|s8|u16|s16|u32|s32|u64|s64|f32|f64);$', norm(d))]
    return f'/* w17g block {fn} */\n' + ''.join(d + '\n' for d in decls) + body.rstrip() + f'\n/* w17g end {fn} */\n'

def main(fns):
    texts = {}
    for fn in fns:
        tu = [p for p in Path('src/rom').glob('*.c') if f'/{fn}.s")' in p.read_text()][0]
        pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/{tu.stem}/{fn}.s")'
        base = texts.get(tu) or tu.read_text()
        notes = []
        for _ in range(30):
            t = base.replace(pragma, block_for(fn, base, pragma), 1)
            out = remote_cc(t)
            lines = t.splitlines()
            s0 = next(i for i, l in enumerate(lines) if l == f'/* w17g block {fn} */') + 1
            s1 = next(i for i, l in enumerate(lines) if l == f'/* w17g end {fn} */') + 1
            errs = [(int(m.group(1)), m.group(2)) for m in re.finditer(r"hz\.c, line (\d+): redeclaration of '(\w+)'", out)]
            errs = [e for e in errs if s0 <= e[0] <= s1]
            if 'RC=0' in out and not errs:
                texts[tu] = t; print(fn, 'OK', '; '.join(notes) or 'no edits'); break
            if not errs:
                print(fn, 'UNRESOLVED', [l for l in out.splitlines() if 'Error' in l][:3]); break
            name = errs[0][1]; src = (SRC / f'{fn}.c').read_text()
            if re.search(r'typedef\b[^;]*\b' + name + r'\s*;', src) or re.search(r'struct\s+' + name + r'\b', src):
                new = f'{name}_{fn[5:]}'
                src = re.sub(r'\b' + name + r'\b', new, src); notes.append(f'type {name}->{new}')
            else:
                text, stmts, body = preamble_and_body(fn)
                victim = [s for s in stmts if re.search(r'\b' + name + r'\b', s) and not s.startswith('#')]
                if len(victim) != 1: print(fn, 'UNRESOLVED decl', name); break
                # remove the statement from the file text (it may span lines)
                pat = r'\s+'.join(map(re.escape, victim[0].split()))
                src, n = re.subn(pat + r'\n?', '', src, count=1)
                if n != 1: print(fn, 'UNRESOLVED remove', name); break
                notes.append(f'dropped own decl of {name} (TU declares it before the slot)')
            (SRC / f'{fn}.c').write_text(src)
        else:
            print(fn, 'UNRESOLVED loop')
        if notes:
            s = (SRC / f'{fn}.c').read_text()
            s = s.replace(' * Shaping devices:', ' * w17g TU-context edits (codegen-neutral, re-proved): ' + '; '.join(notes) + '.\n * Shaping devices:', 1)
            (SRC / f'{fn}.c').write_text(s)

if __name__ == '__main__':
    main(sys.argv[1:])
