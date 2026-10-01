import importlib.util
import json
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("external_docs", REPO / "tools" / "external_docs.py")
external_docs = importlib.util.module_from_spec(spec)
spec.loader.exec_module(external_docs)


def _doc(tmp_path, text):
    source = tmp_path / "source.md"
    source.write_text(text)
    return {"name": "demo", "file": "demo.md", "url": source.as_uri()}, source


def test_first_fetch_writes_the_copy_and_metadata(tmp_path):
    doc, _ = _doc(tmp_path, "# Notes\nline one\n")
    status, _ = external_docs.fetch_one(doc, files_dir=tmp_path / "files")
    assert status == "new"
    assert (tmp_path / "files" / "demo.md").read_text() == "# Notes\nline one\n"
    meta = json.loads((tmp_path / "files" / "demo.md.meta.json").read_text())
    assert meta["url"] == doc["url"] and len(meta["sha256"]) == 64


def test_unchanged_document_is_not_rewritten(tmp_path):
    doc, _ = _doc(tmp_path, "same\n")
    files = tmp_path / "files"
    external_docs.fetch_one(doc, files_dir=files)
    before = (files / "demo.md").stat().st_mtime_ns
    status, _ = external_docs.fetch_one(doc, files_dir=files)
    assert status == "unchanged"
    assert (files / "demo.md").stat().st_mtime_ns == before
    assert not (files / "demo.md.prev").exists()


def test_changed_document_keeps_the_previous_copy_and_reports_the_delta(tmp_path):
    doc, source = _doc(tmp_path, "a\nb\nc\n")
    files = tmp_path / "files"
    external_docs.fetch_one(doc, files_dir=files)
    source.write_text("a\nb\nd\ne\n")
    status, message = external_docs.fetch_one(doc, files_dir=files)
    assert status == "updated" and "+2/-1" in message
    assert (files / "demo.md.prev").read_text() == "a\nb\nc\n"
    assert (files / "demo.md").read_text() == "a\nb\nd\ne\n"


def test_a_failed_download_keeps_the_good_copy(tmp_path):
    doc, source = _doc(tmp_path, "keep me\n")
    files = tmp_path / "files"
    external_docs.fetch_one(doc, files_dir=files)
    source.unlink()
    status, message = external_docs.fetch_one(doc, files_dir=files)
    assert status == "failed" and "keeping" in message
    assert (files / "demo.md").read_text() == "keep me\n"


def test_an_empty_response_is_refused(tmp_path):
    doc, source = _doc(tmp_path, "good\n")
    files = tmp_path / "files"
    external_docs.fetch_one(doc, files_dir=files)
    source.write_text("  \n")
    status, _ = external_docs.fetch_one(doc, files_dir=files)
    assert status == "failed"
    assert (files / "demo.md").read_text() == "good\n"


def test_the_shipped_manifest_is_well_formed():
    for doc in external_docs.load_sources():
        assert {"name", "file", "url", "licence"} <= set(doc)
        assert doc["url"].startswith("https://")
