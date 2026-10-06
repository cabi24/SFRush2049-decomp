A='''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {'''
V={
 'hf0':[(A,'''    bump_index = 0;
    high_force = 0;
    if (D_80140BE0[p] != 0.0f) {''')],
 'cmp0':[(A,'''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0) {''')],
 'vol_only':[(A,'''    bump_index = 0;
    volume = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {''')],
}
