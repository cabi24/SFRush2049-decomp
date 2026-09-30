import json,sys
f=json.load(open('dis.json'))
for n in sys.argv[1:]:
    print('==',n); print('\n'.join(x.split(None,1)[1] if False else x for x in f[n]))
