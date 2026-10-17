"""Bounded source, native-layout, and behavior replay for environment research."""
import importlib.util
from pathlib import Path

def test_environment_research_packet(tmp_path):
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/mat3_environment_20261017/verify.py'
    spec=importlib.util.spec_from_file_location('mat3_environment_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    result=module.verify(tmp_path)
    assert result['claims']==[] and result['new_matching_bytes']==0


def test_environment_guards_survive_optimized_python(tmp_path):
    import subprocess,sys
    root=Path(__file__).resolve().parents[2]
    packet=root/'cloud/work/frontier/mat3_environment_20261017'
    program=r'''
import importlib.util,json,shutil,sys
from pathlib import Path
packet,scratch=map(Path,sys.argv[1:])
spec=importlib.util.spec_from_file_location('environment_proof',packet/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
shutil.copytree(packet,scratch/'packet');v.HERE=scratch/'packet'
def reject(call):
    try:call()
    except ValueError:return
    raise RuntimeError('optimized interpreter removed a proof guard')
p=v.HERE/'group/group.json';data=json.loads(p.read_text());data['claims']=['false-credit'];p.write_text(json.dumps(data))
reject(lambda:v.verify(scratch/'false-claim'))
shutil.copyfile(packet/'group/group.json',p)
p=v.HERE/'verification.json';data=json.loads(p.read_text());data['target_words_sha256']='changed';p.write_text(json.dumps(data))
reject(lambda:v.verify(scratch/'changed-target'))
shutil.copyfile(packet/'verification.json',p)
p=v.HERE/'trace_fidelity.json';data=json.loads(p.read_text());data['inputs_sha256']['verify.py']='changed';p.write_text(json.dumps(data))
reject(lambda:v.verify(scratch/'changed-trace-input'))
shutil.copyfile(packet/'trace_fidelity.json',p)
data=json.loads(p.read_text());data['stock_object_sha256']='changed';p.write_text(json.dumps(data))
reject(lambda:v.verify(scratch/'changed-trace-object'))
'''
    result=subprocess.run([sys.executable,'-O','-c',program,str(packet),str(tmp_path)],text=True,capture_output=True)
    assert result.returncode==0,result.stdout+result.stderr
