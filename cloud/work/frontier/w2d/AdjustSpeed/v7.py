EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
P="    pfs=controller->pfs;\n"
V={
 'r1': [(P, P+"    if (pfs) {}\n")],
 'r2': [(P, P+"    if (pfs) {}\n    if (pfs) {}\n")],
 'r3': [("        if(result==0) break;\n","        if(result==0) break;\n        if (pfs) {}\n")],
 'r4': [("        if(result==0) break;\n","        if(result==0) break;\n        if (pfs) {}\n        if (pfs) {}\n")],
}
