#!/usr/bin/env python3
"""indep_tables_proof.py GROUP_DIR OBJ_DIR — READ-ONLY demonstration (w31b) of the blob_group change that the
func_8010AEAC unit needs. It runs tools/conveyor/pipeline/blob_group.group_bodies (the splice's relocation and
own-data proofs) on an object built with blob_group.builder_script, with ONE function replaced in memory:
`_independent_regions` accepts a window that holds relocated words when every relocated word in the window is an
R_MIPS_32 jump-table entry into THIS function's own text, and the window equals the image after each entry is
replaced by the function's image address + (target - function start). Every other rule of _independent_regions is
kept (whole-extent compare, padding rule, access width, one opaque run, no shared image bytes). Relocated words
outside the function's windows (another member's table) are no longer a reason to refuse: they are not compared
through this function's windows at all.
Nothing under tools/ is modified; the monkeypatch lives only in this process.
"""
import struct, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT))
from tools.cloud import owndata
from tools.conveyor.pipeline import blob_group as bg, blob_layout

_ADDR = {}
_orig_verify = bg.verify_own_data


def verify_own_data(obj, name, want, address, image, start=None, addresses=None, data_runs=None):
    _ADDR[name] = address
    return _orig_verify(obj, name, want, address, image, start=start, addresses=addresses, data_runs=data_runs)


def _independent_regions(obj, name, want, start, image, failures, data_runs):
    sections = set()
    for failure in failures:
        head, sep, _rest = failure.partition(bg.ONE_BASE_REFUSAL)
        if not sep or not head.startswith("own ") or head[4:] not in owndata.READ_ONLY:
            raise bg.GroupError(f"not only the one-base refusal: {failure}")
        sections.add(head[4:])
    elf = owndata._Object(obj)
    text, lo, hi = owndata._locate(elf, name, start, len(want) * 4)
    refs, stray = owndata._references(elf, text)
    if any(lo <= site < hi for site, _sym, _why in stray):
        raise bg.GroupError("a stray own-section relocation in the function")
    runs = sorted(data_runs)
    out = {}
    fn_address = _ADDR[name]
    for section_name in sorted(sections):
        index = [i for i, sec in enumerate(elf.sections) if sec["name"] == section_name]
        if len(index) != 1:
            raise bg.GroupError(f"{section_name}: not exactly one such section")
        index = index[0]
        raw = elf.raw(index)
        if raw is None:
            raise bg.GroupError(f"{section_name}: no bytes")
        table = {}                                   # section offset -> object .text target
        for offset, rtype, sym in elf.relocations(index):
            if rtype != 2 or offset % 4:             # R_MIPS_32, aligned
                raise bg.GroupError(f"{section_name}+0x{offset:x}: unsupported relocation {rtype}")
            if sym["section"] != text:
                raise bg.GroupError(f"{section_name}+0x{offset:x}: relocation not against .text")
            addend, = struct.unpack_from(">I", raw, offset)
            table[offset] = addend + (sym["value"] if sym["type"] != owndata.STT_SECTION else 0)
        starts = sorted({ref.offset for _sym, ref in refs if ref.section == index})
        windows = {}
        for _sym, ref in refs:
            if ref.section != index or not lo <= ref.lo_site < hi:
                continue
            if not all(lo <= site < hi for site in ref.hi_sites):
                raise bg.GroupError(f"{section_name}+0x{ref.offset:x}: HI16 outside the function")
            retail = {((want[(site - lo) // 4] & 0xFFFF) << 16)
                      + owndata._sext16(want[(ref.lo_site - lo) // 4] & 0xFFFF)
                      for site in ref.hi_sites}
            if len(retail) != 1:
                raise bg.GroupError(f"{section_name}+0x{ref.offset:x}: retail HI16 words disagree")
            address = retail.pop() & 0xFFFFFFFF
            seen = windows.setdefault(ref.offset, [address, 4])
            if seen[0] != address:
                raise bg.GroupError(f"{section_name}+0x{ref.offset:x}: read at two retail addresses")
            if ref.opcode in owndata.DOUBLEWORD_OPCODES:
                seen[1] = 8
        placed = []
        for offset, (address, widest) in sorted(windows.items()):
            stop = min([s for s in starts if s > offset] + [len(raw)])
            data = bytearray(raw[offset:stop])
            if len(data) < widest:
                raise bg.GroupError(f"{section_name}+0x{offset:x}: short window")
            if stop == len(raw):
                length = max(widest, (len(bytes(data).rstrip(b"\0")) + 3) & ~3)
                if len(data) - length >= bg.SECTION_PADDING:
                    raise bg.GroupError(f"{section_name}+0x{offset:x}: too much uncompared padding")
                data = data[:min(length, len(data))]
            entries = 0
            for o, target in table.items():
                if offset <= o < offset + len(data):
                    if not lo <= target < hi:
                        raise bg.GroupError(f"{section_name}+0x{o:x}: table entry outside {name}")
                    struct.pack_into(">I", data, o - offset, fn_address + target - lo)
                    entries += 1
            expected = image.read(address, len(data))
            if expected != bytes(data):
                raise bg.GroupError(f"{section_name}+0x{offset:x}: differs from retail 0x{address:08X}")
            if not any(r_lo <= address and address + len(data) <= r_hi for r_lo, r_hi in runs):
                raise bg.GroupError(f"{section_name}+0x{offset:x}: not inside one non-function run")
            print(f"  [{name}] {section_name}+0x{offset:x}..+0x{offset + len(data):x} == retail "
                  f"0x{address:08X}..0x{address + len(data):08X}" + (f" ({entries} table entries relocated)" if entries else ""))
            placed.append((offset, offset + len(data), address, "rodata"))
        ordered = sorted(placed, key=lambda p: p[2])
        for a, b in zip(ordered, ordered[1:]):
            if b[2] < a[2] + a[1] - a[0]:
                raise bg.GroupError("windows overlap in the image")
        out[section_name] = placed
    return out


bg.verify_own_data = verify_own_data
bg._independent_regions = _independent_regions

if __name__ == "__main__":
    group_dir, obj_dir = Path(sys.argv[1]).resolve(), Path(sys.argv[2]).resolve()
    doc = blob_layout.load()
    spec = bg.load(group_dir.name, root=group_dir.parent)
    bodies = bg.group_bodies(group_dir.name, doc, obj_dir=obj_dir, root=group_dir.parent)
    diffs = bg.word_diffs(bodies, doc)
    for m in spec["members"]:
        print(f"{m}: {'image-identical' if diffs[m] == 0 else str(diffs[m]) + ' words differ'}")
    print(f"{sum(1 for m in spec['members'] if diffs[m] == 0)}/{len(spec['members'])} members image-identical")
