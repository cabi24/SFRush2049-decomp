BASE = 'a400/base2'
PK = """                if (func_800A1A3C(pak) && !func_800A35F8(pak) && !func_80094FC4(pak)) {
                    numPaks++;
                    freeBlocks = func_800A35BC(pak);
                    if (freeBlocks >= func_800A3508(2064) && func_800A3518(pak))
                        goto done;
                }
"""
PKF = """                if (!func_800A1A3C(pak) || func_800A35F8(pak) || func_80094FC4(pak))
                    continue;
                numPaks++;
                freeBlocks = func_800A35BC(pak);
                if (freeBlocks >= func_800A3508(2064) && func_800A3518(pak))
                    goto done;
"""
CL = "            if (D_80156CF0[i].attached)\n                break;\n"
VARIANTS = {
 'pkflat': [(PK, PKF)],
 'cl_dead': [(CL, CL + "            numPaks = 0;\n")],
 'both': [(PK, PKF), (CL, CL + "            numPaks = 0;\n")],
 'cl_dead2': [(CL, CL + "            freeBlocks = 0;\n")],
 'both2': [(PK, PKF), (CL, CL + "            freeBlocks = 0;\n")],
}
