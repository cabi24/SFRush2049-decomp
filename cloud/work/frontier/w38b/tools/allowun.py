import json,sys
p=sys.argv[1]+'/group.json'
d=json.load(open(p)); d['allow_unverified']=True
order=['name','members','claims','context','files','keep','flags','allow_unverified','provenance']
out={k:d[k] for k in order if k in d}; out.update({k:v for k,v in d.items() if k not in out})
open(p,'w').write(json.dumps(out,indent=1)+'\n')
