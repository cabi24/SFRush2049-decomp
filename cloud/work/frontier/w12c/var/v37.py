A='''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {
        if (D_8002EB90 - D_80140BE0[p] > 1.0f || D_8002EB90 < D_80140BE0[p]) {'''
D=('    f32 volume, high_force;\n','    f32 volume, high_force;\n    f32 t;\n')
V={
 't1':[D,(A,'''    t = D_80140BE0[p];
    bump_index = 0;
    high_force = 0.0f;
    if (t != 0.0f) {
        if (D_8002EB90 - t > 1.0f || D_8002EB90 < t) {''')],
 't2':[D,(A,'''    bump_index = 0;
    t = D_80140BE0[p];
    high_force = 0.0f;
    if (t != 0.0f) {
        if (D_8002EB90 - t > 1.0f || D_8002EB90 < t) {''')],
 't3':[D,(A,'''    t = D_80140BE0[p];
    bump_index = 0;
    high_force = 0.0f;
    if (t != 0.0f) {
        if (D_8002EB90 - D_80140BE0[p] > 1.0f || D_8002EB90 < D_80140BE0[p]) {''')],
}
