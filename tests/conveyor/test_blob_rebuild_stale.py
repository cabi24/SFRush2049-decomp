"""w24t: rebuild exactly the stale build/blob/obj objects (missing, or bound by
provenance to other source, as after a rename) instead of refusing; the
provenance check itself is unchanged (folded in from w23m/rebuild_stale.py)."""
import json

import pytest

from tools.conveyor.pipeline import blob_group, blob_rom, blob_splice


@pytest.fixture
def tree(tmp_path, monkeypatch):
    repo = tmp_path / "repo"
    (repo / "src/blob/groups/G").mkdir(parents=True)
    for name, text in (("a.c", "int a(void) { return 1; }\n"),
                       ("b.c", "int b(void) { return 2; }\n"),
                       ("c.c", "int c(void) { return 3; }\n")):
        (repo / "src/blob" / name).write_text(text)
    gdir = repo / "src/blob/groups/G"
    (gdir / "group.json").write_text(json.dumps(
        {"members": ["g1"], "files": ["g.c"], "keep": ["g1"], "flags": "-O3"}))
    (gdir / "g.c").write_text("int g1(void) { return 4; }\n")
    monkeypatch.setattr(blob_splice, "REPO", repo)
    monkeypatch.setattr(blob_group, "GROUP_DIR", repo / "src/blob/groups")
    monkeypatch.setattr(blob_group, "load",
                        lambda g, root=None, _l=blob_group.load: _l(g, repo / "src/blob/groups"))
    obj = tmp_path / "obj"
    (obj / "groups").mkdir(parents=True)

    def entry(src, flags="-O2"):
        return {"source": f"src/blob/{src}", "flagset": flags, "toolkit_sha": blob_splice.TOOLKIT,
                "source_sha256": blob_splice.source_sha((repo / "src/blob" / src).read_text())}

    gsha = blob_group.source_sha(blob_group.load("G"))
    lock = {"a": entry("a.c"), "b": entry("b.c"), "c": entry("c.c", "-O1"),
            "g1": {"source": "src/blob/groups/G/g.c", "group": "G", "flagset": "-O3",
                   "toolkit_sha": blob_splice.TOOLKIT, "source_sha256": gsha}}
    # a: current; b: built from pre-rename source; c: object missing; G: stale
    for t in ("a", "b"):
        (obj / f"{t}.o").write_bytes(b"")
    blob_splice.write_provenance(obj / "a.o", lock["a"]["source_sha256"], blob_splice.TOOLKIT)
    blob_splice.write_provenance(obj / "b.o", "0" * 64, blob_splice.TOOLKIT)
    (obj / "groups/G.o").write_bytes(b"")
    blob_splice.write_provenance(obj / "groups/G.o", "1" * 64, blob_splice.TOOLKIT)
    calls = []

    def fake_singles(sources, flagset, obj_dir, **kw):
        calls.append(("singles", flagset, sorted(sources), kw))
        for t, path in sources.items():
            (obj_dir / f"{t}.o").write_bytes(b"")
            blob_splice.write_provenance(obj_dir / f"{t}.o",
                                         blob_splice.source_sha(path.read_text()), blob_splice.TOOLKIT)
        return {t: obj_dir / f"{t}.o" for t in sources}, {}

    def fake_group(spec, obj_dir, **kw):
        calls.append(("group", spec["name"], kw))
        out = blob_group.object_path(spec["name"], obj_dir)
        out.write_bytes(b"")
        blob_splice.write_provenance(out, blob_group.source_sha(spec), blob_splice.TOOLKIT)
        return out

    return repo, obj, lock, calls, fake_singles, fake_group


def test_stale_objects_names_exactly_the_stale_ones(tree):
    repo, obj, lock, *_ = tree
    singles, groups, why = blob_splice.stale_objects(lock, obj)
    assert sorted(singles) == ["b", "c"] and set(groups) == {"G"}
    assert why["c"] == "object missing" and "different source_sha256" in why["b"]


def test_unbound_objects_only_with_include_unbound(tree):
    repo, obj, lock, *_ = tree
    (obj / "a.o.provenance.json").unlink()
    assert "a" not in blob_splice.stale_objects(lock, obj)[0]
    assert "a" in blob_splice.stale_objects(lock, obj, include_unbound=True)[0]


def test_rebuild_compiles_exactly_the_stale_ones_per_flagset(tree):
    repo, obj, lock, calls, fs, fg = tree
    r = blob_splice.rebuild_stale(lock, obj, workdir="~/scratch/x", log=lambda *_: None,
                                  compile_singles=fs, compile_group=fg)
    assert r == {"singles": ["b", "c"], "groups": ["G"], "still": []}
    assert [(c[0], c[1], c[2]) for c in calls if c[0] == "singles"] == [
        ("singles", "-O1", ["c"], ), ("singles", "-O2", ["b"])]
    assert calls[0][3] == {"workdir": "~/scratch/x"}
    assert [c[1] for c in calls if c[0] == "group"] == ["G"]
    assert blob_splice.stale_objects(lock, obj) == ({}, {}, {})


def test_dry_run_compiles_nothing(tree):
    repo, obj, lock, calls, fs, fg = tree
    r = blob_splice.rebuild_stale(lock, obj, dry_run=True, log=lambda *_: None,
                                  compile_singles=fs, compile_group=fg)
    assert r["singles"] == ["b", "c"] and not calls


def test_drifted_source_is_refused_before_any_compile(tree):
    repo, obj, lock, calls, fs, fg = tree
    (repo / "src/blob/b.c").write_text("int b(void) { return 99; }\n")
    with pytest.raises(blob_splice.LockedBodyError) as info:
        blob_splice.rebuild_stale(lock, obj, log=lambda *_: None,
                                  compile_singles=fs, compile_group=fg)
    assert set(info.value.problems) == {"b"} and not calls


def test_provenance_still_binds_after_rebuild(tree):
    """A compile that produced an object for other text leaves it stale (reported)."""
    repo, obj, lock, calls, fs, fg = tree

    def wrong(sources, flagset, obj_dir, **kw):
        for t in sources:
            (obj_dir / f"{t}.o").write_bytes(b"")
            blob_splice.write_provenance(obj_dir / f"{t}.o", "2" * 64, blob_splice.TOOLKIT)
        return {}, {}

    r = blob_splice.rebuild_stale(lock, obj, log=lambda *_: None,
                                  compile_singles=wrong, compile_group=fg)
    assert r["still"] == ["b", "c"]


def test_blob_rom_rom_rebuilds_before_composing(monkeypatch, tmp_path):
    order = []
    for key in ("a", "b"):
        p = tmp_path / f"ovl_{key}.deflate"
        p.write_bytes(b"x")
    monkeypatch.setattr(blob_rom, "OVL_STREAMS", {"a": tmp_path / "ovl_a.deflate",
                                                  "b": tmp_path / "ovl_b.deflate"})
    monkeypatch.setattr(blob_splice, "rebuild_stale", lambda **k: order.append("rebuild") or {})

    def produce():
        order.append("produce")
        raise blob_rom.BlobRomError("stop")

    monkeypatch.setattr(blob_rom, "produce", produce)
    with pytest.raises(blob_rom.BlobRomError):
        blob_rom.rom()
    assert order == ["rebuild", "produce"]
    order.clear()
    with pytest.raises(blob_rom.BlobRomError):
        blob_rom.rom(rebuild_stale=False)
    assert order == ["produce"]


def test_produce_refusal_names_the_remedy(monkeypatch):
    def stale(**kwargs):
        raise blob_splice.LockedBodyError(
            {"a": "object was built from a different source_sha256 than the lock records"})
    monkeypatch.setattr(blob_splice, "check", lambda *a, **k: [])
    monkeypatch.setattr(blob_splice, "spliced_bodies", stale)
    with pytest.raises(blob_rom.BlobRomError, match="rebuild-stale"):
        blob_rom.produce(document={"regions": []})
