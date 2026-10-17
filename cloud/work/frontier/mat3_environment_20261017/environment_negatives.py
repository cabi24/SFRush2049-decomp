"""Compile independent semantic mistakes and require model disagreement."""
from pathlib import Path
import json,shutil
from environment_behavior import require,score,candidate,execute,model,fixtures
HERE=Path(__file__).resolve().parent

def verify(out):
    original=(HERE/'group/group.c').read_text()
    changes={'shadow_alpha':('D_8011AD8C.a=192','D_8011AD8C.a=193'),
             'arrow_z':('created->z=1','created->z=2'),
             'team_arrow_flags':('(1<<j)|0x3200','(1<<j)|0x1200')}
    results={}
    for name,(old,new) in changes.items():
        require(original.count(old)==1,'ambiguous negative '+name)
        group=Path(out)/name;group.mkdir(exist_ok=True)
        (group/'group.c').write_text(original.replace(old,new));shutil.copyfile(HERE/'group/group.json',group/'group.json')
        obj=Path(out)/(name+'.o');score.compile_group(group,obj);code=candidate(obj)
        for index,case in enumerate(fixtures()):
            actual=execute(code,case)
            if actual[:2]!=model(case):results[name]=dict(rejected=True,first_divergent_case=index);break
        else:raise ValueError('semantic negative escaped '+name)
    return results
