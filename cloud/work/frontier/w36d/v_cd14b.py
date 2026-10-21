C='ctx2.c'; D='cd14.c'
ST=(D, "            D_803B7CE4[D_8014978C].position[0] = D_803B8370[0];\n            D_803B7CE4[D_8014978C].position[1] = D_803B8370[1];\n            D_803B7CE4[D_8014978C].position[2] = D_803B8370[2];",
       "            D_803B7CE4[D_8014978C].position[0] = mapPos[0];\n            D_803B7CE4[D_8014978C].position[1] = mapPos[1];\n            D_803B7CE4[D_8014978C].position[2] = mapPos[2];")
ST2=(D, "void func_8038CCEC(void)\n{\n", "void func_8038CCEC(void)\n{\n    static f32 mapPos[3] = {-100.0f, 30.0f, 200.0f};\n")
VARIANTS = {
 'base': [],
 'mappos_static': [ST2, ST],
 'all_def': [ST2, ST, (C, "extern Slot72 D_803B7CE4[20];", "Slot72 D_803B7CE4[20];"), ('group.c', "extern s8 D_8014978C;              /* gTrackNum */", "s8 D_8014978C;              /* gTrackNum */")],
 'pos_via_ptr': [(D, "            D_803B7CE4[D_8014978C].position[0] = D_803B8370[0];\n            D_803B7CE4[D_8014978C].position[1] = D_803B8370[1];\n            D_803B7CE4[D_8014978C].position[2] = D_803B8370[2];\n        }\n    }\n    D_8014978C = saveTrackNum;\n}\n\n/* buildBattle",
   "            D_803B7CE4[D_8014978C].position[0] = D_803B8370[0];\n            D_803B7CE4[D_8014978C].position[1] = D_803B8370[1];\n            D_803B7CE4[D_8014978C].position[2] = D_803B8370[2];\n        }\n    }\n    D_8014978C = saveTrackNum;\n}\n\n/* buildBattle")],
}
