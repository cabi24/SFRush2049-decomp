"""F1: deterministic cloud symbols use the same resolver as image splicing."""
import json
import hashlib

from tools.conveyor.pipeline import blob_layout, blob_splice, blob_tu


def _document():
    return {
        "image": {"base": f"{blob_layout.BASE:08X}", "sha256": "a" * 64},
        "regions": [{
            "name": "region",
            "vaddr_start": blob_layout.BASE,
            "vaddr_end": blob_layout.BASE + 8,
            "entries": [{"kind": "function", "target_id": "member",
                         "vaddr": blob_layout.BASE, "size": 8}],
        }],
    }


def test_export_preserves_resolver_addresses_aliases_and_image_hash(tmp_path, monkeypatch):
    document = _document()
    monkeypatch.setattr(blob_tu, "data_symbols", lambda: {
        blob_layout.BASE: "data_alias", 0x80000400: "static_fn",
        0x80116DE4: "D_80116DE4",
    })
    expected = blob_splice.image_symbols(document)
    path = blob_tu.write_symbols(document, tmp_path / "asm" / "symbols.json")
    payload = json.loads(path.read_text())

    assert payload["image_sha256"] == document["image"]["sha256"]
    assert payload["generated_by"] == "python3 -m tools.conveyor.pipeline.blob_tu symbols"
    assert {name: int(addr, 16) for name, addr in payload["symbols"].items()} == expected
    assert payload["symbols"]["member"] == payload["symbols"]["data_alias"]


def test_export_delegates_to_splice_resolver_and_is_byte_stable(tmp_path, monkeypatch):
    document = _document()
    addresses = {"z": 0x80100000, "a": 0x80000400}
    calls = []

    def resolve(received):
        assert received is document
        calls.append(received)
        return addresses

    monkeypatch.setattr(blob_splice, "image_symbols", resolve)
    path = tmp_path / "symbols.json"
    blob_tu.write_symbols(document, path)
    first = path.read_bytes()
    addresses = dict(reversed(list(addresses.items())))
    blob_tu.write_symbols(document, path)

    assert len(calls) == 2
    assert path.read_bytes() == first
    assert list(json.loads(first)["symbols"]) == ["a", "z"]
    assert first.endswith(b"\n")


def test_generate_refreshes_symbols_in_the_selected_asm_directory(tmp_path, monkeypatch):
    document = _document()
    image = tmp_path / "image.bin"
    image.write_bytes(bytes(8))
    asm_dir = tmp_path / "asm"
    monkeypatch.setattr(blob_tu, "data_symbols", lambda: {})
    blob_tu.generate(document, image, asm_dir, tmp_path / "blob", spliced={})
    path = asm_dir / "symbols.json"
    assert json.loads(path.read_text())["symbols"] == {"member": f"0x{blob_layout.BASE:08X}"}
    first_manifest = (asm_dir / "SHA256SUMS").read_text()
    assert hashlib.sha256(path.read_bytes()).hexdigest() in first_manifest
    assert hashlib.sha256((asm_dir / "region.s").read_bytes()).hexdigest() in first_manifest

    document["regions"][0]["entries"][0]["target_id"] = "renamed"
    blob_tu.generate(document, image, asm_dir, tmp_path / "blob", spliced={})
    assert json.loads(path.read_text())["symbols"] == {"renamed": f"0x{blob_layout.BASE:08X}"}
    assert (asm_dir / "SHA256SUMS").read_text() != first_manifest


def test_symbols_cli_needs_no_image_or_spliced_objects(tmp_path, monkeypatch, capsys):
    document = _document()
    layout = tmp_path / "layout.json"
    layout.write_text(json.dumps(document))
    output = tmp_path / "symbols.json"
    monkeypatch.setattr(blob_tu, "data_symbols", lambda: {})
    monkeypatch.setattr("sys.argv", ["blob_tu", "--layout", str(layout),
                                    "--image", str(tmp_path / "missing.bin"),
                                    "symbols", "--output", str(output)])

    assert blob_tu.main() == 0
    assert json.loads(output.read_text())["symbols"]["member"] == f"0x{blob_layout.BASE:08X}"
    assert str(output) in capsys.readouterr().out
    assert (tmp_path / "SHA256SUMS").is_file()
