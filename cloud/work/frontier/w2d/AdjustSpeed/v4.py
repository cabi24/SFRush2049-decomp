EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
OLD="""        controller->error=func_800A1E94(result);
        sync_release_video();
        D_8011EAE8=port;
        D_80144008(port,controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
"""
V={
 'y1': [(OLD, """        D_80144008(port,(controller->error=func_800A1E94(result), sync_release_video(), D_8011EAE8=port, controller->error),0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
""")],
 'y2': [(OLD, """        D_80144008((controller->error=func_800A1E94(result), sync_release_video(), D_8011EAE8=port, port),controller->error,0,0,&retry,&status,func_8008A6A4);
        D_8011EAE8=-1;
""")],
}
