BASE = 'a400/base3'
D = "    s32 i;\n"
L = "        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4) {\n"
LP = "        for (pak = 0; pak < 4; pak++) {\n            if (D_80156CF0[pak].attached)\n                break;\n        }\n        if (pak < 4) {\n"
VARIANTS = {
 'base': [],
 'u32': [(D, "    u32 i;\n")],
 's16': [(D, "    s16 i;\n")],
 'u8': [(D, "    u8 i;\n")],
 's8': [(D, "    s8 i;\n")],
 'u16': [(D, "    u16 i;\n")],
 'pak': [(D, ""), (L, LP)],
}
