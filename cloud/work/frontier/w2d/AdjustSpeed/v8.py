EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
PF=[("osPfsRename(controller->pfs","osPfsRename(pfs"),("    func_800A2670(controller);\n","    osPfsFreeBlocks(pfs,&controller->freeBytes);\n")]
D="    s32 result;\n"
C="    controller=&D_80144030[port];\n"
BO=[("        if(result==0) break;\n","        if(result==0) break;\n        if (pfs) {}\n")]
PV=PF+[(D, D+"    char *pfs;\n"),(C, C+"    pfs=controller->pfs;\n")]
OLD="""        controller->error=func_800A1E94(result);
        sync_release_video();
        D_8011EAE8=port;
        D_80144008(port,controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
"""
Y1=[(OLD, """        D_80144008(port,(controller->error=func_800A1E94(result), sync_release_video(), D_8011EAE8=port, controller->error),0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
""")]
Y2=[(OLD, """        D_80144008((controller->error=func_800A1E94(result), sync_release_video(), D_8011EAE8=port, port),controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
""")]
V={
 'q0': PV+BO,
 'q1': PV+BO+Y1,
 'q2': PV+BO+Y2,
 'q3': PV+Y1,
}
