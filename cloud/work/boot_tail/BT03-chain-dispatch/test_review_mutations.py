"""Replay the independent reviewer's optimized host mutation checks.
Only its temporary worktree location was made packet-relative.
"""
import hashlib,importlib.util,json,os,pathlib,subprocess,tempfile
P=pathlib.Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('peer_h',P/'test_host.py');h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h)
s=(P/'func_8001CCDC_NONMATCH.c').read_text();cases=[('original',s,True),('wrong_volume_scale',s.replace('127.0f','126.0f'),False),('lost_identifier_snapshot',s.replace('func_8001B29C(identifier,','func_8001B29C(em->identifier34,'),False),('wrong_fade_gate',s.replace('em->flags08 & 0x100000','em->flags08 & 0x200000'),False)]
rows=[]
with tempfile.TemporaryDirectory(prefix='chain-peer-mut-') as t:
 t=pathlib.Path(t)
 for label,source,want in cases:
  c=t/(label+'.c');c.write_text(source);f=t/(label+'_test.c');f.write_text(h.HARNESS.replace('SOURCE',str(c)));exe=t/label
  flags=['-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-O2','-fno-fast-math','-ffp-contract=off','-fexcess-precision=standard']
  subprocess.run(['cc',*flags,str(f),'-o',str(exe)],check=True,capture_output=True,text=True)
  r=subprocess.run([str(exe)],capture_output=True,text=True)
  assert (r.returncode==0)==want,(label,r.returncode,r.stdout,r.stderr)
  rows.append({'case':label,'expected':'PASS' if want else 'REJECT','actual':'PASS' if r.returncode==0 else 'REJECT','exit_code':r.returncode,'source_sha256':hashlib.sha256(source.encode()).hexdigest()})
print(json.dumps({'result':'PASS','compiler_flags':flags,'cases':rows},indent=2))
