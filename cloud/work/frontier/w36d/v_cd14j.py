R='parts/func_8038CCE4.c'
G4=("            GFX(dl, 0xD7000000, 0xFFFFFFFF);\n            GFX(dl, 0xE7000000, 0);\n            GFX(dl, 0xE200001C, 0xC8112230);\n            GFX(dl, 0xFCFFFFFF, 0xFFFE7C38);\n",)
VARIANTS = {
 'base': [],
 'idx4': [(R,)+G4+("            dl[0].w0 = 0xD7000000; dl[0].w1 = 0xFFFFFFFF;\n            dl[1].w0 = 0xE7000000; dl[1].w1 = 0;\n            dl[2].w0 = 0xE200001C; dl[2].w1 = 0xC8112230;\n            dl[3].w0 = 0xFCFFFFFF; dl[3].w1 = 0xFFFE7C38;\n            dl += 4;\n",)],
 'sep_assign': [(R, "            D_803BAAE8[D_8014978C].displayList = (u32 *)(dl = (Gfx *)(((u32)D_803BABC8[D_8014978C] + 7) & ~7));", "            dl = (Gfx *)(((u32)D_803BABC8[D_8014978C] + 7) & ~7);\n            D_803BAAE8[D_8014978C].displayList = (u32 *)dl;")],
}
