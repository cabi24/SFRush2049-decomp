D0 = """    CamTbl *tb;
    s16 na;
    u16 m;
    s32 idx;
    s32 i;
    s32 nx;
"""
V = {
 'o1': [(D0, """    CamTbl *tb;
    u16 m;
    s16 na;
    s32 idx;
    s32 i;
    s32 nx;
""")],
 'o2': [(D0, """    s16 na;
    u16 m;
    CamTbl *tb;
    s32 idx;
    s32 i;
    s32 nx;
""")],
 'o3': [(D0, """    CamTbl *tb;
    s32 idx;
    s32 i;
    s32 nx;
    s16 na;
    u16 m;
""")],
 'o4': [("    f32 f;\n    f32 tt;\n    f32 fr;\n", "    f32 f;\n    f32 fr;\n    f32 tt;\n")],
 'o5': [("    f32 f;\n    f32 tt;\n    f32 fr;\n", "    f32 tt;\n    f32 f;\n    f32 fr;\n")],
}
