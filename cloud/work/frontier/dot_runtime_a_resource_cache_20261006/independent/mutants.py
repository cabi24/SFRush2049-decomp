import hashlib,json,os,subprocess,sys
from pathlib import Path
if not __debug__:raise RuntimeError("Python optimization is unsupported")
HERE=Path(__file__).resolve().parent
OUT=HERE.parents[4]/'build/runtime_a_cache_independent'
OUT.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]).resolve();text=source.read_text();result={}
variants={
 'loaded_must_equal_one':('record->loaded != 0','record->loaded == 1'),
 'only_minus_one_is_free':('D_803BA230[i].id < 0','D_803BA230[i].id == -1'),
 'drop_last_record':('s32 count = 16;','s32 count = 15;'),
 'wrong_resource_bias':('kind + 88','kind + 87'),
 'wrong_loader_argument':('kind + 88, 1, 1, 0, 0','kind + 88, 0, 1, 0, 0'),
 'wrong_table_index':('D_80142B08[id] = record->handle;','D_80142B08[0] = record->handle;'),
 'reject_negative_handle':('*result = record->handle;\n    record->loaded = 1;','if (record->handle < 0) return 0;\n    *result = record->handle;\n    record->loaded = 1;'),
 'unsigned_record_id':('s8 id;','u8 id;'),
 'omit_second_helper':('func_800BB02C(id, kind, 0);',';'),
}
for name,(a,b) in variants.items():
 assert text.count(a)==1,(name,text.count(a))
 (OUT/'mutant.c').write_text(text.replace(a,b))
 p=subprocess.run([sys.executable,str(HERE/'review.py'),str(OUT/'mutant.c'),'--native-only'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 assert p.returncode!=0 and 'AssertionError:' in p.stdout,(name,p.stdout)
 assert 'subprocess.CalledProcessError' not in p.stdout,(name,p.stdout)
 result[name]={'rejected':True,'failure':p.stdout.split('AssertionError:',1)[1].strip()[:180]}
assert source.read_text()==text
(OUT/'mutants.json').write_text(json.dumps({'source_sha256':hashlib.sha256(text.encode()).hexdigest(),'mutants':result},indent=2)+'\n')
print(json.dumps(result,indent=2))
