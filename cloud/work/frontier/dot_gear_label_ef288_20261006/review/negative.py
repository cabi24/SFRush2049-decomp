"""Host-only wrong-source controls. Never edits frozen source or target C."""
import ctypes,hashlib,json,os,pathlib,subprocess,sys
import audit
P=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(pathlib.Path(sys.argv[1])/'tools/cloud'));import score
words=score.targets()['func_800EF288'];src=(P/'candidate.c').read_text();fixture=(P/'host_audit.c').read_text()
base=next(audit.cases())[1]
def case(event,kind,value=2):
 a=base[:];a[8]=event;a[9]=kind;a[10]=value;return a
positions='            position = &D_80115BE8[D_80151AD0 - 1][i];'
where=src.rfind(positions);assert where>0
stale=src.replace('    u32 width;','    u32 width;\n    const char *width_label;').replace('            width = object_utility(D_8017A4E0.labels[204], -1);','            width_label = D_8017A4E0.labels[204];\n            width = object_utility(width_label, -1);',1).replace('(s16)(y + 1), D_8017A4E0.labels[204]);','(s16)(y + 1), width_label);',1)
variants={
 'reload_gear_instead_of_snapshot':(src.replace('            if (gear == -1) {','            gear = model->gear;\n            if (gear == -1) {'),case(7,4,7)),
 'cache_label_across_width_callback':(stale,case(6,3)),
 'omit_glyph_position_row_reload':(src[:where]+src[where:].replace(positions,'',1),case(10,1,1)),
 'unsigned_gear_load':(src.replace('typedef signed char s8;','typedef unsigned char s8;'),base),
 'cache_loop_player_count':(src.replace('    s32 i;','    s32 i;\n    s32 loop_count;').replace('    for (i = 0; i < D_80151AD0; i++, model++) {','    loop_count = D_80151AD0;\n    for (i = 0; i < loop_count; i++, model++) {'),case(14,1,1)),
}
results={}
for name,(wrong,a) in variants.items():
 d=audit.WORK/name;d.mkdir(exist_ok=True);(d/'candidate.c').write_text(wrong);(d/'host_audit.c').write_text(fixture)
 env=dict(os.environ,TMPDIR=str(audit.WORK))
 subprocess.run(['cc','-std=c99','-O2','-fPIC','-shared',str(d/'host_audit.c'),'-o',str(d/'wrong.so')],env=env,check=True)
 lib=ctypes.CDLL(str(d/'wrong.so'));lib.run_audit.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];lib.run_audit.restype=ctypes.c_int
 inp=(ctypes.c_int*20)(*a);out=(ctypes.c_int*512)();n=lib.run_audit(inp,out);wrong_events=[list(out[i*4:i*4+4]) for i in range(n)]
 expected=audit.AuditMachine(words,a).run();assert wrong_events!=expected,name
 first=next((i for i in range(min(n,len(expected))) if wrong_events[i]!=expected[i]),min(n,len(expected)))
 results[name]={'rejected':True,'first_different_event':first+1,'native_event':expected[first] if first<len(expected) else None,'wrong_source_event':wrong_events[first] if first<n else None}
for name,w in [('unknown_opcode',[0xFFFFFFFF]+words[1:]),('truncated_native_extent',words[:190])]:
 try:audit.AuditMachine(w,base).run()
 except AssertionError as e:results[name]={'rejected':True,'reason':str(e)}
 else:raise AssertionError(name)
class WrongSelector(audit.AuditMachine):
 def hook(self):
  p=self.pc;a0=self.r[4];ok=super().hook()
  if p==audit.SLOT:self.events[-1][1]=audit.signed(a0)
  return ok
assert WrongSelector(words,base).run()!=audit.AuditMachine(words,base).run()
results['wrong_selector_a0_instead_of_s2']={'rejected':True}
(audit.OUTPUT/'negative_controls.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
