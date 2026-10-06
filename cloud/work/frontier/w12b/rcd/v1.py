OLD="""                gc = &D_80152818[player];"""
V={}
V['launder']="""                gc = (GameCar *)(u32)&D_80152818[player];"""
V['launder2']="""                gc = (GameCar *)((u32)D_80152818 + player * sizeof(GameCar));"""
V['plus']="""                gc = D_80152818 + player;"""
