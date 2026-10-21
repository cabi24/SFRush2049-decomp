BASE = 'work/w36d_best'
H0 = "            if (D_803B7CE4[trackIdx].handle != -1) {\n                if (trackIdx < 19) {"
H1 = "            if (D_803B7CE4[trackIdx].handle == -1) continue;\n            {\n                if (trackIdx < 19) {"
I0 = "                if (trackIdx < 6 || trackIdx == 19) {\n                    if (D_803BAAE8[mapIdx].displayList) {"
VARIANTS = {
 'S1_cont': [(H0, H1)],
 'S3_inner_cont': [(I0, "                if (!(trackIdx < 6 || trackIdx == 19)) continue;\n                {\n                    if (D_803BAAE8[mapIdx].displayList) {")],
 'S4_dl_cont': [(I0, "                if (trackIdx < 6 || trackIdx == 19) {\n                    if (!D_803BAAE8[mapIdx].displayList) continue;\n                    {")],
 'S13': [(H0, H1), (I0, "                if (!(trackIdx < 6 || trackIdx == 19)) continue;\n                {\n                    if (D_803BAAE8[mapIdx].displayList) {")],
}
