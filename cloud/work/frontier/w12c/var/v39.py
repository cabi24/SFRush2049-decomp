L='''    for (k = 0; k < 4; k++) {
        func_800DED78(k, m->player, m->body_force[k], 5000.0f);
    }
    func_800DED78(4, m->player, m->center_force, 5000.0f);
'''
H='''
static void check_forces_on_car(AudioModel2056 *m) {
    s16 k;

    for (k = 0; k < 4; k++) {
        func_800DED78(k, m->player, m->body_force[k], 5000.0f);
    }
    func_800DED78(4, m->player, m->center_force, 5000.0f);
}
'''
V={
 'after':[(L,'    check_forces_on_car(m);\n'),('#define range','static void check_forces_on_car(AudioModel2056 *m);\n#define range'),('    s16 k;\n','')],
}
