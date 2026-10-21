BASE = 'a400/base'
G = """                    if (freeBlocks >= func_800A3508(2064) && func_800A3518(pak))
                        goto done;
"""
VARIANTS = {
 'nest': [(G, "                    if (freeBlocks >= func_800A3508(2064)) {\n                        if (func_800A3518(pak))\n                            goto done;\n                    }\n")],
 'nest2': [(G, "                    if (freeBlocks >= func_800A3508(2064))\n                        if (func_800A3518(pak))\n                            goto done;\n")],
 'cont': [(G, "                    if (freeBlocks < func_800A3508(2064))\n                        continue;\n                    if (func_800A3518(pak))\n                        goto done;\n")],
}
