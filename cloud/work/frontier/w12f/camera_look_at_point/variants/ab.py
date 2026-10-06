O = """                x = a < 0 ? -a : a;
                y = b < 0 ? -b : b;"""
V = {
 'b1': [(O, """                if (a < 0) {
                    x = -a;
                } else {
                    x = a;
                }
                if (b < 0) {
                    y = -b;
                } else {
                    y = b;
                }""")],
 'b2': [(O, """                x = a;
                if (a < 0) {
                    x = -a;
                }
                y = b;
                if (b < 0) {
                    y = -b;
                }""")],
 'b3': [(O, """                x = a < 0 ? -a : a;
                if (b < 0) {
                    y = -b;
                } else {
                    y = b;
                }""")],
 'b4': [(O, """                if (a < 0) {
                    x = -a;
                } else {
                    x = a;
                }
                y = b < 0 ? -b : b;""")],
}
