O = """                y = 0;
                if (b < 0) {
                    y = 1;
                }
                if (y) {
                }
"""
V = {
 'w1': [(O, """                do {
                    if (b >= 0) {
                        break;
                    }
                } while (0);
""")],
 'w2': [(O, """                do {
                    if (b < 0) {
                        break;
                    }
                } while (0);
""")],
 'w3': [(O, """                switch (b < 0) {
                case 1:
                    break;
                }
""")],
 'w4': [(O, """                if (b < 0) {
                    goto neg;
                }
neg:
""")],
}
