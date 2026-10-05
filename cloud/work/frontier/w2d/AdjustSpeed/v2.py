EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678"
L="""    for(;;) {
        result=osPfsRename(controller->pfs,file->company_code,file->game_code,file->game_name,file->ext_name);
        if(result==0) break;
"""
V={
 'while': [(L, """    while ((result=osPfsRename(controller->pfs,file->company_code,file->game_code,file->game_name,file->ext_name)) != 0) {
""")],
 'nohelp': [("    func_800A2670(controller);\n","    osPfsFreeBlocks(controller->pfs,&controller->freeBytes);\n")],
 'cbvar': [("    s32 result;\n","    s32 result;\n    void (*cb)(void) = func_8008A6A4;\n"),("&status,func_8008A6A4","&status,cb")],
 'errtmp': [("        controller->error=func_800A1E94(result);\n","        result=func_800A1E94(result);\n        controller->error=result;\n")],
 'goto': [(L, """retry_op:
    {
        result=osPfsRename(controller->pfs,file->company_code,file->game_code,file->game_name,file->ext_name);
        if(result==0) goto ok;
"""),("            return;\n        }\n    }\n","            return;\n        }\n        goto retry_op;\n    }\nok:\n")],
 'dowhile': [(L, """    do {
        result=osPfsRename(controller->pfs,file->company_code,file->game_code,file->game_name,file->ext_name);
        if(result==0) break;
"""),("            return;\n        }\n    }\n","            return;\n        }\n    } while (1);\n")],
}
