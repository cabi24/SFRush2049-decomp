BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
C = "            CreateSelectCar(slot, player, cam);\n"
INL = """            music_seq_load(slot, cam, 0);
            model_transform_setup(D_80139320[slot].handle, 1, cam);
            model_data_load(D_80139320[slot].parts[3], 1, cam);
            func_80390A28(slot, player);
"""
INL2 = """            { s32 c = cam;
            music_seq_load(slot, c, 0);
            model_transform_setup(D_80139320[slot].handle, 1, c);
            model_data_load(D_80139320[slot].parts[3], 1, c);
            func_80390A28(slot, player); }
"""
VARIANTS = {}
for nu in (12, 11, 10):
    us = "    s32 " + ", ".join("u%d" % k for k in range(nu)) + ";\n"
    VARIANTS['inl_u%d' % nu] = [(C, INL), (U, us)]
    VARIANTS['inl2_u%d' % nu] = [(C, INL2), (U, us)]
