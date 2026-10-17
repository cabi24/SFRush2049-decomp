"""Fast integrity guards and optional full replay for recovered image-A slider."""
from pathlib import Path
import copy, importlib.util, json, os, shutil, subprocess, sys, tempfile, unittest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_ui_overlay_slider_20261017'
spec=importlib.util.spec_from_file_location('slider_guard',PACKET/'verify.py');proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)

class SliderGuards(unittest.TestCase):
 def mutate(self,edit,expected):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t)/'packet';shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'))
   edit(p)
   with self.assertRaisesRegex(RuntimeError,expected):proof.validate_inputs(p)
 def inputs_edit(self,p,edit):
  f=p/'inputs.json';j=json.loads(f.read_text());edit(j);f.write_text(json.dumps(j))
 def test_false_credit(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j['claims'].append(proof.NAME)),'False credit')
 def test_source_change(self):
  self.mutate(lambda p:(p/'candidate.c').write_text((p/'candidate.c').read_text()+'\n/* changed */\n'),'Source changed')
 def test_source_omission(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j['sources'].pop('host.c')),'Source inventory')
 def test_compiler_omission(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j['compiler_sha256'].pop('cfe')),'Compiler inventory')
 def test_tool_omission(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j['tools_sha256'].pop('tools/cloud/score.py')),'Comparison tool inventory')
 def test_tool_fingerprint(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j['tools_sha256'].update({'tools/cloud/owndata.py':'0'*64})),'Comparison tool implementation')
 def test_deliverable_mismatch(self):
  with tempfile.TemporaryDirectory() as t:
   root=Path(t);f=root/proof.DELIVERABLE;f.parent.mkdir(parents=True);f.write_text((PACKET/'candidate.c').read_text()+'\n/* changed deliverable */\n')
   with self.assertRaisesRegex(RuntimeError,'Deliverable source mismatch'):proof.validate_delivery(json.loads((PACKET/'inputs.json').read_text()),root=root)
 def test_target_fingerprint(self):
  self.mutate(lambda p:self.inputs_edit(p,lambda j:j.update(native_sha256='0'*64)),'Native target changed')
 def test_native_word_and_extent_mutations(self):
  old=proof.score.ASM_DIR
  try:
   proof.score.ASM_DIR=ROOT/'asm/us/ovl_a';words=proof.score.targets()[proof.NAME]
  finally:proof.score.ASM_DIR=old
  for index in (0,113,227):
   mutant=list(words);mutant[index]^=1
   with self.subTest(index=index),self.assertRaisesRegex(RuntimeError,'Complete native'):proof.check_native(words,mutant)
  for mutant in (words[:-1],words+[0]):
   with self.assertRaisesRegex(RuntimeError,'Complete native'):proof.check_native(words,mutant)
 def test_relocated_integrity(self):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t)/'packet';shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'))
   code="import importlib.util;from pathlib import Path;p=Path(%r);s=importlib.util.spec_from_file_location('relocated',p/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m);m.validate_inputs()"%str(p)
   env=dict(os.environ,N64_RESEARCH_REPO=str(ROOT));r=subprocess.run([sys.executable,'-O','-c',code],cwd=t,env=env,capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stderr)
 def test_complete_replay(self):
  pins=json.loads((PACKET/'inputs.json').read_text())
  missing=[n for n in pins['compiler_sha256'] if not (proof.score.IDO/n).is_file()]
  missing += [n for n in ('gcc','mips-linux-gnu-ld') if not shutil.which(n)]
  if not (ROOT/'assets/us/data.bin').is_file():missing.append('authenticated private image asset')
  if missing:
   if os.environ.get('REQUIRE_TOOLCHAIN')=='1':self.fail('Required existing tools/input unavailable: '+', '.join(missing))
   self.skipTest('Existing tools/input unavailable: '+', '.join(missing))
  self.assertEqual(proof.measure(),json.loads((PACKET/'verification.json').read_text()))

if __name__=='__main__':unittest.main()
