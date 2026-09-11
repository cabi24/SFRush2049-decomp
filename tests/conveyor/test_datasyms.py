"""007 generated data-symbol contract tests (contracts §7-§10)."""
import hashlib
import json

from tools.conveyor.pipeline import datasyms, disasm


# --- §8 width rule -------------------------------------------------------------

def test_width_rule_widest_wins_and_signedness_from_loads():
    assert datasyms.choose_type(["lbu", "sb"]) == ("u8", [])
    assert datasyms.choose_type(["lb", "lbu"]) == ("s8", [])
    assert datasyms.choose_type(["sh"]) == ("s16", [])
    assert datasyms.choose_type(["lhu", "lbu"]) == ("u16", [])
    assert datasyms.choose_type(["lbu", "lw", "sh"]) == ("s32", [])
    assert datasyms.choose_type(["ld", "lw"]) == ("s64", [])


def test_width_rule_fp_only_and_conflicts():
    assert datasyms.choose_type(["lwc1", "swc1"]) == ("f32", [])
    assert datasyms.choose_type(["ldc1"]) == ("f64", [])
    # same-width int/fp: conflict recorded, integer wins
    assert datasyms.choose_type(["lwc1", "lw"]) == ("s32", ["int32_vs_fp32"])
    # wider fp than int: widest wins, conflict still recorded
    assert datasyms.choose_type(["ldc1", "lbu"]) == ("f64", ["int8_vs_fp64"])
    # wider int than fp
    assert datasyms.choose_type(["sd", "lwc1"]) == ("s64", ["int64_vs_fp32"])


def test_formation_only_entries_type_s32_and_say_so():
    assert datasyms.choose_type([]) == ("s32", ["formation_only"])


# --- §7 scan via the symbolizer's own observations ----------------------------

def _obs(vaddr, mnemonic, address, kind="access", symbol=None):
    return {"vaddr": vaddr, "mnemonic": mnemonic, "address": address,
            "kind": kind, "symbol": symbol}


def test_build_table_cites_accesses_skips_hand_named_and_is_order_independent():
    scans_a = [
        ("f1", [_obs(0x80090000, "lw", 0x80152000),
                _obs(0x80090010, "lbu", 0x80152000),
                _obs(0x80090020, "addiu", 0x80153000, kind="formation"),
                _obs(0x80090030, "lbu", 0x801146EC, symbol="gstate")]),
        ("f0", [_obs(0x80088000, "swc1", 0x80152000),
                _obs(0x80088004, "lw", 0x80152000)]),      # duplicate site
    ]
    hand = {0x801146EC: "gstate"}
    symbols, omitted = datasyms.build_table(scans_a, hand)
    assert list(symbols) == ["80152000", "80153000"]
    assert symbols["80152000"] == {
        "name": "D_80152000", "type": "s32",
        "accesses": [
            {"target": "f0", "label": ".L80088000", "mnemonic": "swc1"},
            {"target": "f0", "label": ".L80088004", "mnemonic": "lw"},
            {"target": "f1", "label": ".L80090000", "mnemonic": "lw"},
            {"target": "f1", "label": ".L80090010", "mnemonic": "lbu"}],
        "formations": [], "conflicts": ["int32_vs_fp32"]}
    assert symbols["80153000"]["type"] == "s32"
    assert symbols["80153000"]["conflicts"] == ["formation_only"]
    assert symbols["80153000"]["formations"] == [
        {"target": "f1", "label": ".L80090020", "mnemonic": "addiu"}]
    assert omitted == {}
    # byte-stability: reversed scan order renders identically
    symbols_b, omitted_b = datasyms.build_table(list(reversed(scans_a)), hand)
    stamp = {"image_sha": "x"}
    assert datasyms.render(symbols, omitted, stamp) == datasyms.render(
        symbols_b, omitted_b, stamp)


def test_build_table_omits_hand_collisions_constants_and_function_pointers():
    scans = [("f", [
        _obs(0x80090000, "lw", 0x80142AFC),                       # frame_counter
        _obs(0x80090004, "addiu", 0x00039B40, kind="formation"),  # integer constant
        _obs(0x80090008, "lwc1", 0x3F0039E0),                     # float immediate
        _obs(0x8009000C, "addiu", 0x8008A77C, kind="formation"),  # function pointer
        _obs(0x80090010, "addiu", 0x80124C30, kind="formation"),  # real bss
    ])]
    symbols, omitted = datasyms.build_table(
        scans, {0x80142AFC: "frame_counter"}, function_addresses={0x8008A77C})
    assert list(symbols) == ["80124C30"]
    assert omitted == {
        "00039B40": {"reason": "not_ram"},
        "3F0039E0": {"reason": "not_ram"},
        "8008A77C": {"reason": "function_address"},
        "80142AFC": {"reason": "hand_table", "name": "frame_counter"},
    }


def test_normalize_observations_cover_the_three_idioms_with_hand_only_table():
    raw = "\n".join((
        "80090000: 3c088015 lui t0,0x8015",
        "80090004: 8d092000 lw t1,0x2000(t0)",            # direct access
        "80090008: 3c0a8015 lui t2,0x8015",
        "8009000c: 254a3000 addiu t2,t2,0x3000",          # formation
        "80090010: 3c0b8015 lui t3,0x8015",
        "80090014: 00046080 sll t4,a0,2",
        "80090018: 018b6821 addu t5,t4,t3",
        "8009001c: 8dae4000 lw t6,0x4000(t5)",            # indexed idiom (c)
        "80090020: 3c0c8011 lui t4,0x8011",
        "80090024: 918c46ec lbu t4,0x46ec(t4)",           # hand: gstate
        "80090028: 8d4f0008 lw t7,8(t2)",                 # field off formed ptr: not a symbol
    ))
    observations = []
    text = disasm.normalize_objdump(raw, "t", {}, symbols=disasm.GAME_SYMBOLS,
                                    observations=observations)
    assert [(o["address"], o["kind"], o["mnemonic"], o["symbol"]) for o in observations] == [
        (0x80152000, "access", "lw", None),
        (0x80153000, "formation", "addiu", None),
        (0x80154000, "access", "lw", None),
        (0x801146EC, "access", "lbu", "gstate"),
    ]
    assert "lw      $t1,0x2000($t0)" in text        # untabled stays numeric


# --- §10 merged lookup + cache key ---------------------------------------------

def test_merged_lookup_hand_wins_and_generated_symbolizes(tmp_path, monkeypatch):
    layer = tmp_path / "m2c_datasyms.json"
    layer.write_text(json.dumps({"symbols": {
        "80152000": {"name": "D_80152000", "type": "s32"},
        "801146EC": {"name": "D_801146EC", "type": "u8"},   # collides with gstate
    }}))
    monkeypatch.setattr(disasm, "DATASYMS_JSON", layer)
    monkeypatch.setattr(disasm, "_generated_cache", {})
    table = disasm.symbol_table()
    assert table[0x80152000] == "D_80152000"
    assert table[0x801146EC] == "gstate"                    # hand wins
    raw = "\n".join((
        "80090000: 3c088015 lui t0,0x8015",
        "80090004: 8d092000 lw t1,0x2000(t0)",
        "80090008: 3c0c8011 lui t4,0x8011",
        "8009000c: 918c46ec lbu t4,0x46ec(t4)",
    ))
    text = disasm.normalize_objdump(raw, "t", {})
    assert "lui     $t0,%hi(D_80152000)" in text
    assert "lw      $t1,%lo(D_80152000)($t0)" in text
    assert "lbu     $t4,%lo(gstate)($t4)" in text


def test_symbol_table_sha_changes_with_generated_layer_content(tmp_path, monkeypatch):
    layer = tmp_path / "m2c_datasyms.json"
    monkeypatch.setattr(disasm, "DATASYMS_JSON", layer)
    monkeypatch.setattr(disasm, "_generated_cache", {})
    without = disasm.symbol_table_sha()
    hand_only = hashlib.sha256(json.dumps(
        sorted(disasm.GAME_SYMBOLS.items()), separators=(",", ":")).encode()).hexdigest()
    assert without == hand_only
    layer.write_text(json.dumps({"symbols": {"80152000": {"name": "D_80152000", "type": "s32"}}}))
    with_layer = disasm.symbol_table_sha()
    assert with_layer != without
    layer.write_text(json.dumps({"symbols": {"80152004": {"name": "D_80152004", "type": "s32"}}}))
    assert disasm.symbol_table_sha() not in (without, with_layer)
    # derive()'s cache key is symbol_table_sha (covered by test_disasm's
    # cache-key test), so a changed generated table regenerates derivations.
