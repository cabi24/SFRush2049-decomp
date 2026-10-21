R='parts/func_8038CCF4.c'
src=open('groups/trackscr/parts/func_8038CCF4.c').read()
s=src
s=s.replace("""            dl[0].words.w0 = 0xD7000000; dl[0].words.w1 = 0xFFFFFFFF;
            dl[1].words.w0 = 0xE7000000; dl[1].words.w1 = 0;
            dl[2].words.w0 = 0xE200001C; dl[2].words.w1 = 0xC8112230;
            dl[3].words.w0 = 0xFCFFFFFF; dl[3].words.w1 = 0xFFFE7C38;
            dl += 4;
""","""            gSPTexture(dl++, 0xFFFF, 0xFFFF, 0, 0, 0);
            gDPPipeSync(dl++);
            gDPSetRenderMode(dl++, 0xC8112230, 0);
            gDPSetCombine(dl++, 0xFFFFFF, 0xFFFE7C38);
""")
s2=s.replace("""                    GFX(dl, 0x01004008, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4));
                    GFX(dl, 0x01004010, (u32)D_803BABE8[D_8014978C]);""","""                    gSPVertex(dl++, (Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4, 4, 0);
                    gSPVertex(dl++, D_803BABE8[D_8014978C], 4, 4);""").replace("""                    GFX(dl, 0x01008010, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4));""","""                    gSPVertex(dl++, (Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4, 8, 0);""").replace("""                GFX(dl, 0x06000802, 0x000A0208);
                GFX(dl, 0x06020A04, 0x000C040A);
                GFX(dl, 0x06040C06, 0x000E060C);
                GFX(dl, 0x06060E00, 0x0008000E);""","""                gSP2Triangles(dl++, 0, 4, 1, 0, 5, 1, 4, 0);
                gSP2Triangles(dl++, 1, 5, 2, 0, 6, 2, 5, 0);
                gSP2Triangles(dl++, 2, 6, 3, 0, 7, 3, 6, 0);
                gSP2Triangles(dl++, 3, 7, 0, 0, 4, 0, 7, 0);""").replace("""            GFX(dl, 0xD7000002, 0xFFFFFFFF);
            GFX(dl, 0xDF000000, 0);""","""            gSPTexture(dl++, 0xFFFF, 0xFFFF, 0, 0, 1);
            gSPEndDisplayList(dl++);""")
VARIANTS = {
 'base': [],
 'gbi_first4': [(R, src, s)],
 'gbi_all': [(R, src, s2)],
}
