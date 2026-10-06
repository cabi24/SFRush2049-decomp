"""Call-graph frontier: layers, recipes, joint units and unlock values."""
import struct

from tools.conveyor.pipeline import frontier


def _jal(target):
    return (3 << 26) | ((target & 0x0FFFFFFF) >> 2)


def test_call_graph_reads_jal_words_and_ignores_internal_jumps():
    base = 0x80086A50
    funcs = [(base, 16, "a"), (base + 16, 8, "b")]
    j_inside = (2 << 26) | (((base + 8) & 0x0FFFFFFF) >> 2)
    words = [_jal(base + 16), 0, j_inside, _jal(0x80001234), 0, 0]
    image = struct.pack(">6I", *words)
    calls, external = frontier.call_graph(funcs, image, base)
    assert calls == {"a": {"b"}, "b": set()}
    assert external == {"a": {0x80001234}, "b": set()}


def test_layers_follow_unmatched_callees_only():
    calls = {"top": {"mid", "done"}, "mid": {"leaf"}, "leaf": set(), "done": set()}
    size = {"top": 400, "mid": 200, "leaf": 100, "done": 50}
    facts = frontier.analyze(calls, size, {"done"})
    assert {n: f["layer"] for n, f in facts.items()} == {"leaf": 1, "mid": 2, "top": 3}
    assert facts["leaf"]["ready"] and not facts["mid"]["ready"]
    assert facts["mid"]["blockers"] == ["leaf"]
    assert frontier.layer_table(facts) == [(1, 1, 100), (2, 1, 200), (3, 1, 400)]


def test_unlock_value_counts_sole_blockers_and_transitive_dependents():
    calls = {"hub": set(), "x": {"hub"}, "y": {"hub", "other"}, "other": set(),
             "z": {"x"}}
    size = {"hub": 10, "x": 100, "y": 200, "other": 20, "z": 400}
    facts = frontier.analyze(calls, size, set())
    assert facts["hub"]["sole_blocker_for"] == ["x"]
    assert facts["hub"]["sole_unlock_bytes"] == 100
    assert facts["hub"]["dependents"] == 3
    assert facts["hub"]["dependent_bytes"] == 700
    assert frontier.hubs(facts)[0][0] == "hub"


def test_recipes_from_the_ipa_scan():
    calls = {"callee": set(), "setter": {"callee"}, "keeper": {"plain"},
             "plain": set()}
    size = dict.fromkeys(calls, 40)
    ipa = {"callees": {"callee": ["t0", "a0"]}, "preservers": {"keeper": ["a1"]},
           "callers": {"setter": ["callee"]}}
    facts = frontier.analyze(calls, size, set(), ipa)
    assert facts["callee"]["recipe"] == "unit"
    assert facts["callee"]["register_params"] == ["t0"]
    assert facts["callee"]["unit"] == ["callee", "setter"]
    assert facts["setter"]["recipe"] == "group"
    assert facts["keeper"]["recipe"] == "group"
    assert facts["plain"]["recipe"] == "single"


def test_unit_readiness_needs_every_outside_callee_locked():
    calls = {"callee": set(), "setter": {"callee", "far"}, "far": set()}
    size = dict.fromkeys(calls, 40)
    ipa = {"callees": {"callee": ["s0"]}, "callers": {"setter": ["callee"]}}
    open_facts = frontier.analyze(calls, size, set(), ipa)
    assert not open_facts["callee"]["unit_ready"]
    assert open_facts["callee"]["unit_blockers"] == ["far"]
    closed = frontier.analyze(calls, size, {"far"}, ipa)
    assert closed["callee"]["unit_ready"]
    assert [n for n, _ in frontier.queue(closed, "unit")] == ["callee"]


def test_unit_is_one_hop_and_component_is_transitive():
    calls = {"c1": set(), "c2": set(), "s1": {"c1"}, "s2": {"c1", "c2"}, "s3": {"c2"}}
    size = dict.fromkeys(calls, 40)
    ipa = {"callees": {"c1": ["t0"], "c2": ["t1"]},
           "callers": {"s1": ["c1"], "s2": ["c1", "c2"], "s3": ["c2"]}}
    facts = frontier.analyze(calls, size, set(), ipa)
    assert facts["c1"]["unit"] == ["c1", "s1", "s2"]
    assert facts["s1"]["unit"] == ["c1", "s1"]
    assert facts["c1"]["component"] == 5


def test_recursion_does_not_hang():
    calls = {"a": {"b"}, "b": {"a"}}
    facts = frontier.analyze(calls, {"a": 8, "b": 8}, set())
    assert set(facts) == {"a", "b"}


def test_evidence_lists_unclaimed_group_context(tmp_path):
    group = tmp_path / "groups" / "g1"
    group.mkdir(parents=True)
    (group / "group.json").write_text('{"members": ["m"], "context": ["hub"]}')
    matches = tmp_path / "matches"
    matches.mkdir()
    (matches / "hub.c").write_text("")
    found = frontier.evidence(["hub", "none"], group_roots=(tmp_path / "groups",),
                              cloud_matches=matches, near_miss=tmp_path / "nm")
    assert found == {"hub": sorted([str(group), str(matches / "hub.c")])}


def test_unsaved_callee_saved_write_needs_a_group():
    move_s0_a0 = (4 << 21) | (16 << 11) | 0x25          # or $s0,$a0,$zero
    save_s0 = (43 << 26) | (29 << 21) | (16 << 16) | 20  # sw $s0,20($sp)
    assert frontier.unsaved_callee_writes([move_s0_a0]) == ["s0"]
    assert frontier.unsaved_callee_writes([save_s0, move_s0_a0]) == []
    facts = frontier.analyze({"f": set()}, {"f": 8}, set(), None, {"f": ["s0"]})
    assert facts["f"]["recipe"] == "group"


# --- word builders -----------------------------------------------------------
T0, T5, T6, T7, T8, T9, RA, SP, A0, V0 = 8, 13, 14, 15, 24, 25, 31, 29, 4, 2
JR_RA, NOP = 0x03E00008, 0


def _i(op, rs, rt, imm=0):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def _lw(rt, base=A0, off=0):
    return _i(35, base, rt, off)


def _sw(rt, base=V0, off=0):
    return _i(43, base, rt, off)


def _ring_words(regs):
    return [_lw(r) for r in regs]


# --- provisional set ---------------------------------------------------------
def test_provisional_callee_satisfies_its_callers_but_is_not_matched():
    calls = {"inner": set(), "caller": {"inner"}, "top": {"caller"}}
    size = {"inner": 40, "caller": 400, "top": 80}
    plain = frontier.analyze(calls, size, set())
    assert not plain["caller"]["ready"] and plain["caller"]["layer"] == 2
    facts = frontier.analyze(calls, size, set(), provisional={"inner"})
    assert set(facts) == {"inner", "caller", "top"}          # still unmatched
    assert facts["inner"]["provisional"] and not facts["caller"]["provisional"]
    assert facts["caller"]["ready"] and facts["caller"]["layer"] == 1
    assert facts["caller"]["blockers"] == []
    assert facts["caller"]["provisional_callees"] == ["inner"]
    assert facts["top"]["layer"] == 2
    assert [n for n, _ in frontier.queue(facts)] == ["caller"]   # not `inner`


def test_provisional_entry_is_ignored_once_locked_or_unknown():
    calls = {"inner": set(), "caller": {"inner"}}
    facts = frontier.analyze(calls, {"inner": 8, "caller": 8}, {"inner"},
                             provisional={"inner", "ghost"})
    assert set(facts) == {"caller"}
    assert facts["caller"]["provisional_callees"] == []


def test_provisional_callee_does_not_block_a_unit():
    calls = {"callee": set(), "setter": {"callee", "far"}, "far": set()}
    ipa = {"callees": {"callee": ["s0"]}, "callers": {"setter": ["callee"]}}
    facts = frontier.analyze(calls, dict.fromkeys(calls, 40), set(), ipa,
                             provisional={"far"})
    assert facts["callee"]["unit_ready"]


def test_load_provisional_tolerates_missing_and_malformed_files(tmp_path):
    assert frontier.load_provisional(tmp_path / "absent.json") == {}
    bad = tmp_path / "bad.json"
    bad.write_text("{not json")
    assert frontier.load_provisional(bad) == {}
    good = tmp_path / "good.json"
    good.write_text('{"f": {"evidence": "dir", "note": "n"}, "_comment": "x", "g": 3}')
    assert frontier.load_provisional(good) == {"f": {"evidence": "dir", "note": "n"}}


def test_seeded_provisional_file_names_the_wave0_standins():
    seeded = frontier.load_provisional()
    # func_800D1AB0 was a wave-0 stand-in proof; it closed for real in wave 3
    # (frontier_car_checkpoints) and left the file.
    # func_80099B30 closed for real in the particle-knot claim (2026-10-05).
    assert {"func_800B66B0"} <= set(seeded)
    assert "func_80099B30" not in seeded
    assert "func_800D1AB0" not in seeded
    assert "func_80091B00" not in seeded
    for entry in seeded.values():
        assert (frontier.REPO / entry["evidence"]).is_dir()


# --- temp ring ---------------------------------------------------------------
def test_ring_wrap_without_low_temps_is_the_strict_arm():
    info = frontier.temp_ring(_ring_words([T6, T7, T8, T9, T6, T7]))
    assert info == {"writes": 6, "wraps": 1, "low_written": []}
    assert frontier.ring_arms(info) == ["strict"]


def test_ten_wide_ring_of_a_standalone_compile_is_not_flagged():
    info = frontier.temp_ring(_ring_words([T6, T7, T8, T9, T5, T6, T7, T8, T9, T5]))
    assert info["low_written"] == ["t5"]
    assert frontier.ring_arms(info) == []                   # t5 in use: neither arm


def test_t5_arm_needs_three_wraps_and_an_untouched_t5():
    lap = [T6, T7, T8, T9]
    pool = [_i(9, A0, T0, 8)]                               # addiu t0,a0,8: a pool variable
    two = frontier.temp_ring(pool + _ring_words(lap * 3))
    assert two["wraps"] == 2 and frontier.ring_arms(two) == []
    three = frontier.temp_ring(pool + _ring_words(lap * 4))
    assert three["low_written"] == ["t0"]
    assert frontier.ring_arms(three) == ["t5"]
    with_t5 = frontier.temp_ring([_lw(T5)] + _ring_words(lap * 4))
    assert frontier.ring_arms(with_t5) == []


def test_branch_likely_delay_slot_copy_is_not_a_ring_step():
    bnezl = _i(21, A0, 0, 4)
    words = [bnezl, _lw(T9)] + _ring_words([T6, T7, T8, T9])
    assert frontier.temp_ring(words)["wraps"] == 0
    bne = _i(5, A0, 0, 4)
    assert frontier.temp_ring([bne, _lw(T9)] + _ring_words([T6]))["wraps"] == 1


def test_ring_moves_a_function_to_group_and_hints_do_not():
    calls = {"ringed": set(), "hinted": set()}
    size = dict.fromkeys(calls, 40)
    ring = {"ringed": {"writes": 6, "wraps": 1, "low_written": [], "arms": ["strict"]}}
    facts = frontier.analyze(calls, size, set(), rings=ring,
                             hints={"hinted": ["inlined_callee@0x80000000:ra,t5,t4"]})
    assert facts["ringed"]["recipe"] == "group"
    assert facts["ringed"]["signatures"] == ["temp_ring"]
    assert facts["hinted"]["recipe"] == "single"
    assert facts["hinted"]["signatures"] == []
    assert facts["hinted"]["hints"] == ["inlined_callee@0x80000000:ra,t5,t4"]


# --- inlined callee ----------------------------------------------------------
# Input_ApplyPadConfig, words 0..22 (Input_InitPadHandlers inlined).
_APPLY_PAD_CONFIG = [
    0x27BDFFD8, 0xAFBF0024, 0xAFB00020, 0x94830034, 0x3C0F8014, 0x25EF0BF0,
    0x00037140, 0x849F0016, 0x848D0014, 0x848C0010, 0x848B000E, 0x8C8A0004,
    0x8C890008, 0x9488000C, 0x01CF1021, 0x00808025, 0xA45F0012, 0xA44D0010,
    0xA44C000C, 0xA44B000A, 0xAC4A0000, 0xAC490004, 0xA4480008]


def test_inlined_callee_run_is_found_in_the_retail_example():
    runs = frontier.inlined_callee_runs(_APPLY_PAD_CONFIG)
    assert runs == [(7, ["ra", "t5", "t4", "t3", "t2", "t1", "t0"])]


def test_inlined_callee_needs_descending_loads_then_matching_stores():
    desc = [_lw(RA), _lw(T5), _lw(12)]
    assert frontier.inlined_callee_runs(desc + [_sw(RA), _sw(T5), _sw(12)]) == [
        (0, ["ra", "t5", "t4"])]
    # ascending loads, a run of two, stores in another order, stack spills
    assert frontier.inlined_callee_runs([_lw(12), _lw(T5), _lw(RA),
                                         _sw(12), _sw(T5), _sw(RA)]) == []
    assert frontier.inlined_callee_runs([_lw(RA), _lw(T5), _sw(RA), _sw(T5)]) == []
    assert frontier.inlined_callee_runs(desc + [_sw(12), _sw(T5), _sw(RA)]) == []
    assert frontier.inlined_callee_runs(
        desc + [_sw(RA, SP), _sw(T5, SP), _sw(12, SP)]) == []
    assert frontier.inlined_callee_runs(desc) == []


# --- stubs -------------------------------------------------------------------
def _image(base, funcs, bodies, tail=()):
    words = []
    for (_, size, name) in funcs:
        body = list(bodies[name])
        assert len(body) * 4 == size
        words += body
    words += list(tail)
    return struct.pack(f">{len(words)}I", *words)


def test_stubs_lists_callerless_jr_ra_bodies_with_neighbours():
    base = 0x80086A50
    funcs = [(base, 16, "caller"), (base + 16, 8, "dead"), (base + 24, 8, "live"),
             (base + 32, 8, "tiny"), (base + 44, 8, "far")]
    bodies = {"caller": [_jal(base + 24), NOP, JR_RA, NOP], "dead": [JR_RA, NOP],
              "live": [JR_RA, NOP], "tiny": [JR_RA, _i(9, 0, V0, 1)]}
    image = _image(base, funcs[:4], bodies, tail=[base + 44, JR_RA, NOP])
    calls, _ = frontier.call_graph(funcs, image, base)
    rows = frontier.stubs(funcs, image, base, calls, {"dead", "caller"})
    assert [r["name"] for r in rows] == ["dead", "far"]     # live is called, tiny is not empty
    dead, far = rows
    assert dead["locked"] and dead["data_refs"] == 0
    assert dead["prev"] == {"name": "caller", "address": f"0x{base:08X}", "size": 16,
                            "locked": True, "stub": False, "adjacent": True}
    assert dead["next"]["name"] == "live" and not dead["next"]["locked"]
    assert not dead["next"]["stub"]                          # called, so not a caller-less stub
    assert not far["locked"] and far["data_refs"] == 1      # the pointer word in the gap
    assert far["prev"]["name"] == "tiny" and not far["prev"]["adjacent"]
    assert far["next"] is None


# --- queue hygiene -----------------------------------------------------------
def test_queue_exclude_and_name_files(tmp_path):
    calls = {"a": set(), "b": set(), "c": set()}
    facts = frontier.analyze(calls, {"a": 30, "b": 20, "c": 10}, set())
    assert [n for n, _ in frontier.queue(facts)] == ["a", "b", "c"]
    listing = tmp_path / "skip.txt"
    listing.write_text("# taken by agent 1\n\nb  0x80000000 20 L1\n c # note\n")
    assert frontier.read_names(listing) == {"b", "c"}
    assert [n for n, _ in frontier.queue(facts, exclude=frontier.read_names(listing))] == ["a"]


def test_in_flight_is_cloud_matches_minus_the_lock(tmp_path):
    for name in ("done", "pending"):
        (tmp_path / f"{name}.c").write_text("")
    (tmp_path / "notes.md").write_text("")
    assert frontier.in_flight({"done"}, tmp_path) == {"pending"}


def _assign_fixture():
    calls = {"callee": set(), "s1": {"callee"}, "s2": {"callee"}}
    calls.update({f"f{i}": set() for i in range(8)})
    calls.update({"keeper": {"f0"}})
    size = {n: 40 for n in calls}
    size.update({f"f{i}": 100 - i for i in range(8)})
    ipa = {"callees": {"callee": ["t0"]}, "callers": {"s1": ["callee"], "s2": ["callee"]},
           "preservers": {"f7": ["a1"]}}
    return frontier.analyze(calls, size, set(), ipa)


def test_bundles_keep_a_unit_with_its_partners():
    facts = _assign_fixture()
    by_first = {b["names"][0]: b for b in frontier.bundles(facts)}
    unit = next(b for b in by_first.values() if "callee" in b["names"] + b["with"])
    assert set(unit["names"] + unit["with"]) == {"callee", "s1", "s2"}
    assert unit["recipe"] == "unit" and unit["functions"] == 3 and unit["bytes"] == 120
    assert by_first["f0"]["recipe"] == "single" and by_first["f0"]["with"] == []
    assert by_first["f7"]["recipe"] == "group"
    assert not any("keeper" in b["names"] for b in by_first.values())   # not ready
    dropped = frontier.bundles(facts, exclude={"s2"})
    assert not any({"callee", "s1", "s2"} & set(b["names"] + b["with"]) for b in dropped)


def test_assign_gives_disjoint_bounded_batches_and_never_splits_a_unit():
    facts = _assign_fixture()
    batches, skipped = frontier.assign(facts, agents=3, per=4)
    assert skipped == []
    seen = []
    for batch in batches:
        assert 3 <= sum(b["functions"] for b in batch) <= 4
        for b in batch:
            seen += b["names"] + b["with"]
    assert len(seen) == len(set(seen)) == 11
    homes = [i for i, batch in enumerate(batches)
             for b in batch if {"callee", "s1", "s2"} & set(b["names"] + b["with"])]
    assert len(homes) == 1
    assert "f0" in seen                                      # best single made the cut
    assert "f7" in seen                                      # the group came along


def test_assign_skips_a_bundle_larger_than_a_batch_and_honours_exclude():
    facts = _assign_fixture()
    batches, skipped = frontier.assign(facts, agents=2, per=2, exclude={"f0"})
    names = [n for batch in batches for b in batch for n in b["names"] + b["with"]]
    assert "f0" not in names and "callee" not in names
    assert len(names) == 4
    assert [set(b["names"] + b["with"]) for b in skipped] == [{"callee", "s1", "s2"}]


# --- calibration and end to end ----------------------------------------------
def test_calibrate_counts_each_population_separately():
    ringed = tuple(_ring_words([T6, T7, T8, T9, T6]))
    clean = (JR_RA, NOP)
    words_of = {"alone": clean, "alone_bad": ringed, "member": ringed, "open": ringed,
                "open_clean": clean}
    lock = {"alone": {}, "alone_bad": {}, "member": {"group": "g"}}
    rows, totals = frontier.calibrate(words_of, lock)
    assert totals == (2, 1, 2)
    by_label = {r[0]: r for r in rows}
    assert by_label["temp ring, strict"][1:] == (True, 1, 1, 1, ["alone_bad"])
    assert by_label["unsaved callee-saved write"][2:] == (0, 0, 0, [])
    assert by_label["inlined-callee loads"][1] is False


def test_real_detectors_flag_no_standalone_lock():
    """The gate for a recipe-changing detector, against the live lock."""
    if not (frontier.LAYOUT_JSON.exists() and frontier.LOCKFILE.exists()):
        return
    try:
        rows, totals = frontier._calibration()
    except OSError:
        return
    assert totals[0] > 0
    for label, changes_recipe, standalone, _, _, names in rows:
        if changes_recipe:
            assert standalone == 0, (label, names)


def test_build_reports_provisional_stubs_and_signatures(tmp_path):
    import json
    base = 0x80086A50
    funcs = [(base, 24, "caller"), (base + 24, 20, "inner"), (base + 44, 8, "dead")]
    bodies = {"caller": [_jal(base + 24), NOP, JR_RA, NOP, NOP, NOP],
              "inner": _ring_words([T6, T7, T8, T9, T6]), "dead": [JR_RA, NOP]}
    (tmp_path / "image.bin").write_bytes(_image(base, funcs, bodies))
    layout = {"image": {"path": str(tmp_path / "image.bin"), "base": f"{base:X}"},
              "regions": [{"entries": [{"kind": "function", "vaddr": v, "size": s,
                                        "target_id": n} for v, s, n in funcs]}]}
    (tmp_path / "layout.json").write_text(json.dumps(layout))
    (tmp_path / "lock.json").write_text(json.dumps({"dead": {}}))
    (tmp_path / "prov.json").write_text(json.dumps({"inner": {"evidence": "e", "note": "n"}}))
    doc = frontier.build(tmp_path / "layout.json", tmp_path / "lock.json",
                         tmp_path / "no_ipa.json", tmp_path / "prov.json")
    assert doc["matched"] == 1 and doc["unmatched"] == 2    # provisional is not matched
    assert doc["provisional"] == ["inner"] and doc["provisional_bytes"] == 20
    assert doc["facts"]["inner"]["recipe"] == "group"
    assert doc["facts"]["inner"]["temp_ring"]["arms"] == ["strict"]
    assert doc["facts"]["inner"]["provisional_evidence"] == "e"
    assert doc["facts"]["caller"]["ready"]
    assert [s["name"] for s in doc["stubs"]] == ["dead"]
    text = frontier._summary(doc)
    assert text.splitlines()[0].startswith("2 unmatched game functions")
    assert "provisional (stand-in proof" in text and "inner" in text
    assert "caller-less jr-ra stubs: 1 (1 locked)" in text
    json.dumps(doc)                                          # scan output stays serialisable


def test_ipa_scan_adds_internal_classes_without_touching_members():
    from tools.conveyor.pipeline import ipa

    class Conn:
        def execute(self, _sql):
            return self

        def fetchall(self):
            return [{"target_id": n, "address": 0x80086A50 + 64 * i, "insn_count": 8,
                     "gate_reason": None}
                    for i, n in enumerate(("ringed", "saver", "plain"))]

    asm = "glabel f\n    jr $ra\n    nop\n"
    words = {"ringed": _ring_words([T6, T7, T8, T9, T6]),
             "saver": [(4 << 21) | (16 << 11) | 0x25],      # or s0,a0,zero with no save
             "plain": [JR_RA, NOP]}
    before = ipa.scan(Conn(), lambda _t: asm)
    after = ipa.scan(Conn(), lambda _t: asm, lambda row: words[row["target_id"]])
    assert set(before) == {"callees", "preservers", "callers", "members", "scanned"}
    assert {k: after[k] for k in before} == before
    assert after["unsaved"] == {"saver": ["s0"]}
    assert list(after["ring"]) == ["ringed"]
    assert after["internal"] == ["ringed", "saver"]


def test_load_internal_falls_back_to_members(tmp_path):
    from tools.conveyor.pipeline import ipa
    old = tmp_path / "old.json"
    old.write_text('{"members": ["a"]}')
    new = tmp_path / "new.json"
    new.write_text('{"members": ["a"], "internal": ["a", "b"]}')
    assert ipa.load_internal(old) == {"a"}
    assert ipa.load_internal(new) == {"a", "b"}
    assert ipa.load_internal(tmp_path / "absent.json") == set()
