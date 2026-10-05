P="    p = &D_801407F0.points[close_index];\n"
V={
 'c1': [(P,"    p = &((Point *) D_801407F0.points)[close_index];\n")],
 'c2': [(P,"    p = (Point *) D_801407F0.points + close_index;\n")],
 'c3': [(P,"    p = (Point *) ((s32) D_801407F0.points + close_index * 6);\n")],
 'c4': [(P,"    p = (Point *) ((u8 *) D_801407F0.points + close_index * sizeof(Point));\n")],
 'c5': [(P,"    p = (Point *) (close_index * 6 + (s32) D_801407F0.points);\n")],
 'c6': [(P,"    i = close_index;\n    p = &D_801407F0.points[i];\n")],
}
