import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess

import pytest
from tools.conveyor.pipeline import owned_data as O, layout as L, promote as P


@pytest.fixture
def owner(tmp_path):
    (tmp_path/'assets/us').mkdir(parents=True)
    data=bytes(range(96));(tmp_path/'assets/us/data.bin').write_bytes(data)
    row={'owner':'f','tu':'src/rom/lib_test.c','source':'assets/us/data.bin','offset':16,'size':32,'container_vram':0x80000000,'sha256':hashlib.sha256(data[16:48]).hexdigest(),'passthrough_asm':'asm/us/f.table.s'}
    reg={'schema':1,'rom_slots':[row],'storage_blocks':[]}
    (tmp_path/O.REGISTRY).write_text(json.dumps(reg))
    return tmp_path,row,reg


def save(repo,reg):
    (repo/O.REGISTRY).write_text(json.dumps(reg))


@pytest.mark.parametrize('changes',[
    {'offset':80}, {'size':0}, {'offset':17}, {'size':17},
    {'sha256':'0'*64}, {'tu':'../x.c'}, {'owner_section':'.text'},
    {'container_vram':0xFFFFFFFF},
])
def test_invalid_owner_refuses(owner,changes):
    repo,row,reg=owner;row.update(changes);save(repo,reg)
    with pytest.raises(O.OwnershipError):O.rom_slots(repo)


def test_overlapping_and_duplicate_input_sections_refuse(owner):
    repo,row,reg=owner;other=copy.deepcopy(row);other.update(owner='other',tu='src/rom/lib_other.c',offset=32,sha256=hashlib.sha256(bytes(range(96))[32:64]).hexdigest());reg['rom_slots'].append(other);save(repo,reg)
    with pytest.raises(O.OwnershipError,match='overlap'):O.rom_slots(repo)
    other['offset']=48;other['tu']=row['tu'];save(repo,reg)
    with pytest.raises(O.OwnershipError,match='input section'):O.rom_slots(repo)


def test_preserves_separate_storage_registry(owner):
    repo,row,reg=owner;reg['storage_blocks']=[{'owner':'other','tu':'src/rom/other.c'}];save(repo,reg)
    assert O.load_registry(repo)['storage_blocks']==reg['storage_blocks']


def test_split_excludes_actual_owned_bytes(owner,monkeypatch):
    repo,row,reg=owner;composed=repo/'composed.bin';data=(repo/row['source']).read_bytes();composed.write_bytes(data)
    calls=[];monkeypatch.setattr(O.subprocess,'run',lambda argv,**kw:calls.append(argv))
    chunks=O.split_container(repo,row['source'],composed,repo/'build/data.o')
    assert [Path(p).read_bytes() for p in chunks]==[data[:16],data[48:]]
    assert '.data=.data.owned0' in calls[0]
    assert '.data.owned1='+chunks[1] in calls[1]
    composed.write_bytes(data[:16]+b'X'*32+data[48:])
    with pytest.raises(O.OwnershipError,match='altered'):O.split_container(repo,row['source'],composed,repo/'bad.o')


def test_linker_regeneration_stable_and_exact_slot(owner):
    repo,row,reg=owner;stock='SECTIONS {\n .x {\n  build/us/src/rom/lib_test.o(.rodata);\n  build/us/assets/us/data.o(.data);\n }\n}\n'
    got=O.rewrite_linker(stock,repo)
    assert got==O.rewrite_linker(got,repo)
    assert got==O.rewrite_linker(stock,repo)
    assert 'build/us/assets/us/data.o(.data);' not in got
    assert got.count('build/us/src/rom/lib_test.o(.rodata);')==1
    assert '== 0x80000010' in got and '== 0x20' in got
    with pytest.raises(O.OwnershipError):O.rewrite_linker(stock+stock,repo)


def test_generate_companion_only_for_actual_passthrough(owner,monkeypatch):
    repo,row,reg=owner;monkeypatch.setattr(L,'REPO',repo)
    seg={'rom_tu':'rom/lib_test','yaml_name':'0x1000','functions':[{'name':'f','state':'passthrough'}]}
    baseline=L.generate_tu(seg,'hash');assert O.companion_block(O.rom_slots(repo)[0]) in baseline
    assert baseline==L.generate_tu(seg,'hash')
    seg['functions'][0].update(state='promoted',body='void f(void) {}')
    assert 'f.table.s' not in L.generate_tu(seg,'hash')


def test_splice_failure_leaves_original_companion_and_success_removes_both(owner,monkeypatch):
    repo,row,reg=owner;monkeypatch.setattr(P,'REPO',repo)
    tu=repo/row['tu'];tu.parent.mkdir(parents=True);slot='#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_test/f.s")'
    original=O.companion_block(O.rom_slots(repo)[0])+slot+'\n'
    tu.write_text(original.replace('ROM_OWNED_RODATA_END f','BROKEN_END'))
    before=tu.read_text()
    with pytest.raises(O.OwnershipError):P._splice(tu,'f',{'rom_tu':'rom/lib_test'},'void f(void) {}','header')
    assert tu.read_text()==before
    tu.write_text(original);P._splice(tu,'f',{'rom_tu':'rom/lib_test'},'void f(void) {}','header')
    assert 'GLOBAL_ASM' not in tu.read_text()
    # Existing transaction restores the complete TU snapshot, including real companion.
    tu.write_text(original);assert O.companion_block(O.rom_slots(repo)[0]) in tu.read_text()


def load_asm():
    path=Path(__file__).resolve().parents[2]/'tools/asm-processor/asm_processor.py'
    spec=importlib.util.spec_from_file_location('owned_asm',path);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module


@pytest.mark.parametrize('directive,section',[('.incbin "data.bin", 0, 4','.text'),('.incbin "data.bin", 0','.rodata'),('.incbin "data.bin", 0, -1','.rodata'),('.incbin "data.bin", 4, 8','.rodata'),('.incbin "missing.bin", 0, 4','.rodata')])
def test_incbin_rejects_text_unbounded_or_outside_file(tmp_path,monkeypatch,directive,section):
    monkeypatch.chdir(tmp_path);(tmp_path/'data.bin').write_bytes(b'12345678')
    asm=load_asm();block=asm.GlobalAsmBlock('fixture');block.cur_section=section
    with pytest.raises(asm.Failure):block.process_line(directive,'utf-8')


def test_incbin_count_from_real_bounded_file(tmp_path,monkeypatch):
    monkeypatch.chdir(tmp_path);(tmp_path/'data.bin').write_bytes(b'12345678')
    asm=load_asm();block=asm.GlobalAsmBlock('fixture');block.cur_section='.rodata';block.process_line('.incbin "data.bin", 2, 4','utf-8')
    assert block.fn_section_sizes['.rodata']==4


@pytest.mark.parametrize('path',['assets/us/x".bin','assets/us/x\n.bin','assets/us/x;.bin','/tmp/x','../x'])
def test_unsafe_paths_refused(owner,path):
    repo,row,reg=owner;row['passthrough_asm']=path;save(repo,reg)
    with pytest.raises(O.OwnershipError):O.rom_slots(repo)


def test_generated_asm_symlink_cannot_escape(owner,tmp_path):
    repo,row,reg=owner;outside=repo.parent/'outside';outside.write_text('x');(repo/'asm/us').mkdir(parents=True);(repo/row['passthrough_asm']).symlink_to(outside)
    with pytest.raises(O.OwnershipError,match='outside'):O.rom_slots(repo)


def test_missing_and_truncated_source_refuse(owner):
    repo,row,reg=owner;p=repo/row['source'];p.unlink()
    with pytest.raises(O.OwnershipError,match='missing'):O.rom_slots(repo)
    p.write_bytes(b'short')
    with pytest.raises(O.OwnershipError,match='bounds'):O.rom_slots(repo)


def test_missing_metadata_field_refuses_cleanly(owner):
    repo,row,reg=owner;del row['size'];save(repo,reg)
    with pytest.raises(O.OwnershipError,match='metadata'):O.rom_slots(repo)


def test_duplicate_owner_refuses(owner):
    repo,row,reg=owner;reg['rom_slots'].append(copy.deepcopy(row));save(repo,reg)
    with pytest.raises(O.OwnershipError,match='duplicate'):O.rom_slots(repo)


def test_source_built_game_blob_cannot_be_owned(owner):
    from tools.conveyor.pipeline.blob_rom import ROM_OFFSET
    repo,row,reg=owner;row['offset']=ROM_OFFSET-0x10000;save(repo,reg)
    with pytest.raises(O.OwnershipError,match='source-built'):O.rom_slots(repo)


def test_empty_registry_restores_all_generated_linker_inputs(owner):
    repo,row,reg=owner;stock='SECTIONS {\n build/us/src/rom/lib_test.o(.rodata);\n build/us/assets/us/data.o(.data);\n}\n';rendered=O.rewrite_linker(stock,repo)
    reg['rom_slots']=[];save(repo,reg);restored=O.rewrite_linker(rendered,repo)
    assert restored==stock
    assert O.rewrite_linker(stock,repo)==stock


def test_no_owner_split_keeps_original_binary_recipe(owner,monkeypatch):
    repo,row,reg=owner;reg['rom_slots']=[];save(repo,reg);calls=[];monkeypatch.setattr(O.subprocess,'run',lambda a,**k:calls.append(a))
    assert O.split_container(repo,row['source'],repo/row['source'],repo/'none.o')==[]
    assert calls==[['mips-linux-gnu-objcopy','-I','binary','-O','elf32-big',str(repo/row['source']),str(repo/'none.o')]]


@pytest.mark.parametrize('offset,size',[(0,32),(64,32),(0,96)])
def test_zero_length_edge_slices_make_real_valid_elf(owner,offset,size):
    repo,row,reg=owner;data=(repo/row['source']).read_bytes();row.update(offset=offset,size=size,sha256=hashlib.sha256(data[offset:offset+size]).hexdigest());save(repo,reg)
    out=repo/'edge.o';O.split_container(repo,row['source'],repo/row['source'],out)
    table=subprocess.check_output(['mips-linux-gnu-readelf','-S',str(out)],text=True)
    assert '.data ' not in table
    assert out.is_file()


def test_adjacent_slots_do_not_feed_empty_chunks_to_objcopy(owner):
    repo,row,reg=owner;data=(repo/row['source']).read_bytes();other=copy.deepcopy(row);other.update(owner='other',tu='src/rom/other.c',offset=48,sha256=hashlib.sha256(data[48:80]).hexdigest());reg['rom_slots'].append(other);save(repo,reg)
    out=repo/'adjacent.o';O.split_container(repo,row['source'],repo/row['source'],out)
    table=subprocess.check_output(['mips-linux-gnu-readelf','-S',str(out)],text=True)
    assert '.data.owned0' in table and '.data.owned2' in table and '.data.owned1' not in table


def test_owned_gate_failure_restores_companion_and_function(tmp_path, monkeypatch):
    spec = importlib.util.spec_from_file_location("promotion_fixture", Path(__file__).parent / "unit/test_promote.py")
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    r, mp, lock, data = module.repo.__wrapped__(tmp_path, monkeypatch)
    (r / "assets/us").mkdir(parents=True)
    raw = bytes(range(96)); (r / "assets/us/data.bin").write_bytes(raw)
    row = {"owner":"strlen", "tu":"src/rom/lib_8800.c", "source":"assets/us/data.bin", "offset":16, "size":32, "container_vram":0x80000000, "sha256":hashlib.sha256(raw[16:48]).hexdigest(), "passthrough_asm":"asm/us/strlen.table.s"}
    save(r, {"schema":1,"rom_slots":[row],"storage_blocks":[]})
    tu = r / row["tu"]; baseline = O.companion_block(row) + tu.read_text(); tu.write_text(baseline)
    module._git(r, "add", "-A"); module._git(r, "commit", "-q", "-m", "ownership baseline")
    module._mock_gate(mp, False, "deliberately bad table")
    with pytest.raises(P.Refusal, match="GATE FAILED"):
        P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)
    assert tu.read_text() == baseline
    assert "src/libc/string.c:strlen" in json.loads(lock.read_text())
    assert module._outcomes(data)[-1]["outcome"] == "failed"


def test_reserved_game_slot_matches_build_authority():
    from tools.conveyor.pipeline import blob_rom
    assert O.SOURCE_BUILT_GAME_SLOT == blob_rom.ROM_OFFSET - 0x10000
    assert O.SOURCE_BUILT_GAME_LENGTH == blob_rom.LENGTH
    makefile = (Path(__file__).resolve().parents[2] / 'Makefile').read_text()
    import re
    assert int(re.search(r'^GAME_BLOB_SLOT\s*:=\s*(\S+)', makefile, re.M).group(1), 0) == O.SOURCE_BUILT_GAME_SLOT
    assert int(re.search(r'^GAME_BLOB_LEN\s*:=\s*(\S+)', makefile, re.M).group(1), 0) == O.SOURCE_BUILT_GAME_LENGTH


def test_dependency_listing_works_without_ignored_assets(owner, capsys):
    repo, row, reg = owner
    (repo / row['source']).unlink()
    O.main(['--repo', str(repo), 'companions'])
    assert capsys.readouterr().out.strip() == row['passthrough_asm']
    with pytest.raises(O.OwnershipError, match='missing or unreadable'):
        O.rom_slots(repo)
