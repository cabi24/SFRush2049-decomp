EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
V={
 'z1': [("    func_800A2670(controller);\n","    osPfsFreeBlocks(controller->pfs,&controller->freeBytes);\n")],
 'z3': [("    Controller *controller;\n","    char *pfs;\n"),("    controller=&D_80144030[port];\n","    pfs=D_80144030[port].pfs;\n"),
        ("osPfsRename(controller->pfs","osPfsRename(pfs"),("controller->error","D_80144030[port].error"),("    func_800A2670(controller);\n","    osPfsFreeBlocks(pfs,&D_80144030[port].freeBytes);\n")],
 'z4': [("    s32 result;\n","    s32 result;\n    char *pfs;\n"),("    controller=&D_80144030[port];\n","    controller=&D_80144030[port];\n    pfs=controller->pfs;\n"),
        ("osPfsRename(controller->pfs","osPfsRename(pfs"),("    func_800A2670(controller);\n","    osPfsFreeBlocks(pfs,&controller->freeBytes);\n")],
}
