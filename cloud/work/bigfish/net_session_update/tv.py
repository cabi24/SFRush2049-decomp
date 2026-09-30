import sys; sys.path.insert(0,'.')
import ev
def tv(base, reps, show=True):
    t=base
    for o,n in reps:
        assert o in t, o
        t=t.replace(o,n,1)
    r=ev.evaluate(t)
    if show: print(r)
    return t,r
