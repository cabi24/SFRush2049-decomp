#!/usr/bin/env python3
"""Input-bound strict overlay-A cleanup proof; optional bounded behavior replay."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = HERE / "group"
NAME = "func_8039D05C"
NAMES = {NAME: 676, "func_8039D494": 528, "func_8039ED20": 480,
         "func_8039E200": 444, "func_8039E3BC": 136}
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"
def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    sys.modules[name] = result
    spec.loader.exec_module(result)
    return result

_original_path = sys.path[:]
try:
    sys.path[:0] = [str(ROOT), str(ROOT / "tools/cloud")]
    from tools.conveyor import compilers
    score = module("cleanup_colortrace_score", ROOT / "tools/cloud/score.py")
    elf = module("cleanup_colortrace_elf", HERE / "elf_support.py").elf
    semantics = module("cleanup_colortrace_semantics", HERE / "semantics.py")
finally:
    sys.path[:] = _original_path
    del _original_path

def require(ok, message):
    if not ok:
        raise ValueError(message)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def packed(words):
    return struct.pack(">%dI" % len(words), *words)

def shell(*args):
    r = subprocess.run(list(map(str, args)), capture_output=True, text=True)
    require(r.returncode == 0, (args, r.stdout, r.stderr))
    return r.stdout

def gnu_resolve(obj, temp):
    """Re-section complete ELF function bodies; GNU independently applies relocations.

    Local .text addends are translated to their ELF STT_FUNC owner and offset.
    This is not a claim that this extracted three-function ELF is a full image.
    """
    secs, syms, rels = elf(obj)
    ti, _, raw = secs['.text']; lines = ['.set noreorder', '.set noat']; externs = set(); counts = {}
    for name, size in NAMES.items():
        start, actual, kind, section = syms[name]
        assert (actual, kind, section) == (size, 2, ti)
        following = min(v[0] for v in syms.values() if v[2] == 2 and v[3] == ti and v[0] > start)
        assert following == start + size, (name, actual, following)
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start+size])); entries = []
        for loc, typ, symbol, dest in rels:
            if dest != ti or not start <= loc < start + size: continue
            assert typ in (4, 5, 6) and loc % 4 == 0
            off = loc - start
            if symbol == '.text':
                assert typ == 4
                add = (words[off//4] & 0x3ffffff) * 4
                owners = [(n,v) for n,v in syms.items() if v[2] == 2 and v[3] == ti and v[0] <= add < v[0]+v[1]]
                assert len(owners) == 1
                symbol, owner = owners[0]
                words[off//4] = (words[off//4] & 0xfc000000) | ((add-owner[0])//4)
            assert not symbol.startswith('.')
            entries.append((off, typ, symbol)); externs.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4', '.globl ' + name, '.type '+name+',@function', name+':']
        lines += ['.word 0x%08x' % w for w in words]
        lines += ['.size %s, .-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[typ], sym) for off,typ,sym in entries]
        counts[name] = len(entries)
    assembly = temp/'review.s'; assembly.write_text('\n'.join(lines)+'\n')
    reloc = temp/'review.o'; linked = temp/'review.elf'; script = temp/'review.ld'
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n,n[-8:],n) for n in NAMES)+' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+''.join('%s = 0x%X;\n' % (n, score.image_symbols().get(n,score.address_named(n))) for n in sorted(externs-set(NAMES))))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',reloc,assembly)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,reloc)
    ss, sy, rr = elf(linked); assert not rr
    result = {}
    for n,size in NAMES.items():
        index, row, body = ss['.review_'+n]
        assert row[3] == int(n[-8:],16) and len(body) == size
        assert sy[n] == (int(n[-8:],16),size,2,index)
        result[n] = list(struct.unpack('>%dI' % (size//4), body))
    return result, counts

def audit_callers(obj):
    targets=score.targets();sections,symbols,relocs=elf(obj);allwords=score.text_words(obj)
    index=sections['.text'][0];callee=symbols[NAME][0];generated=[];native=[]
    def saves(body):
        for reg in (*range(16,24),30):
            stores=[w for w in body[:64] if w>>26==43 and (w>>21&31)==29 and (w>>16&31)==reg]
            require(len(stores)==1,('caller prologue save',reg))
            require(((stores[0]&0x03ffffff)|(35<<26)) in body[-64:],('caller epilogue restore',reg))
    for name,body in targets.items():
        sites=[i*4 for i,w in enumerate(body) if w>>26==3 and (w&0x3ffffff)==((semantics.ENTRY>>2)&0x3ffffff)]
        if sites:
            require(name=='func_803A1EAC','unexpected native caller');saves(body);native.extend(sites)
    for offset,kind,symbol,section in relocs:
        if section!=index or kind!=4 or symbol!='.text' or (allwords[offset//4]&0x3ffffff)*4!=callee:continue
        owners=[(name,v) for name,v in symbols.items() if v[2]==2 and v[3]==index and v[0]<=offset<v[0]+v[1]]
        require(len(owners)==1 and owners[0][0]=='func_803A1EAC','unexpected generated caller')
        name,v=owners[0];saves(allwords[v[0]//4:(v[0]+v[1])//4]);generated.append(offset-v[0])
    require(len(native)==len(generated)==4,'cleanup caller population changed')
    return {'caller':'func_803A1EAC','native_offsets':list(map(hex,native)),'compiled_offsets':list(map(hex,generated)),
            'caller_prologue_epilogue_saves':'s0 through s8','no_arguments':True,
            'limit':'caller remains nonmatching and is not executed end-to-end'}

def inputs():
    require(__debug__, "Run without Python -O: independent ELF reader uses assertions")
    group = json.loads((GROUP / "group.json").read_text())
    require(group["members"] == list(NAMES) and group["claims"] == [NAME], "member/claim drift")
    require(group["flags"] == FLAGS and group["files"] == ["group.c"], "group flags/files drift")
    require(group["keep"] == ["func_803A1EAC"], "private-ABI root drift")
    require(NAME not in group["context"] and "func_803A1EAC" in group["context"], "context drift")
    score.ASM_DIR = ROOT / "asm/us/ovl_a"
    native = {name: score.targets()[name] for name in NAMES}
    require(all(len(native[n])*4 == size for n, size in NAMES.items()), "target extent changed")
    manifests = {}
    for line in (score.ASM_DIR / "SHA256SUMS").read_text().splitlines():
        digest, name = line.split(None, 1)
        if name.lstrip(" *") in ("extents.json", "symbols.json"):
            manifests[name.lstrip(" *")] = digest
    require(set(manifests) == {"extents.json", "symbols.json"}, "missing relevant manifest rows")
    files = ["group/group.c", "group/group.json", "verify.py", "elf_support.py", "semantics.py", "research.py"]
    return {"population": "ovl_a", "claims": [NAME], "inputs_sha256": {n: sha((HERE/n).read_bytes()) for n in files},
            "preservation_members": [n for n in NAMES if n != NAME],
            "targets": {n: {"start": hex(int(n[-8:],16)), "end": hex(int(n[-8:],16)+size), "bytes": size,
                             "words_sha256": sha(packed(native[n]))} for n,size in NAMES.items()},
            "relevant_manifest_rows": manifests}, native

def verify(deep=False):
    bound, native = inputs()
    # Preserve score.ido's canonical missing-tool diagnostic for tests/conftest.py.
    for name in ("cc", "cfe", "uld", "usplit", "umerge", "uopt", "ugen", "as1"):
        score.ido(name)
    require(compilers.identify_dir(score.IDO) == "ido5.3", "unrecognized or instrumented compiler")
    with tempfile.TemporaryDirectory(prefix="cleanup-colortrace-verify-") as td:
        temp = Path(td)
        obj = temp / "candidate.o"
        score.compile_group(GROUP, obj)
        results = {}
        for name, size in NAMES.items():
            result = score.compare(obj, name, show=0)
            require(result.differing == 0 and result.total == size//4 and not
                    (result.unresolved or result.unverified or result.errors or result.extra_words), result)
            results[name] = dataclasses.asdict(result)
        linked, relocations = gnu_resolve(obj, temp)
        require(linked == native, "independent full-body GNU relocation mismatch")
        callers = audit_callers(obj)
        behavior = semantics.verify(native[NAME], linked[NAME]) if deep else None
    proof = {**bound, "status": "MATCH", "compiler": "ido5.3", "flags": FLAGS,
             "scores": results, "ELF_STT_FUNC_bytes": NAMES,
             "next_boundary_checked": True, "GNU_relocations": relocations,
             "linked_words_sha256": {n: sha(packed(linked[n])) for n in NAMES}, "caller_ABI": callers,
             "new_code_match_bytes": 676, "accepted_image_bytes": 0, "cartridge_bytes": 0,
             "owned_data": "No owned literal or storage references in this complete body",
             "private_ABI": "No arguments; clobbers s0-s8; not a standalone O32 replacement"}
    if deep:
        return {"proof": proof, "semantics": behavior}
    return proof

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--write", action="store_true")
    p.add_argument("--deep", action="store_true")
    args = p.parse_args()
    result = verify(args.deep)
    if args.write:
        require(not args.deep, "deep report is not the routine receipt")
        (HERE / "verification.json").write_text(json.dumps(result, indent=2) + "\n")
    elif not args.deep:
        require(result == json.loads((HERE / "verification.json").read_text()), "receipt drift")
    print(json.dumps(result, indent=2))
