void func_800E7D04(s8 *flag)
{
    if (*flag == 0) {
        *flag = 1;
        osCreateMesgQueue(D_80152770, &D_801527A0, 1);
        osJamMesg(D_80152770, 0, 0);
    }
}

void func_800E7D0C(u16 count, u16 a)
{
    Heap *h;

    func_800E7D04(&D_80116488);
    if (count) {}
    h = (Heap *)(((u32)D_8017A640 + 31) & ~31);
    D_801527C8 = h;
    h->end = D_80000318 | 0x80000000;
    if (h->end >= 0x80400001) {
        D_80156994 = 1;
    }
    func_800E7B44(D_801527C8, count, a);
}
