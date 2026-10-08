# gen3.py: default statement order x shaping if(amount) copies x placement, w14p round 3
import itertools, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gen1
def make(place, order, nshape, shape_pos):
    src = gen1.make(place, 'direct', 'k1')
    if order == 'amount_first':
        src = src.replace("kind=1; amount=D_80121D64;", "amount=D_80121D64; kind=1;")
    # strip existing shaping and insert nshape copies at position
    src = src.replace("        if(amount) {}\n        if(amount) {}\n", "")
    shape = "        if(amount) {}\n" * nshape
    if shape_pos == 'before_cmp':
        src = src.replace("        if(car->kind==kind)", shape + "        if(car->kind==kind)")
    elif shape_pos == 'after_cmp':
        src = src.replace("        if(car->kind==kind) car->amount+=amount;\n        else {car->kind=kind;car->amount=amount;}\n",
                          "        if(car->kind==kind) car->amount+=amount;\n        else {car->kind=kind;car->amount=amount;}\n" + shape)
    else:
        src = src.replace("        if(car->kind==kind)", shape + "        if(car->kind==kind)")  # default
    return src
if __name__ == '__main__':
    out = sys.argv[1]; os.makedirs(out, exist_ok=True)
    n = 0
    for place, order, ns, sp in itertools.product(['start','end','goto'], ['kind_first','amount_first'], [0,1,2], ['before_cmp','after_cmp']):
        text = make(place, order, ns, sp)
        open(os.path.join(out, f"v{n:04d}_{place}_{order}_s{ns}_{sp}.c"), 'w').write(text)
        n += 1
    print(n)
