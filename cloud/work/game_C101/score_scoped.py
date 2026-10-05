import sys,os,json
from pathlib import Path
p=Path.home()/"agents/C/scratch/game-C101";wt=Path.home()/"agents/C/wt"
os.environ["CONVEYOR_TOOLKIT"]=str(Path.home()/"rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5")
sys.path.insert(0,str(wt/"tools/conveyor/jobs"));import scoring
out=[]
for obj in ["complete_callers.o","clamp_carrier.o","consumed_carriers.o"]:
 for n in ["camera_target_track","func_80092278","func_80091FBC","func_8009211C"]:
  target=p/("target.o" if n=="camera_target_track" else n+".o")
  scorer=scoring.Scorer(target_o=str(target),stack_differences=True,algorithm="difflib",debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+" --disassemble="+n)
  val,_=scorer.score(str(p/obj));out.append({"object":obj,"function":n,"strict":val});print(obj,n,val)
(p/"scoped_strict_scores.json").write_text(json.dumps({"protocol":"unchanged canonical scorer settings, function-scoped objdump; raw unlinked object slices, no masks or linked substitutions","results":out},indent=2)+"\n")
