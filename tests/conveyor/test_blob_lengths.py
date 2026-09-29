"""F3: no splice may silently truncate instructions or consume its neighbor."""
import json
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import blob_build, blob_group, blob_splice

pytestmark = pytest.mark.skipif(
    not all(shutil.which(tool) for tool in
            (blob_build.AS, blob_build.LD, blob_build.OBJCOPY, blob_build.READELF)),
    reason="needs MIPS binutils")


def _object(tmp_path, path, extra="", neighbor=False, before=False):
    section = ".text.f" if path == "single" else ".text"
    assembly = f'.set noreorder\n.section {section}, "ax", @progbits\n'
    if before:
        assembly += '.globl before\n.type before,@function\nbefore:\njr $ra\nnop\n'
    assembly += '.globl f\n.type f,@function\nf:\njr $ra\nnop\n.size f,8\n'
    # An ordinary label must not conceal excess instructions after f.
    assembly += '.globl local_label\nlocal_label:\n' + extra
    if neighbor:
        assembly += '.globl neighbor\n.type neighbor,@function\nneighbor:\njr $ra\nnop\n'
    source = tmp_path / "object.s"
    source.write_text(assembly)
    obj = tmp_path / "object.o"
    subprocess.run([blob_build.AS, "-EB", "-mips2", "-32", "-o", str(obj), str(source)],
                   check=True, capture_output=True)
    return obj


def _body(tmp_path, path, obj, size=8):
    if path == "single":
        return blob_splice.link_function(obj, "f", 0x80100000, size,
                                         provides={}, work=tmp_path / "link")
    slices, ndx = blob_group.member_slices(
        obj, ["f"], {"f": {"vaddr": 0x80100000, "size": size}})
    return blob_group.relocate(obj, slices, ndx, {})["f"]


@pytest.mark.parametrize("path", ["single", "group"])
def test_zero_padding_passes(tmp_path, path):
    obj = _object(tmp_path, path, extra=".word 0,0\n")
    assert _body(tmp_path, path, obj) == struct.pack(">II", 0x03E00008, 0)


@pytest.mark.parametrize("path,error", [("single", blob_build.BuildError),
                                       ("group", blob_group.GroupError)])
def test_extra_instruction_fails(tmp_path, path, error):
    obj = _object(tmp_path, path, extra=".word 0x24020001,0\n")
    with pytest.raises(error, match="f: 1 extra words"):
        _body(tmp_path, path, obj)


@pytest.mark.parametrize("path", ["single", "group"])
def test_next_function_is_not_counted_as_excess(tmp_path, path):
    obj = _object(tmp_path, path, extra=".word 0,0\n", neighbor=True)
    assert _body(tmp_path, path, obj) == struct.pack(">II", 0x03E00008, 0)


@pytest.mark.parametrize("path,error", [("single", blob_build.BuildError),
                                       ("group", blob_group.GroupError)])
def test_short_body_cannot_consume_next_function(tmp_path, path, error):
    obj = _object(tmp_path, path, neighbor=True)
    with pytest.raises(error, match="shorter than the 12-byte extent"):
        _body(tmp_path, path, obj, size=12)


def test_group_member_need_not_start_at_zero(tmp_path):
    obj = _object(tmp_path, "group", before=True, neighbor=True)
    assert _body(tmp_path, "group", obj) == struct.pack(">II", 0x03E00008, 0)


def test_single_extent_uses_section_offsets_not_linker_alignment(tmp_path):
    obj = _object(tmp_path, "single", extra=".word 0,0\n")
    body = blob_splice.link_function(obj, "f", 0x80100004, 8,
                                     provides={}, work=tmp_path / "link")
    assert body == struct.pack(">II", 0x03E00008, 0)


def test_context_length_does_not_block_spliced_member(tmp_path):
    obj = _object(tmp_path, "group", extra=".word 0x24020001,0\n", neighbor=True)
    # f is oversized context here; only neighbor is spliced.
    spec = {"members": ["neighbor"], "context": ["f"], "files": [],
            "keep": ["neighbor"], "flags": "-O3"}
    root = tmp_path / "groups"
    (root / "demo").mkdir(parents=True)
    (root / "demo" / "group.json").write_text(json.dumps(spec))
    obj.rename(tmp_path / "demo.o")
    document = {"regions": [{"entries": [
        {"kind": "function", "target_id": "f", "vaddr": 0x80100000, "size": 8},
        {"kind": "function", "target_id": "neighbor", "vaddr": 0x80200000, "size": 8},
    ]}]}
    bodies = blob_group.group_bodies("demo", document, {}, obj_dir=tmp_path, root=root)
    assert bodies == {"neighbor": struct.pack(">II", 0x03E00008, 0)}
    with pytest.raises(blob_group.GroupError, match="f: 1 extra words"):
        blob_group.group_bodies("demo", document, {}, obj_dir=tmp_path, root=root,
                                include_context=True)
