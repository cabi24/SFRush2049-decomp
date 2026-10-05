D="        default: return 0;\n"
V={
 'l1': [(D, "        default:\n            return 0;\n")],
 'l2': [(D, "        default: mode = 0; return mode;\n")],
 'l3': [(D, "        default: mode = 0;\n            return mode;\n")],
 'l4': [(D, "        default: break;\n"), ("        }\n    }\n    if (mode >= 14","        }\n        return 0;\n    }\n    if (mode >= 14"),("        }\n    }\n    fields = owner","        }\n        return 0;\n    }\n    fields = owner")],
 'l5': [(D, "        default: goto zero;\n"), ("    return fields->fallback[3];\n","    return fields->fallback[3];\nzero:\n    return 0;\n")],
 'l6': [(D, "        default: return 0; break;\n")],
 'l7': [(D, ""), ("        }\n    }\n    if (mode >= 14","        }\n    } else\n    if (mode >= 14")],
}
