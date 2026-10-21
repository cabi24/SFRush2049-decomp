BASE = 'work/a820_p4'
S = "    static f32 mapPos[3] = { -100.0f, 30.0f, 200.0f };\n    s32 trackIdx;"
P = """                    D_803B7CE4[trackIdx].position[0] = mapPos[0];
                    D_803B7CE4[trackIdx].position[1] = mapPos[1];
                    D_803B7CE4[trackIdx].position[2] = mapPos[2];"""
VARIANTS = {
 'base': [],
 'ext': [(S, "    s32 trackIdx;"), (P, P.replace('mapPos', 'D_803B8388'))],
 'ord210': [(P, """                    D_803B7CE4[trackIdx].position[2] = mapPos[2];
                    D_803B7CE4[trackIdx].position[0] = mapPos[0];
                    D_803B7CE4[trackIdx].position[1] = mapPos[1];""")],
}
