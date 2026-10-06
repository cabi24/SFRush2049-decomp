A='''    func_800DED78(4, m->player, m->center_force, 5000.0f);

    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {'''
V={
 'nl':[(A,'''    func_800DED78(4, m->player, m->center_force, 5000.0f);
    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {''')],
 'vol':[(A,'''    func_800DED78(4, m->player, m->center_force, 5000.0f);

    bump_index = 0;
    volume = 0;
    high_force = 0;
    if (D_80140BE0[p] != 0.0f) {''')],
 'vol2':[(A,'''    func_800DED78(4, m->player, m->center_force, 5000.0f);

    bump_index = 0;
    volume = 0.0f;
    high_force = 0.0f;

    if (D_80140BE0[p] != 0.0f)
        {''')],
 'pre':[(A,'''    func_800DED78(4, m->player, m->center_force, 5000.0f);

    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f)
    {''')],
 'early':[('    p = m->player;\n','    p = m->player;\n    bump_index = 0;\n    high_force = 0.0f;\n'),(A,'''    func_800DED78(4, m->player, m->center_force, 5000.0f);

    if (D_80140BE0[p] != 0.0f) {''')],
}
