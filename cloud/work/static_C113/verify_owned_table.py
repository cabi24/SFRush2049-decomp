"""Private existing-production-helper C113 table normalization/container replay."""
import sys,json,hashlib,shutil,tempfile,subprocess,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT))
from tools.conveyor.pipeline import owned_data as O,targets
P=Path(__file__).parent;W=ROOT/'build/C113';row=json.loads((P/'schedule.slot.proposed.json').read_text())
original=(ROOT/row['source']).read_bytes();offset=int(row['offset'],0);size=int(row['size'],0)
sha=lambda b:hashlib.sha256(b).hexdigest()
assert sha(original[offset:offset+size])==row['sha256']
with tempfile.TemporaryDirectory(prefix='rush-C113-owned-table-') as scratch:
 repo=Path(scratch);(repo/'assets/us').mkdir(parents=True);(repo/'tools/asm-processor').mkdir(parents=True)
 shutil.copy2(ROOT/'tools/asm-processor/asm_processor.py',repo/'tools/asm-processor/asm_processor.py')
 (repo/row['source']).write_bytes(original)
 (repo/O.REGISTRY).write_text(json.dumps({'schema':1,'rom_slots':[row],'storage_blocks':[]}))
 obj=repo/'build/us/src/rom/lib_1050_sdk.o';obj.parent.mkdir(parents=True);shutil.copy2(W/'native_test.o',obj)
 E=O._elf_processor(repo);old=E.ElfFile(obj.read_bytes());relocations={s.name:s.data for s in old.sections if s.is_rel()}
 changes=O.normalize_object_sections(repo,row['tu'],obj)
 assert changes==[{'owner':'__scScheduleCore','section':'.rodata','size':28,'alignment':4}]
 new=E.ElfFile(obj.read_bytes());ro=new.find_section('.rodata')
 assert ro.data==old.find_section('.rodata').data[:28] and ro.sh_addralign==4
 assert {s.name:s.data for s in new.sections if s.is_rel()}==relocations
 assert new.find_section('.text').data==old.find_section('.text').data
 changed=[]
 for a,b in zip(old.symtab.symbol_entries,new.symtab.symbol_entries):
  if a.to_bin()!=b.to_bin():
   assert a.type==b.type==E.STT_SECTION and a.st_shndx==b.st_shndx==ro.index and (a.st_size,b.st_size)==(32,28)
   changed.append({'symbol':a.name,'old_size':32,'new_size':28})
 normalized=obj.read_bytes();assert O.normalize_object_sections(repo,row['tu'],obj)==[] and obj.read_bytes()==normalized
 container=repo/'build/us/assets/us/data.o';O.split_container(repo,row['source'],repo/row['source'],container)
 script='''OUTPUT_ARCH(mips)
__scTaskReady = 0x80000BA4;
SECTIONS {
 .text 0x80000FE8 : SUBALIGN(4) {
  build/us/src/rom/lib_1050_sdk.o(.text);
 }
 .rodata : {
  build/us/src/rom/lib_1050_sdk.o(.rodata);
 }
 .data 0x8000F400 : SUBALIGN(16) {
  build/us/assets/us/data.o(.data);
 }
 /DISCARD/ : { *(.bss) *(.reginfo) *(.options) *(.mdebug) *(.MIPS.abiflags) }
}
'''
 rewritten=O.rewrite_linker(script,repo);assert O.rewrite_linker(rewritten,repo)==rewritten
 assert 'rom_owned___scScheduleCore_START == 0x8002d4a4' in rewritten
 assert 'rom_owned___scScheduleCore_END - rom_owned___scScheduleCore_START == 0x1c' in rewritten
 (repo/'model.ld').write_text(rewritten);(P/'generated_linker.proposed.ld').write_text(rewritten)
 command=['mips-linux-gnu-ld','--accept-unknown-input-arch','-T','model.ld','-o','linked.elf',str(obj.relative_to(repo)),str(container.relative_to(repo))]
 subprocess.run(command,cwd=repo,check=True,capture_output=True,text=True)
 def section(name):
  dst=repo/(name[1:]+'.bin');subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j',name,str(repo/'linked.elf'),str(dst)],check=True,capture_output=True);return dst.read_bytes()
 data=section('.data');assert data==original
 text=section('.text');region=targets.index_asm_regions()[0x80000fe8];native=b''.join(struct.pack('>I',int(x,16)) for x in region.words)
 assert len(native)==872 and text[:872]==native and not any(text[872:])
 neighbor=original[offset+size:offset+size+4];assert sum(bool(b) for b in neighbor)==4 and data[offset+size:offset+size+4]==neighbor
 shutil.copy2(W/'native_test.o',obj);refusal=subprocess.run(command,cwd=repo,capture_output=True,text=True);assert refusal.returncode!=0 and 'owned ROM slot size changed' in refusal.stderr
 obj.write_bytes(normalized)
 changed_container=bytearray(original);changed_container[offset+size]^=1;(repo/'changed.bin').write_bytes(changed_container)
 O.split_container(repo,row['source'],repo/'changed.bin',container);subprocess.run(command,cwd=repo,check=True,capture_output=True,text=True);assert section('.data')==bytes(changed_container) and sha(section('.data'))!=sha(original)
 result={'production_helper_sha256':sha((ROOT/'tools/conveyor/pipeline/owned_data.py').read_bytes()),'normalization':changes,'all_retained_relocations_exact':True,'all_text_bytes_unchanged_by_normalization':True,'only_descriptive_symbol_size_changed':changed,'native_text_bytes_exact':872,'table_bytes_exact':28,'table_start':'0x8002D4A4','full_original_data_container_bytes_exact':len(original),'full_original_data_container_sha256':sha(original),'nonzero_neighbor_bytes_preserved':4,'untrimmed_object_link_refuses':True,'changed_neighbor_detectable':True,'idempotent_normalization_and_link_rewrite':True,'accepted_credit':0}
 (P/'owned_table_proof.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
