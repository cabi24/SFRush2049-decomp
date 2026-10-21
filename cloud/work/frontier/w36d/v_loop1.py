E='e5dc.c'; D='dfec.c'
A=(E, "                for (i = 21; i < 41; i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);", "                for (i = 21; i < 41; i++)\n                    D_80146108[i] = reverb_setup(D_8014A118[0].save, i);")
B=(D, "        for (i = 21; i < 41; i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);", "        for (i = 21; i < 41; i++)\n            D_80146108[i] = reverb_setup(D_8014A118[0].save, i);")
C=(D, "        for (i = 21; i < 41; i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);", "        for (i = 21;\n             i < 41;\n             i++) D_80146108[i] = reverb_setup(D_8014A118[0].save, i);")
VARIANTS = {
 'base': [],
 'split_body': [A, B],
 'split_for': [C],
}
