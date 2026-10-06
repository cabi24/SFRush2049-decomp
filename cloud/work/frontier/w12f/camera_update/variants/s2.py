O = """                    na = cam->s58 + v;
                    if (na != cam->s50) {"""
V = {
 's6': [(O, """                    na = cam->s58 + v;
                    if (cam->s50 != na) {""")],
 's7': [(O, """                    na = v + cam->s58;
                    if (na != cam->s50) {""")],
 's8': [(O, """                    na = cam->s58 + v;
                    if (na == cam->s50) {
                        goto block_99;
                    }
                    {""")],
 's9': [("    s16 na;\n", "    u16 na;\n")],
}
