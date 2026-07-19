"""Contract tests for deterministic extracted-target disassembly."""
import hashlib
import struct

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import disasm


BASE = 0x80086A50


def _words(*values):
    return b"".join(struct.pack(">I", value) for value in values)


def test_objdump_normalization_labels_registers_branches_and_calls():
    raw = """
80086a50: 27bdfff8 addiu sp,sp,-8
80086a54: 11090002 beq t0,t1,80086a60
80086a58: 45030001 bc1tl 80086a60
80086a5c: 0c021ab0 jal 80086ac0
80086a60: 080246a0 j 80091a80
80086a64: 04110001 bal 80086a6c
"""
    targets = {0x80086AC0: "known_target"}

    assert disasm.normalize_objdump(raw, "sample", targets) == (
        "glabel sample\n"
        ".L80086A50:\n"
        "    addiu   $sp,$sp,-8\n"
        ".L80086A54:\n"
        "    beq     $t0,$t1,.L80086A60\n"
        ".L80086A58:\n"
        "    bc1tl   .L80086A60\n"
        ".L80086A5C:\n"
        "    jal     known_target\n"
        ".L80086A60:\n"
        "    j       func_80091a80\n"
        ".L80086A64:\n"
        "    bal     .L80086A6C\n"
    )


def test_only_committed_game_symbols_are_symbolized():
    raw = """
80086a50: 3c088011 lui t0,0x8011
80086a54: 910846ec lbu t0,0x46ec(t0)
80086a58: 3c098012 lui t1,0x8012
80086a5c: 8d291234 lw t1,0x1234(t1)
80086a60: 3c0a8012 lui t2,0x8012
80086a64: 8d4ac8e8 lw t2,-0x3718(t2)
"""
    text = disasm.normalize_objdump(raw, "globals", {})

    assert "lui     $t0,%hi(gstate)" in text
    assert "lbu     $t0,%lo(gstate)($t0)" in text
    assert "lui     $t1,0x8012" in text
    assert "lw      $t1,0x1234($t1)" in text
    assert "lui     $t2,0x8012" in text
    assert "lw      $t2,-0x3718($t2)" in text


def test_addiu_pointer_formation_updates_or_propagates_tracking():
    raw = """
80086a50: 3c088014 lui t0,0x8014
80086a54: 25082afc addiu t0,t0,11004
80086a58: 3c098014 lui t1,0x8014
80086a5c: 252a2afc addiu t2,t1,11004
80086a60: 8d292afc lw t1,11004(t1)
"""
    text = disasm.normalize_objdump(raw, "addiu_globals", {})

    assert text.count("%hi(frame_counter)") == 2
    assert "addiu   $t0,$t0,%lo(frame_counter)" in text
    assert "addiu   $t2,$t1,%lo(frame_counter)" in text
    assert "lw      $t1,%lo(frame_counter)($t1)" in text


def test_conflicting_write_invalidates_stale_lui():
    raw = """
80086a50: 3c088014 lui t0,0x8014
80086a54: 01004821 addu t1,t0,zero
80086a58: 24080000 addiu t0,zero,0
80086a5c: 8d082afc lw t0,11004(t0)
"""
    text = disasm.normalize_objdump(raw, "stale_lui", {})

    assert "lui     $t0,0x8014" in text
    assert "lw      $t0,11004($t0)" in text
    assert "%hi(" not in text
    assert "%lo(" not in text


def test_one_lui_rebinds_each_consumer_without_mismatched_pairs():
    raw = """
80086a50: 3c088011 lui t0,0x8011
80086a54: 8d0946e8 lw t1,0x46e8(t0)
80086a58: 910a46ec lbu t2,0x46ec(t0)
"""
    text = disasm.normalize_objdump(raw, "conflicting_symbols", {})

    assert text.count("%hi(game_state_flags)") == 1
    assert "lw      $t1,%lo(game_state_flags)($t0)" in text
    assert "lui     $t0,%hi(gstate)\n    lbu     $t2,%lo(gstate)($t0)" in text
    assert "%hi(game_state_flags)" in text
    assert "%lo(gstate)" in text


def test_symbol_and_numeric_consumers_restore_original_lui():
    raw = """
80086a50: 3c088003 lui t0,0x8003
80086a54: 8d09eb64 lw t1,-0x149c(t0)
80086a58: 8d0ae4aa lw t2,-0x1b56(t0)
"""
    text = disasm.normalize_objdump(raw, "numeric_co_consumer", {})

    assert text.count("%hi(game_loop_tick)") == 1
    assert "lw      $t1,%lo(game_loop_tick)($t0)" in text
    assert "lui     $t0,0x8003\n    lw      $t2,-0x1b56($t0)" in text
    assert "game_loop_tick.unk" not in text


def test_numeric_pointer_formation_then_symbolic_field_rebinds():
    raw = """
80086a50: 3c088003 lui t0,0x8003
80086a54: 2508e8e8 addiu t0,t0,-5912
80086a58: 8d09e8e8 lw t1,636(t0)
"""
    text = disasm.normalize_objdump(raw, "game_loop_shape", {})

    assert "lui     $t0,0x8003\n    addiu   $t0,$t0,-5912" in text
    assert "lui     $t0,%hi(game_loop_tick)\n    lw      $t1,%lo(game_loop_tick)($t0)" in text
    assert "game_loop_tick.unk" not in text


def test_derive_is_deterministic_and_cache_key_covers_all_inputs(
        tmp_path, monkeypatch):
    image = tmp_path / "game_code.bin"
    image.write_bytes(_words(0x27BDFFF8, 0x03E00008, 0))
    cache = tmp_path / "cache"
    conn = dbmod.connect(tmp_path / "conveyor.db")
    conn.execute(
        "INSERT INTO n64_target"
        " (target_id,address,population,insn_count,target_o_sha,tier)"
        " VALUES ('sample',?,'extracted',2,?,'raw_word')",
        (BASE, "a" * 64),
    )
    calls = []

    def fake_run(command, **kwargs):
        calls.append(command)
        start = int(command[command.index("--start-address") + 1], 0)
        count = (int(command[command.index("--stop-address") + 1], 0) - start) // 4
        lines = [f"{start + i * 4:08x}: 00000000 nop" for i in range(count)]
        return type("Result", (), {"returncode": 0, "stdout": "\n".join(lines),
                                    "stderr": ""})()

    monkeypatch.setattr(disasm.subprocess, "run", fake_run)
    first = disasm.derive(conn, "sample", image_path=image, cache_dir=cache)
    first_bytes = first.read_bytes()
    second = disasm.derive(conn, "sample", image_path=image, cache_dir=cache)
    assert second.read_bytes() == first_bytes
    assert len(calls) == 1

    conn.execute("UPDATE n64_target SET insn_count=3 WHERE target_id='sample'")
    disasm.derive(conn, "sample", image_path=image, cache_dir=cache)
    assert len(calls) == 2

    image.write_bytes(image.read_bytes() + _words(0))
    disasm.derive(conn, "sample", image_path=image, cache_dir=cache)
    assert len(calls) == 3

    monkeypatch.setattr(disasm, "symbol_table_sha",
                        lambda: hashlib.sha256(b"changed").hexdigest())
    disasm.derive(conn, "sample", image_path=image, cache_dir=cache)
    assert len(calls) == 4

    disasm.derive(conn, "sample", image_path=image, cache_dir=cache,
                  context_sha="context-a")
    assert len(calls) == 5
    disasm.derive(conn, "sample", image_path=image, cache_dir=cache,
                  context_sha="context-a")
    assert len(calls) == 5
    disasm.derive(conn, "sample", image_path=image, cache_dir=cache,
                  context_sha="context-b")
    assert len(calls) == 6


def test_indexed_addu_idiom_symbolizes_lui_and_access(monkeypatch):
    # Contract §5 idiom (c): lui + addu $d,$r,$idx + imm($d) — the lui and
    # the access rewrite; the addu stays untouched. 0x8014<<16 - 31120 =
    # 0x80138670 (a table symbol).
    out = "\n".join((
        "   0:\t3c098014 \tlui\tt9,0x8014",
        "   4:\t00047080 \tsll\tt6,a0,2",
        "   8:\t030e1821 \taddu\tv1,t8,t9",
        "   c:\t8c6f8670 \tlw\tt7,-31120(v1)",
        "  10:\t03e00008 \tjr\tra",
        "  14:\t00000000 \tnop",
    ))
    text = disasm.normalize_objdump(out, "t", {})
    assert "lui     $t9,%hi(D_80138670)" in text
    assert "addu    $v1,$t8,$t9" in text
    assert "lw      $t7,%lo(D_80138670)($v1)" in text


def test_indexed_addu_idiom_dest_equals_base(monkeypatch):
    out = "\n".join((
        "   0:\t3c098014 \tlui\tt9,0x8014",
        "   4:\t03287821 \taddu\tt9,t9,t8",
        "   8:\t8f2f8670 \tlw\tt7,-31120(t9)",
        "   c:\t03e00008 \tjr\tra",
        "  10:\t00000000 \tnop",
    ))
    text = disasm.normalize_objdump(out, "t", {})
    assert "lui     $t9,%hi(D_80138670)" in text
    assert "lw      $t7,%lo(D_80138670)($t9)" in text


def test_indexed_addu_untabled_address_stays_numeric():
    # Same idiom, but the formed address is not in the table: nothing rewrites.
    out = "\n".join((
        "   0:\t3c091234 \tlui\tt9,0x1234",
        "   4:\t030e1821 \taddu\tv1,t8,t9",
        "   8:\t8c6f0010 \tlw\tt7,16(v1)",
        "   c:\t03e00008 \tjr\tra",
        "  10:\t00000000 \tnop",
    ))
    text = disasm.normalize_objdump(out, "t", {})
    assert "%hi" not in text and "%lo" not in text
    assert "lui     $t9,0x1234" in text
    assert "lw      $t7,16($v1)" in text


def test_formed_pointer_does_not_propagate_through_addu():
    # A fully-formed lui+addiu pointer must not tunnel through addu (only
    # raw-page luis do, per idiom (c)).
    out = "\n".join((
        "   0:\t3c098014 \tlui\tt9,0x8014",
        "   4:\t27398670 \taddiu\tt9,t9,-31120",
        "   8:\t03287821 \taddu\tt9,t9,t8",
        "   c:\t8f2f0000 \tlw\tt7,0(t9)",
        "  10:\t03e00008 \tjr\tra",
        "  14:\t00000000 \tnop",
    ))
    text = disasm.normalize_objdump(out, "t", {})
    assert "lw      $t7,0($t9)" in text
