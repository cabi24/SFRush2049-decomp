A="    ret = destination;\n    out = destination;\n"
R="            ret = destination;\n            out = ret + 1;\n"
V={
 'u1': [(A,"    ret = (u8 *) (unsigned int) destination;\n    out = destination;\n")],
 'u2': [(A,"    ret = (u8 *) (unsigned int) destination;\n    out = destination;\n"),(R,"            ret = (u8 *) (unsigned int) destination;\n            out = ret + 1;\n")],
 'c1': [("u8 *func_800BE4F0(u8 *destination, u8 *source) {","u8 *func_800BE4F0(char *destination, u8 *source) {"),(A,"    ret = (u8 *) destination;\n    out = (u8 *) destination;\n"),(R,"            ret = (u8 *) destination;\n            out = ret + 1;\n"),("*destination == 255 ||","*ret == 255 ||"),("if (*destination == 255) {","if (*ret == 255) {")],
 'o1': [(A,"    ret = out = destination;\n")],
 'o2': [(A,"    out = destination;\n    ret = out;\n"),(R,"            out = destination + 1;\n            ret = out - 1;\n")],
 'o3': [(R,"            out = destination + 1;\n            ret = out - 1;\n")],
}
