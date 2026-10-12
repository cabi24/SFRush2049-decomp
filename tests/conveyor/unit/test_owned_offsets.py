"""w25a: the object-offset gate (pipeline.owned_offsets). syms.ld overrides
every named definition, so the full-ROM SHA-1 cannot see a ROM TU whose
C-defined data comes out in a different order than retail; the gate compares
each object symbol's would-be address (map placement + st_value) with the
retail symbol tables."""
import struct
import subprocess
import types

import pytest

from tools.conveyor.pipeline import owned_offsets as O


def _elf(symbols, sections=(".bss",), common=()):
    """Minimal big-endian ELF32 relocatable: the given (NOBITS/PROGBITS)
    sections, a symtab with `symbols` [(name, section, value, size)] and
    COMMON symbols `common`."""
    shstr = b"\0" + b"".join(s.encode() + b"\0" for s in sections) + b".symtab\0.strtab\0.shstrtab\0"

    def shname(n):
        return shstr.index(n.encode() + b"\0")

    strtab, syms = b"\0", [b"\0" * 16]
    for name, sec, value, size in symbols:
        off = len(strtab)
        strtab += name.encode() + b"\0"
        syms.append(struct.pack(">IIIBBH", off, value, size, (1 << 4) | 1, 0, 1 + sections.index(sec)))
    for name in common:
        off = len(strtab)
        strtab += name.encode() + b"\0"
        syms.append(struct.pack(">IIIBBH", off, 4, 4, (1 << 4) | 1, 0, 0xFFF2))
    symdata = b"".join(syms)
    body = 0x34
    blobs = [symdata, strtab, shstr]
    offs, pos = [], body
    for b in blobs:
        offs.append(pos)
        pos += len(b)
    shoff = (pos + 3) & ~3
    n = len(sections)
    hdrs = [b"\0" * 40]
    for s in sections:
        hdrs.append(struct.pack(">IIIIIIIIII", shname(s), 8 if s == ".bss" else 1, 3, 0, body, 0x400, 0, 0, 16, 0))
    hdrs.append(struct.pack(">IIIIIIIIII", shname(".symtab"), 2, 0, 0, offs[0], len(symdata), n + 2, 1, 4, 16))
    hdrs.append(struct.pack(">IIIIIIIIII", shname(".strtab"), 3, 0, 0, offs[1], len(strtab), 0, 0, 1, 0))
    hdrs.append(struct.pack(">IIIIIIIIII", shname(".shstrtab"), 3, 0, 0, offs[2], len(shstr), 0, 0, 1, 0))
    ident = b"\x7fELF\x01\x02\x01" + b"\0" * 9
    head = ident + struct.pack(">HHIIIIIHHHHHH", 1, 8, 1, 0, 0, shoff, 0, 0x34, 0, 0, 40, len(hdrs), n + 3)
    data = head + b"".join(blobs)
    return data + b"\0" * (shoff - len(data)) + b"".join(hdrs)


MAP = """\
.rom_owned_bss___scMain
                0x8002e8e0      0x2d0
                0x8002e8e0                        rom_owned___scMain_START = ABSOLUTE (.)
 build/us/src/rom/lib_s.o(.bss)
 .bss           0x8002e8e0      0x2d0 build/us/src/rom/lib_s.o
                0x8002ebb0                        rom_owned___scMain_END = ABSOLUTE (.)
 build/us/src/rom/lib_t.o(.rodata)
 .rodata
                0x8002d490       0x30 build/us/src/rom/lib_t.o
 .bss           0x80050000        0x0 build/us/src/rom/lib_u.o
 .data          0x800277d0     0x4b90 build/us/assets/us/data.o
"""


def _repo(tmp_path, objects, retail):
    r = tmp_path / "repo"
    (r / "build/us/src/rom").mkdir(parents=True)
    (r / O.MAP).write_text(MAP)
    for name, data in objects.items():
        (r / f"build/us/src/rom/{name}.o").write_bytes(data)
    (r / "undefined_syms_auto.us.txt").write_text(
        "".join(f"{n} = 0x{a:08X};\n" for n, a in retail.items()))
    (r / "symbol_addrs.us.txt").write_text("// comment\n")
    return r


RETAIL = {"__osScRetraceTime": 0x8002E8E0, "__osScTaskCount": 0x8002EB70, "gViLastTick": 0x8002EBA4,
          "jtbl": 0x8002D490}


def test_map_placements_owned_marker_wrapped_lines_and_filters():
    got = O.map_placements(MAP)
    assert got == [("build/us/src/rom/lib_s.o", ".bss", 0x8002E8E0, 0x2D0, "__scMain"),
                   ("build/us/src/rom/lib_t.o", ".rodata", 0x8002D490, 0x30, None)]


def test_retail_order_passes(tmp_path):
    s = _elf([("__osScRetraceTime", ".bss", 0, 8), ("__osScTaskCount", ".bss", 0x290, 2),
              ("D_8002EB6C", ".bss", 0x28C, 4), ("gViLastTick", ".bss", 0x2C4, 4), ("helper", ".bss", 0x2C8, 4)])
    t = _elf([("jtbl", ".rodata", 0, 0x14)], sections=(".rodata",))
    r = _repo(tmp_path, {"lib_s": s, "lib_t": t}, RETAIL)
    results, problems = O.check(r)
    assert problems == []
    bss = next(x for x in results if x["section"] == ".bss")
    # D_8002EB6C is checked against its own name; `helper` has no retail address
    assert (bss["checked"], bss["unlisted"], bss["owned"]) == (4, 1, "__scMain")


def test_w24s_order_is_refused(tmp_path, capsys):
    # the w24s TU: pre-declared __osScTaskCount sorted after the fresh definitions
    s = _elf([("__osScRetraceTime", ".bss", 0, 8), ("gViLastTick", ".bss", 0x2BC, 4),
              ("__osScTaskCount", ".bss", 0x2C0, 2)])
    r = _repo(tmp_path, {"lib_s": s, "lib_t": _elf([], sections=(".rodata",))}, RETAIL)
    _, problems = O.check(r)
    assert len(problems) == 2
    assert any("__osScTaskCount: built 0x8002eba0 (+0x2c0), retail 0x8002eb70" in p for p in problems)
    assert O.main(["check", "--repo", str(r)]) == 1
    out = capsys.readouterr().out
    assert out.count(O.MARK + " MISMATCH") == 2 and (O.MARK + " FAIL") in out


def test_placeholder_name_and_common_and_tu_filter(tmp_path):
    s = _elf([("D_8002EB6C", ".bss", 0x290, 4)], common=["gStray"])
    r = _repo(tmp_path, {"lib_s": s, "lib_t": _elf([], sections=(".rodata",))}, RETAIL)
    _, problems = O.check(r)
    assert any("D_8002EB6C: built 0x8002eb70" in p for p in problems)
    assert any("COMMON symbol(s) gStray" in p for p in problems)
    assert O.check(r, tus=["src/rom/lib_t.c"])[1] == []


def test_conflicting_retail_tables_are_reported(tmp_path):
    s = _elf([("__osScTaskCount", ".bss", 0x290, 2)])
    r = _repo(tmp_path, {"lib_s": s, "lib_t": _elf([], sections=(".rodata",))}, RETAIL)
    (r / "symbol_addrs.us.txt").write_text("__osScTaskCount = 0x8002EB72; // size:2\n")
    _, problems = O.check(r)
    assert problems == ["src/rom/lib_s.c(.bss) __osScTaskCount: built 0x8002eb70 (+0x290), "
                        "retail 0x8002eb72 (symbol_addrs.us.txt)"]


def test_missing_map_fails_closed(tmp_path, capsys):
    assert O.main(["check", "--repo", str(tmp_path)]) == 1
    assert "FAIL no link map" in capsys.readouterr().out


def test_report_surfaces_lines():
    seen = []
    assert O.report("x\nOWNED-OFFSETS: OK 1 ROM TU data sections\ny", log=seen.append)
    assert seen == ["builder gate: OWNED-OFFSETS: OK 1 ROM TU data sections"]
    assert not O.report("ROM matches!", log=seen.append)


def test_promote_batch_gate_runs_the_check(monkeypatch):
    import importlib
    import sys
    from pathlib import Path
    sys.path.insert(0, str(Path("cloud/work/boot_tail_promotion").resolve()))
    pb = importlib.import_module("promote_batch")
    calls = []

    def fake_run(cmd, timeout=3600):
        calls.append(cmd)
        out = "ROM matches!\nOWNED-OFFSETS: OK 16 ROM TU data sections" if cmd[0] == "ssh" else ""
        return types.SimpleNamespace(returncode=0, stdout=out, stderr="")
    monkeypatch.setattr(pb, "run", fake_run)
    ok, _ = pb.gate([pb.REPO / "src/rom/lib_1050_sdk.c"])
    ssh = next(c for c in calls if c[0] == "ssh")[-1]
    assert ok and "make test && " + O.gate_command() + " && " in ssh


def test_promote_and_owned_storage_gates_chain_the_check():
    from pathlib import Path
    for mod in ("promote", "owned_storage"):
        text = Path(f"tools/conveyor/pipeline/{mod}.py").read_text()
        assert "owned_offsets.gate_command()" in text and "owned_offsets.report(" in text, mod


def test_cli_runs_as_module(tmp_path):
    s = _elf([("__osScRetraceTime", ".bss", 0, 8)])
    r = _repo(tmp_path, {"lib_s": s, "lib_t": _elf([], sections=(".rodata",))}, RETAIL)
    out = subprocess.run(["python3", "-m", "tools.conveyor.pipeline.owned_offsets", "check", "--repo", str(r), "-v"],
                         capture_output=True, text=True)
    assert out.returncode == 0, out.stdout + out.stderr
    assert "OWNED-OFFSETS: src/rom/lib_s.c(.bss) @0x8002e8e0+0x2d0 owned __scMain: 1 at retail" in out.stdout
    assert out.stdout.strip().splitlines()[-1].startswith("OWNED-OFFSETS: OK")


def test_promote_batch_bss_at_companion_places_definitions(tmp_path, monkeypatch):
    """--bss-at-companion: the slot's object definitions replace its companion
    (at the TU top) and are not repeated before the member's body."""
    import importlib
    import sys
    from pathlib import Path
    sys.path.insert(0, str(Path("cloud/work/boot_tail_promotion").resolve()))
    pb = importlib.import_module("promote_batch")
    row = {"owner": "f", "passthrough_asm": "asm/us/nonmatchings/rom/lib_q/f.bss.s"}
    comp = pb.owned_data.companion_block(row)
    tu = tmp_path / "src/rom/lib_q.c"
    tu.parent.mkdir(parents=True)
    tu.write_text('#include "rom_tu.h"\n' + comp + "extern s16 b;\nvoid g(void) { b = 0; }\n"
                  '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_q/f.s")\n')
    src = tmp_path / "f.c"
    src.write_text("void f(void) {\n a = 1;\n}\n")
    monkeypatch.setattr(pb, "REPO", tmp_path)
    monkeypatch.setattr(pb.owned_data, "rom_slots", lambda repo, check_source=False: [row])
    monkeypatch.setattr(pb, "BSS_AT_COMPANION", True)
    monkeypatch.setattr(pb, "PREFIX_DEDUP", {"rom/lib_q"})
    monkeypatch.setattr(pb, "extract_named_function", lambda path, fn: src.read_text())
    seg = {"rom_tu": "rom/lib_q", "flagset": "-g1 -O1"}
    cand = {"fn": "f", "seg": seg, "src": "f.c",
            "row": {"preamble": ["typedef int s32;", "s32 a;", "s16 b;", "extern void h(void);"]},
            "slot": {"tu": "src/rom/lib_q.c", "slots": [{"owner": "f", "members": ["f"], "section": ".bss"}]}}
    pb.splice([cand], out_dir=tmp_path)
    text = (tmp_path / "lib_q.c").read_text()
    assert comp not in text
    top = text.index("s32 a;\ns16 b;\n")
    assert top < text.index("void g(void)") < text.index("void f(void)")
    lines = text.splitlines()
    assert lines.count("s32 a;") == 1 and lines.count("s16 b;") == 1
    assert "extern void h(void);" in text[text.index("void g(void)"):]
    assert pb.is_object_definition("volatile OSTime t;") and not pb.is_object_definition("struct S;")
    assert not pb.is_object_definition("extern s32 x;") and not pb.is_object_definition("void f(void);")
