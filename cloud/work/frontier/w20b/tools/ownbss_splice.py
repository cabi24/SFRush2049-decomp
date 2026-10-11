#!/usr/bin/env python3
"""w20b (builder, scratch root): manual splice of the static-.bss trio into a COPY of src/rom/lib_16320.c.
context_check.py refuses sources with .bss (owned-slot rule), so the preamble is taken from the source
itself: every top-level line before the function (comments dropped), deduplicated against the TU text
before the slot (promote_batch --prefix-dedup semantics; preprocessor lines kept unless an identical
#include is already present).
    python3 lane/tools/ownbss_splice.py OUTDIR SRCDIR fn [fn...]"""
import re, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.seeds.extract_candidates import extract_named_function  # noqa: E402
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True); srcdir = Path(sys.argv[2])
norm = lambda s: ' '.join(s.split())
tu = Path('src/rom/lib_16320.c'); text = tu.read_text()
for fn in sys.argv[3:]:
    pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/{fn}.s")'
    assert text.count(pragma) == 1, fn
    src = (srcdir / f'{fn}.c').read_text()
    body = extract_named_function(srcdir / f'{fn}.c', fn)
    pre = re.sub(r'/\*.*?\*/', '', src[:src.index(body.strip()[:40])], flags=re.S)
    before = {norm(l) for l in text[:text.index(pragma)].splitlines() if l.strip()}
    # basic typedefs come from rom_tu.h (types.h); context_check drops them the same way
    decls = [l for l in pre.splitlines() if l.strip() and not re.match(r'typedef (unsigned |signed )?(char|short|int|long) [us](8|16|32);', l) and (
        (l.startswith('#') and not (l.startswith('#include') and norm(l) in before)) or
        (not l.startswith('#') and norm(l) not in before))]
    hdr = f"/* w20b dry run - {fn} (static .bss member; lane/ownbss/{fn}.c) */\n"
    text = text.replace(pragma, hdr + ''.join(d + '\n' for d in decls) + body.rstrip() + '\n', 1)
(out / tu.name).write_text(text); print('wrote', out / tu.name)
