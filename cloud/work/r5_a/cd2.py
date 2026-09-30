import sys; sys.path.insert(0,'.'); import run
keys=['a','b','c']  # global per role first, values 0..5
base={'c_1':1,'c_2':1,'c_3':1}
import itertools
res=[]
for combo in itertools.product(range(6),repeat=3):
    o=dict(base); o.update(dict(zip(keys,combo)))
    s=run.score(o); res.append((s,combo)); 
res.sort(); print(res[:10])
