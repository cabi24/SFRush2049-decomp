OLD="""    link = &D_80153E88[player];"""
V={}
B=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11d/race_countdown_display/best.c').read()
V['top']="""    link = &D_80153E88[player];
    gc = &D_80152818[player];"""
# also remove the inner assignment and use gc in tests -- via post-processing hook
POST=[("                gc = &D_80152818[player];\n",""),("D_80152818[player].place_locked != -1","gc->place_locked != -1")]
V['top_test']=V['top']
POST2=POST+[("!D_80152818[player].place_locked","!gc->place_locked")]
V['top_all']=V['top']
POSTMAP={'top':POST[:1],'top_test':POST,'top_all':POST2}
