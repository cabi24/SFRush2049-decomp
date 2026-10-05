EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
OLD="""        controller->error=func_800A1E94(result);
        sync_release_video();
        D_8011EAE8=port;
        D_80144008(port,controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
"""
H="void AdjustSpeed(PakNode *node)\n"
def helper(params, body): return "void func_800A266C(%s) {\n%s}\n\n" % (params, body)
B1="""    controller->error = err;
    osJamMesg(&D_801497D0,0,0);
    D_8011EAE8 = port;
    D_80144008(port, controller->error, 0, 0, retry, status, cb);
    D_8011EAE8 = -1;
"""
V={
 'x1': [(H, helper("Controller *controller, s32 port, s32 err, s8 *retry, s8 *status, void (*cb)(void)", B1)+H), (OLD, "        func_800A266C(controller, port, func_800A1E94(result), &retry, &status, func_8008A6A4);\n")],
 'x2': [(H, helper("s32 port, s32 err, s8 *retry, s8 *status, void (*cb)(void)", B1.replace("controller->","D_80144030[port]."))+H), (OLD, "        func_800A266C(port, func_800A1E94(result), &retry, &status, func_8008A6A4);\n")],
 'x3': [(H, helper("Controller *controller, s32 port, s32 err, s8 *retry, s8 *status", B1.replace(", cb)",", func_8008A6A4)"))+H), (OLD, "        func_800A266C(controller, port, func_800A1E94(result), &retry, &status);\n")],
 'x4': [(OLD, """        controller->error=func_800A1E94(result);
        sync_release_video();
        D_8011EAE8=port;
        (*D_80144008)(port,controller->error,0,0,&retry,&status,&func_8008A6A4);
        D_8011EAE8=-1;
""")],
}
