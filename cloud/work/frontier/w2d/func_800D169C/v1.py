P2A="    delta2[0] = D_80152818[0].pos[0] - p[0].pos[0];\n    delta2[1] = D_80152818[0].pos[1] - p[0].pos[1];\n    delta2[2] = D_80152818[0].pos[2] - p[0].pos[2];\n"
P1A="    x = D_80152818[0].pos[0];\n    y = D_80152818[0].pos[1];\n    z = D_80152818[0].pos[2];\n"
V={
 'base': [],
 'ext': [("extern s16 D_8014AA14;","extern s16 D_8014AA14;\nextern f32 D_80152820, D_80152824, D_80152828;"),(P2A,P2A.replace("D_80152818[0].pos[0]","D_80152820").replace("D_80152818[0].pos[1]","D_80152824").replace("D_80152818[0].pos[2]","D_80152828"))],
 'vol': [("extern s16 D_8014AA14;","extern volatile s16 D_8014AA14;")],
 'ptr1': [("    Point *p;\n","    Point *p;\n    Car *car = D_80152818;\n"),(P1A,P1A.replace("D_80152818[0].","car->"))],
 'ptr2': [("    Point *p;\n","    Point *p;\n    Car *car = D_80152818;\n"),(P2A,P2A.replace("D_80152818[0].","car->"))],
}
