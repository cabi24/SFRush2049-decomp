from pathlib import Path
import hashlib,json,struct,subprocess,zlib,sys
sys.path.insert(0,str(Path(__file__).resolve().parents[3]))
from tools.conveyor.pipeline import blob_group as bg
root=Path.cwd();p=Path('/tmp/rush-C90-independent');obj=p/'group.o';base=0x80086A50;address=0x800A1E94;table=0x80123B68;size=132
rom=(root/'baserom.us.z64').read_bytes();image=zlib.decompress(rom[0xB0CB10:],-15);assert image==(root/'build/game_code.bin').read_bytes()
slices,ndx=bg.member_slices(obj,['func_800A1E94'],{'func_800A1E94':{'vaddr':address,'size':size}})
# Actual production helper validates each local section entry and every paired
# text reference against the original image before relocating the full body.
bodies=bg.relocate(obj,slices,ndx,{},image=(image,base));body=bodies['func_800A1E94'];assert body==image[address-base:address-base+size]
raw=bg._section_bytes(obj,'.rodata');rels,others=bg._text_relocations(obj);table_rels=others['.rel.rodata'];syms=bg._symbols(obj)
assert len(raw)==48 and len(table_rels)==12
linked=bytearray(raw)
for index,(off,kind,name) in enumerate(table_rels):
 assert off==index*4 and kind=='R_MIPS_32' and name=='.text'
 addend=struct.unpack('>I',raw[off:off+4])[0];assert 0<=addend<size and addend%4==0
 linked[off:off+4]=struct.pack('>I',address+addend)
assert linked==image[table-base:table-base+48]
new=bytearray(image);new[address-base:address-base+size]=body;new[table-base:table-base+48]=linked;assert new==image
s=(p/'func_800A1E94.o3.c').read_bytes()
result={'function':'func_800A1E94','source':'cloud/work/game_C90/func_800A1E94.o3.c','source_sha256':hashlib.sha256(s).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'flags':'-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul','compiler_path':'Fresh private score.compile_group on Rocky; supported blob_group relocation and full original ROM/image checks locally','compile_exit':0,'production_relocate_exact':True,'native_text_bytes':size,'native_original_words':33,'differing':0,'table_vaddr':hex(table),'table_bytes':48,'table_R_MIPS_32_relocations':12,'table_sha256':hashlib.sha256(linked).hexdigest(),'body_sha256':hashlib.sha256(body).hexdigest(),'original_ROM_sha1':hashlib.sha1(rom).hexdigest(),'original_image_sha256':hashlib.sha256(image).hexdigest(),'composed_private_image_sha256':hashlib.sha256(new).hexdigest(),'original_image_matches_original_ROM_deflate':True,'unresolved':[],'unverified':[],'errors':[],'accepted_coverage':False,'note':'Strict singleton target0 still has unverified local-table refs; this stronger production relocation + complete source-built table proof resolves every ref privately. Root must independently replay supported group gates/ROM lock workflow.'}
(root/'cloud/work/coordinator_readonly_20261002/C90_independent.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
