"""Serialize four frozen machine batches; each retains the two-compiler limit."""
import datetime,json,shutil,subprocess,sys,time
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]

def utc(): return datetime.datetime.now(datetime.timezone.utc).isoformat()
def run(out):
 out=Path(out).resolve();out.mkdir(parents=True,exist_ok=False)
 for lane in ['low','medium','high','xhigh']:
  assert (HERE/lane/'freeze.json').is_file()
 for lane in ['low','medium','high','xhigh']:
  started=utc();mark=time.monotonic();machine=out/lane
  subprocess.run([sys.executable,str(HERE/'prepare.py'),lane],cwd=ROOT,check=True)
  subprocess.run([sys.executable,'tools/cloud/hypothesis_batch.py','run',str(HERE/lane/'batch.json'),'--jobs','2','--out',str(machine)],cwd=ROOT,check=True)
  elapsed=time.monotonic()-mark
  for source,dest in [('public-summary.json','machine-public-summary.json'),('events.jsonl','machine-events.jsonl'),('summary.md','machine-summary.md')]:
   shutil.copyfile(machine/source,HERE/lane/dest)
  (HERE/lane/'coordinator-machine.json').write_text(json.dumps({'start_utc':started,'end_utc':utc(),'elapsed_seconds':elapsed,'scope':'Serialized lane manifest preparation, frozen compiler/scoring/diagnostics batch. Includes orchestration, excludes separate semantics.'},indent=2)+'\n')
  sem=out/('semantics-'+lane)
  with (out/('semantics-'+lane+'.log')).open('w') as log:
   subprocess.run([sys.executable,str(HERE/'verify_sources.py'),str(HERE/lane),str(sem)],cwd=ROOT,stdout=log,stderr=subprocess.STDOUT,check=True)
  shutil.copyfile(sem/'result.json',HERE/lane/'common-semantic-check.json')
  print(lane+' machine and semantics complete',flush=True)
if __name__=='__main__':run(sys.argv[1])
