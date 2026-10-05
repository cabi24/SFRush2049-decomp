T="        if (*destination == 255) {\n"
V={
 't_ret': [(T,"        if (*ret == 255) {\n")],
 't_out': [(T,"        if (*out == 255) {\n")],
 't_ret2': [(T,"        if (*ret == 255) {\n"),("if (*destination == 255 || *source == 255)","if (*ret == 255 || *in == 255)")],
 't_out2': [(T,"        if (*out == 255) {\n"),("if (*destination == 255 || *source == 255)","if (*out == 255 || *in == 255)")],
 'r1': [("            *destination = 255;\n            ret = destination;\n            out = ret + 1;\n","            ret = destination;\n            *ret = 255;\n            out = ret + 1;\n")],
 'r2': [("            *destination = 255;\n            ret = destination;\n            out = ret + 1;\n","            *destination = 255;\n            out = destination + 1;\n            ret = destination;\n")],
}
