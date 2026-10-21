D='dfec.c'
VARIANTS = {
 'base': [],
 'savetrack_j': [(D, "        for (i = 0; i < D_8014A108; i++) {\n            if (D_8014A110 == 4) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 14);\n            else if (D_8014A110 == 5) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 18);\n            else if (D_8014A110 == 6) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 6);\n            else menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C);",
   "        for (j = 0; j < D_8014A108; j++) {\n            if (D_8014A110 == 4) menu_text_input(D_8014A118[j].save, D_8014A110, D_8014978C - 14);\n            else if (D_8014A110 == 5) menu_text_input(D_8014A118[j].save, D_8014A110, D_8014978C - 18);\n            else if (D_8014A110 == 6) menu_text_input(D_8014A118[j].save, D_8014A110, D_8014978C - 6);\n            else menu_text_input(D_8014A118[j].save, D_8014A110, D_8014978C);")],
 'getopt_j': [(D, "for (i = 21; i < 41; i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);", "for (j = 21; j < 41; j++) D_80146108[j] = reverb_setup(D_8014A118[0].save, j);")],
 'decl_ji': [(D, "    s32 i;\n    s32 j;\n    f32 pos;", "    s32 j;\n    s32 i;\n    f32 pos;")],
 'decl_pos_first': [(D, "    s32 i;\n    s32 j;\n    f32 pos;", "    f32 pos;\n    s32 i;\n    s32 j;")],
 'search_block_i': [(D, "    i = 0;\n    for (j = 0; j < 19; j++) {\n        if (func_8038B834(j)) {\n            if (j == D_8014978C) { D_803BA856 = i; break; }", "    { s32 k;\n    k = 0;\n    for (j = 0; j < 19; j++) {\n        if (func_8038B834(j)) {\n            if (j == D_8014978C) { D_803BA856 = k; break; }"), (D, "            i++;\n        }\n    }\n    D_803BA852 = 19;", "            k++;\n        }\n    }\n    }\n    D_803BA852 = 19;")],
}
