# gen4.py: break the kind=1 constant web between default and case 351 (w14p round 4)
import itertools, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen1
dforms = ["kind=1;", "kind=0; kind+=1;", "kind=2-1;", "kind=1; kind=kind;", "kind=0; kind++;", "kind=1; if(amount){}"]
cforms = ["kind=1;", "kind=0; kind+=1;", "kind=1; kind=kind;"]
def make(place, d, c):
    src = gen1.make(place, 'direct', 'k1')
    src = src.replace("kind=1; amount=D_80121D64;", f"{d} amount=D_80121D64;")
    src = src.replace("        case 351:\n            kind=1;", f"        case 351:\n            {c}")
    return src
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    n = 0
    for place, d, c in itertools.product(['start','end','goto'], dforms, cforms):
        name = f"v{n:04d}_{place}_d{d}_c{c}"
        name = "".join(ch if ch.isalnum() else "_" for ch in name)
        open(os.path.join(out, name + ".c"), "w").write(make(place, d, c))
        n += 1
    print(n)
