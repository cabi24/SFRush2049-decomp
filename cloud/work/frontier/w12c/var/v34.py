A='''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {'''
V={
 'vol0':[(A,'''    bump_index = 0;
    volume = 0.0f;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {''')],
 'hf_first':[(A,'''    high_force = 0.0f;
    bump_index = 0;
    if (D_80140BE0[p] != 0.0f) {''')],
 'after':[(A,'''    if (D_80140BE0[p] != 0.0f) {'''),('''            D_80140BE0[p] = 0.0f;
        }
    }
''','''            D_80140BE0[p] = 0.0f;
        }
    }
    bump_index = 0;
    high_force = 0.0f;
''')],
 'vol0b':[(A,'''    bump_index = 0;
    volume = 0;
    high_force = 0;
    if (D_80140BE0[p] != 0) {''')],
 'tmp':[(A,'''    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {'''.replace('if (D_80140BE0[p] != 0.0f) {','if (D_80140BE0[p]) {'))],
}
