OLD="""                gc = &D_80152818[player];"""
V={}
V['h1']="""                gc = gcp(player);"""
V['h0']="""                gc = gcp0(player);"""
PRE_OLD="void race_countdown_display(Model *m, f32 time)\n"
PRE="static GameCar *gcp(s32 p) { GameCar *r; if (1) r = &D_80152818[p]; return r; }\nstatic GameCar *gcp0(s32 p) { return &D_80152818[p]; }\n"
