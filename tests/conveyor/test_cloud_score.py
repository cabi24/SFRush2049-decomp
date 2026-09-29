"""Strict cloud relocation checks, plus IDO regressions for committed matches."""
import json
import shutil
import struct
from pathlib import Path

import pytest

from tools.cloud import score


def _object(tmp_path, words, symbols, rels):
    """Minimal ELF32/MSB object; section and symbol indices are independent."""
    names = ["", ".text", ".rel.text", ".symtab", ".strtab", ".shstrtab", ".rodata"]
    shstrings = b"\0"
    offsets = [0]
    for name in names[1:]:
        offsets.append(len(shstrings))
        shstrings += name.encode() + b"\0"
    strings = b"\0"
    symbytes = bytes(16)
    indices = {}
    for index, (name, value, typ, section, size) in enumerate(symbols, 1):
        indices[name] = index
        # Section symbols commonly have st_name == 0.
        nameoff = 0 if typ == 3 else len(strings)
        if typ != 3:
            strings += name.encode() + b"\0"
        symbytes += struct.pack(">IIIBBH", nameoff, value, size, typ, 0, section)
    relbytes = b"".join(struct.pack(">II", offset, indices[name] << 8 | typ)
                        for offset, name, typ in rels)
    text = struct.pack(f">{len(words)}I", *words)
    sections = [b"", text, relbytes, symbytes, strings, shstrings, bytes(32)]
    types = [0, 1, 9, 2, 3, 3, 1]
    data = bytearray(52)
    headers = []
    for index, raw in enumerate(sections):
        data.extend(bytes(-len(data) % 4))
        headers.append(struct.pack(">10I", offsets[index], types[index], 0, 0,
                                   len(data), len(raw),
                                   {2: 3, 3: 4}.get(index, 0), 1 if index == 2 else 0,
                                   4, {2: 8, 3: 16}.get(index, 0)))
        data.extend(raw)
    data.extend(bytes(-len(data) % 4))
    shoff = len(data)
    data.extend(b"".join(headers))
    data[:16] = b"\x7fELF\x01\x02\x01" + bytes(9)
    struct.pack_into(">HHIIIIIHHHHHH", data, 16,
                     1, 8, 1, 0, 0, shoff, 0, 52, 0, 0, 40, len(sections), 5)
    path = tmp_path / "fixture.o"
    path.write_bytes(data)
    return path


def _compare(monkeypatch, obj, want, addresses=None):
    monkeypatch.setattr(score, "_targets", {"f": want})
    monkeypatch.setattr(score, "image_symbols", lambda: addresses or {})
    return score.compare(obj, "f", show=0)


def test_named_call_resolves_full_address_and_addend(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x0C000001],
                  [("f", 0, 2, 1, 4), ("callee", 0, 2, 0, 0)], [(0, "callee", 4)])
    result = _compare(monkeypatch, obj, [0x0C004001], {"callee": 0x80010000})
    assert result.accepted() and result.summary() == "MATCH"
    result = _compare(monkeypatch, obj, [0x0C004001], {"callee": 0x80020000})
    assert result.differing == 1 and not result.accepted(True)


def test_address_spelling_fallback_and_table_precedence(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x0C000000],
                  [("f", 0, 2, 1, 4), ("func_80010000", 0, 2, 0, 0)],
                  [(0, "func_80010000", 4)])
    assert _compare(monkeypatch, obj, [0x0C004000]).accepted()
    assert not _compare(monkeypatch, obj, [0x0C004000],
                        {"func_80010000": 0x80020000}).accepted()
    assert score.address_named("D_80116de4") == 0x80116DE4
    assert score.address_named("func_80010000_suffix") is None


def test_unresolved_callee_cannot_pass_even_when_words_equal(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x0C000000],
                  [("f", 0, 2, 1, 4), ("unknown", 0, 2, 0, 0)], [(0, "unknown", 4)])
    result = _compare(monkeypatch, obj, [0x0C000000])
    assert not result.accepted(True) and result.differing == 0
    assert "unresolved symbols: unknown" in result.summary()
    assert "MATCH" not in result.summary()


def test_hi_lo_signed_addend_and_carry(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x3C040001, 0x2484FFF0],
                  [("f", 0, 2, 1, 8), ("data", 0, 1, 0, 0)],
                  [(0, "data", 5), (4, "data", 6)])
    # 0x80108000 + 0x10000 - 16 = 0x80117ff0 (no LO carry).
    assert _compare(monkeypatch, obj, [0x3C048011, 0x24847FF0],
                    {"data": 0x80108000}).accepted()
    # 0x80108020 + 0x10000 - 16 = 0x80118010 (HI carries).
    assert _compare(monkeypatch, obj, [0x3C048012, 0x24848010],
                    {"data": 0x80108020}).accepted()


def test_each_pending_hi_uses_its_own_addend_and_symbol(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x3C040001, 0x3C050002, 0x3C060000, 0x2484FFF0, 0x24C60020],
                  [("f", 0, 2, 1, 20), ("a", 0, 1, 0, 0), ("b", 0, 1, 0, 0)],
                  [(0, "a", 5), (4, "a", 5), (8, "b", 5), (12, "a", 6), (16, "b", 6)])
    assert _compare(monkeypatch, obj,
                    [0x3C048011, 0x3C058012, 0x3C068014, 0x24847FF0, 0x24C60020],
                    {"a": 0x80108000, "b": 0x80140000}).accepted()


def test_lo_without_hi_resolves_signed_addend(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x2484FFF0],
                  [("f", 0, 2, 1, 4), ("D_80110000", 0, 1, 0, 0)],
                  [(0, "D_80110000", 6)])
    assert _compare(monkeypatch, obj, [0x2484FFF0]).accepted()


@pytest.mark.parametrize("typ,message", [(5, "unpaired R_MIPS_HI16"),
                                         (2, "unsupported relocation type 2")])
def test_incomplete_or_unsupported_relocations_fail(tmp_path, monkeypatch, typ, message):
    obj = _object(tmp_path, [0x3C040000],
                  [("f", 0, 2, 1, 4), ("data", 0, 1, 0, 0)], [(0, "data", typ)])
    result = _compare(monkeypatch, obj, [0x3C040000], {"data": 0x80100000})
    assert not result.accepted(True)
    assert message in result.summary()


def test_data_section_references_remain_explicitly_unverified(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x3C040000, 0x24840010],
                  [("f", 0, 2, 1, 8), (".rodata", 0, 3, 6, 32)],
                  [(0, ".rodata", 5), (4, ".rodata", 6)])
    result = _compare(monkeypatch, obj, [0x3C048010, 0x24841234])
    assert result.differing == 0 and len(result.unverified) == 2
    assert not result.accepted() and result.accepted(True)
    assert "2 section-relative relocations unverified" in result.summary()
    assert ".rodata+0x10" in result.summary()
    result = _compare(monkeypatch, obj, [0x3C048010, 0x24A51234])
    assert result.differing == 1 and not result.accepted(True)


def test_text_section_reference_maps_to_group_member_not_object_base(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x0C000003, 0, 0, 0],
                  [("f", 0, 2, 1, 8), ("g", 8, 2, 1, 8), (".text", 0, 3, 1, 16)],
                  [(0, ".text", 4)])
    # .text + 12 is g + 4, regardless of g's position in the group object.
    result = _compare(monkeypatch, obj, [0x0C080001], {"f": 0x80100000, "g": 0x80200000})
    assert result.accepted() and not result.unverified
    result = _compare(monkeypatch, obj, [0x0C080001], {"f": 0x80100000})
    assert result.unresolved and not result.accepted(True)


def test_named_defined_function_uses_its_image_address(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x0C000001, 0, 0, 0],
                  [("f", 0, 2, 1, 8), ("g", 8, 2, 1, 8)], [(0, "g", 4)])
    assert _compare(monkeypatch, obj, [0x0C080001], {"g": 0x80200000}).accepted()


def test_other_functions_relocations_do_not_change_this_result(tmp_path, monkeypatch):
    obj = _object(tmp_path, [0x03E00008, 0x0C000000],
                  [("f", 0, 2, 1, 4), ("unknown", 0, 2, 0, 0)], [(4, "unknown", 4)])
    assert _compare(monkeypatch, obj, [0x03E00008]).accepted()


def test_missing_symbol_table_is_a_clear_failure(tmp_path, monkeypatch):
    monkeypatch.setattr(score, "ASM_DIR", tmp_path)
    with pytest.raises(SystemExit, match="cannot read symbol table"):
        score.image_symbols()


@pytest.mark.parametrize("unknown,allow,expected", [(False, False, 1), (False, True, 0),
                                                   (True, False, 1), (True, True, 1)])
def test_cli_unverified_opt_in_does_not_allow_unknown_symbols(
        tmp_path, monkeypatch, capsys, unknown, allow, expected):
    name, typ, section = ("unknown", 1, 0) if unknown else (".rodata", 3, 6)
    obj = _object(tmp_path, [0x24840010], [("f", 0, 2, 1, 4), (name, 0, typ, section, 0)],
                  [(0, name, 6)])
    monkeypatch.setattr(score, "_targets", {"f": [0x24840010]})
    monkeypatch.setattr(score, "image_symbols", lambda: {})
    monkeypatch.setattr(score, "compile_single", lambda source, flags, out: shutil.copyfile(obj, out))
    monkeypatch.setattr("sys.argv", ["score.py", "fn", "dummy.c", "f"]
                        + (["--allow-unverified"] if allow else []))
    assert score.main() == expected
    output = capsys.readouterr().out
    assert "unresolved" in output if unknown else "unverified" in output


requires_ido = pytest.mark.skipif(not (score.IDO / "cc").is_file(),
                                  reason="needs tools/cloud/ido/cc (or IDO_DIR)")
LOCK = json.loads((score.REPO / "blob_matched.lock.json").read_text())


@requires_ido
@pytest.mark.parametrize("name,entry", [(name, entry) for name, entry in LOCK.items()
                                        if "group" not in entry], ids=[name for name, entry
                                        in LOCK.items() if "group" not in entry])
def test_locked_single_function_is_strict_match(tmp_path, name, entry):
    obj = tmp_path / "out.o"
    score.compile_single(score.REPO / entry["source"], entry["flagset"], obj)
    result = score.compare(obj, name)
    assert result.accepted(), result.summary()
    assert result.summary() == "MATCH"


@requires_ido
@pytest.mark.parametrize("group", ["resource_slot_clear", "entity_flag_check"])
def test_locked_group_members_are_strict_matches(tmp_path, group, monkeypatch):
    # F7 will fix compile_group's lifetime; contain its current scratch dirs.
    monkeypatch.setattr(score.tempfile, "tempdir", str(tmp_path))
    obj = tmp_path / "out.o"
    spec = score.compile_group(score.REPO / "src/blob/groups" / group, obj)
    for member in spec["members"]:
        result = score.compare(obj, member)
        assert result.accepted(), f"{member}: {result.summary()}"
        assert result.summary() == "MATCH"


@requires_ido
@pytest.mark.parametrize("mutant", ["addend", "global", "callee", "known_global"])
def test_wrong_sound_handles_clear_fails(tmp_path, mutant):
    source = (score.REPO / "src/blob/sound_handles_clear.c").read_text()
    if mutant == "addend":
        source = source.replace("entry = (s8 *) &D_80116DE4;", "entry = (s8 *) &D_80116DE4 + 0x40;")
    elif mutant in ("global", "known_global"):
        name = "D_WRONG" if mutant == "global" else "D_80116FE4"
        if mutant == "global":
            source = source.replace("void sound_handles_clear(s32 arg0)",
                                    f"extern s8 {name};\nvoid sound_handles_clear(s32 arg0)")
        source = source.replace("entry = (s8 *) &D_80116DE4;", f"entry = (s8 *) &{name};")
    else:
        source = source.replace("void sound_handles_clear(s32 arg0)",
                                "extern void some_other_fn(s32);\nvoid sound_handles_clear(s32 arg0)")
        source = source.replace("sound_stop(h);", "some_other_fn(h);")
    src, obj = tmp_path / "mutant.c", tmp_path / "out.o"
    src.write_text(source)
    score.compile_single(src, LOCK["sound_handles_clear"]["flagset"], obj)
    result = score.compare(obj, "sound_handles_clear")
    assert not result.accepted(True), result.summary()
    if mutant in ("global", "callee"):
        assert result.unresolved
    else:
        assert result.differing > 0
