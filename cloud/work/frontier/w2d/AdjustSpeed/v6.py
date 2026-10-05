EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
PF=[("osPfsRename(controller->pfs","osPfsRename(pfs"),("    func_800A2670(controller);\n","    osPfsFreeBlocks(pfs,&controller->freeBytes);\n")]
CB=[("&status,func_8008A6A4","&status,cb")]
D="    s32 result;\n"
T="    func_8008A704();\n    request=node->req;\n"
C="    controller=&D_80144030[port];\n"
V={
 'w1': PF+CB+[(D, D+"    void (*cb)(void);\n    char *pfs;\n"),(C, C+"    cb=func_8008A6A4;\n    pfs=controller->pfs;\n")],
 'w2': PF+CB+[(D, D+"    void (*cb)(void);\n    char *pfs;\n"),(C, C+"    pfs=controller->pfs;\n    cb=func_8008A6A4;\n")],
 'w3': PF+CB+[(D, D+"    void (*cb)(void);\n    char *pfs;\n"),(T, "    cb=func_8008A6A4;\n"+T),(C, C+"    pfs=controller->pfs;\n")],
 'w4': PF+CB+[(D, D+"    void (*cb)(void) = func_8008A6A4;\n    char *pfs;\n"),(C, C+"    pfs=controller->pfs;\n")],
}
