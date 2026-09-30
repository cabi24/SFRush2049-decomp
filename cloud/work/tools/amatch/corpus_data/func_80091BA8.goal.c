Entity *func_80091BA8(s32 h)
{
    if (h == -1) {
        return 0;
    }
    if (D_80110244[h & D_80146104].id != h) {
        return 0;
    }
    return &D_80110244[h & D_80146104];
}