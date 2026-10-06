A='''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {
        if (D_8002EB90 - D_80140BE0[p] > 1.0f || D_8002EB90 < D_80140BE0[p]) {
            D_80140BE0[p] = 0.0f;
        }
    }
'''
B='''    if (D_80142518[p] != 0.0f) {
        if (D_8002EB90 - D_80142518[p] > 0.16666667f || D_8002EB90 < D_80142518[p]) {
            D_80142518[p] = 0.0f;
        }
    }
'''
H='''static void timer_check(f32 *t, f32 limit) {
    if (*t != 0.0f) {
        if (D_8002EB90 - *t > limit || D_8002EB90 < *t) {
            *t = 0.0f;
        }
    }
}
#define range'''
V={
 'tc':[(A,'''    bump_index = 0;
    high_force = 0.0f;
    timer_check(&D_80140BE0[p], 1.0f);
'''),(B,'''    timer_check(&D_80142518[p], 0.16666667f);
'''),('#define range',H)],
 'tc_b':[(A,'''    timer_check(&D_80140BE0[p], 1.0f);
    bump_index = 0;
    high_force = 0.0f;
'''),(B,'''    timer_check(&D_80142518[p], 0.16666667f);
'''),('#define range',H)],
}
