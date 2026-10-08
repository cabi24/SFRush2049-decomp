# gen2.py: placement x default kind-form x case-351 kind-form (constant-web sharing), w14p round 2
import itertools, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen1
forms = ["1", "1U", "(s8)1", "(u8)1", "(s16)1"]
def make(place, d_k, c_k):
    src = gen1.make(place, 'direct', 'k1')
    # rewrite the kind value in the default and in case 351
    dflt_old = "kind=1; amount=D_80121D64;"
    dflt_new = f"kind={d_k}; amount=D_80121D64;"
    src = src.replace(dflt_old, dflt_new)
    c351_old = "        case 351:\n            kind=1;"
    assert c351_old in src, "351 not found"
    src = src.replace(c351_old, f"        case 351:\n            kind={c_k};")
    return src
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    n = 0
    for place, d, c in itertools.product(['start', 'end', 'goto'], forms, forms):
        open(os.path.join(out, f"v{n:04d}_{place}_d{d}_c{c}.c".replace('(', '').replace(')', '')), 'w').write(make(place, d, c))
        n += 1
    print(n)
