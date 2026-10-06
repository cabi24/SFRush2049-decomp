O = """                        cam->m[0][i] = cam->m[0][i] * sv[0];
                        cam->m[1][i] = cam->m[1][i] * sv[1];
                        cam->m[2][i] = cam->m[2][i] * sv[2];"""
V = {
 'm1': [(O, """                        cam->m[0][i] = sv[0] * cam->m[0][i];
                        cam->m[1][i] = sv[1] * cam->m[1][i];
                        cam->m[2][i] = sv[2] * cam->m[2][i];""")],
 'm2': [(O, """                        cam->m[0][i] *= sv[0];
                        cam->m[1][i] *= sv[1];
                        cam->m[2][i] *= sv[2];""")],
}
