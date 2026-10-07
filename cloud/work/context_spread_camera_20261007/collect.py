"""Metadata-only deduplication of private compiled objects; never emit words."""
import collections,datetime,hashlib,json,struct,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT))
from tools.cloud import score

def write(path,value):path.write_text(json.dumps(value,indent=2)+'\n')
def body(obj):
 syms=score.symbols(obj);words=score.text_words(obj);start=syms['camera_transform']
 end=min((x for x in syms.values() if x>start),default=len(words)*4)
 resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
 if unresolved or unverified or errors: raise ValueError('body relocation verification failed')
 selected=resolved[start//4:end//4]
 raw=b''.join(struct.pack('>I',w) for w in selected)
 counts=collections.Counter(0x80000000|((w&0x3ffffff)<<2) for w in selected if w>>26==3)
 addr=score.image_symbols()
 return raw,{'function_words':len(selected),'relocated_function_sha256':hashlib.sha256(raw).hexdigest(),'list_remove_calls':counts[addr['func_8009211C']],'list_insert_calls':counts[addr['func_80091FBC']]}

def collect(out):
 out=Path(out);rows=[];sources={};elfs={};bodies={};lanes={};equality={}
 old_refs=json.loads((HERE/'prior-body-reference.json').read_text())['rows']
 old_hashes={r['relocated_function_sha256'] for lane in old_refs for r in old_refs[lane]}
 for lane in ['low','medium','high','xhigh']:
  machine=json.loads((HERE/lane/'machine-public-summary.json').read_text());sem=json.loads((HERE/lane/'common-semantic-check.json').read_text())
  semantic={r['id']:r for r in sem['rows']};local=[]
  for row in machine['variants']:
   key=row['id'];objs=list((out/lane/'objects').glob('*-'+key+'.o'));assert len(objs)==1
   raw,metadata=body(objs[0]);h=metadata['relocated_function_sha256']
   if h in equality:assert equality[h]==raw
   equality[h]=raw
   result=dict(row,**metadata,lane=lane,semantic_status=semantic[key]['status'],known_prior_body=h in old_hashes)
   rows.append(result);local.append(result)
   for mapping,value in [(sources,row['source_sha256']),(elfs,row['object_sha256']),(bodies,h)]:mapping.setdefault(value,[]).append(key)
  freeze=json.loads((HERE/lane/'freeze.json').read_text());timing=json.loads((HERE/lane/'coordinator-machine.json').read_text())
  lanes[lane]={'count':len(local),'best_differing':min(r['differing'] for r in local),'best_ids':[r['id'] for r in local if r['differing']==min(x['differing'] for x in local)],'unique_bodies':len({r['relocated_function_sha256'] for r in local}),'machine_seconds':timing['elapsed_seconds'],'freeze':freeze.get('freeze'),'machine_start_utc':timing['start_utc'],'semantic_status':sem['status'],'semantic_seconds':sem['elapsed_seconds']}
  write(HERE/lane/'retained-body-hashes.json',{'rows':local,'scope':'Full relocated camera_transform symbol extent, same-byte equality checked privately across lanes. Metadata only; own extent can include padding.'})
 summary={'generated_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'function':'camera_transform','clean_baseline_differing':533,'prior_best_differing':532,'total_words':557,'variants':len(rows),'unique_sources':len(sources),'unique_elves':len(elfs),'unique_bodies':len(bodies),'new_body_count_vs_prior_spread':len(set(bodies)-old_hashes),'shared_body_groups':[v for v in bodies.values() if len(v)>1],'best_differing':min(r['differing'] for r in rows),'best_ids':[r['id'] for r in rows if r['differing']==min(x['differing'] for x in rows)],'all_compiled_clean':all(r['status']=='scored_mismatch' and not r['errors_count'] and not r['unresolved_count'] and not r['unverified_count'] for r in rows),'all_bounded_pass':all(r['semantic_status']=='bounded_pass' for r in rows),'lanes':lanes,'rows':rows}
 write(HERE/'comparison.json',summary)
 print(json.dumps({k:v for k,v in summary.items() if k not in ['rows','shared_body_groups']},indent=2))
if __name__=='__main__':collect(sys.argv[1])
