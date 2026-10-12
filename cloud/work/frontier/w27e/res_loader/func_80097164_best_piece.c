void func_80097164(s32 slot) {
    ResSlot *r;
    u8 *dst;
    s32 size;
    s32 rom;
    u8 type;
    u8 *dst2;
    u8 *dst3;
    s32 kind;
    OSIoMesg mb;

    r = &D_80156D38[slot];
    type = r->type;
    if ((&D_80123564)[type] == 0) {
        if (r->f5 != 0) {
            dst = *r->p0C;
        } else {
            dst = (u8 *)r->p0C;
        }
        rom = (&D_8011B5BC)[type];
        size = D_8002E580[type];
        osInvalICache(dst, size);
        osInvalDCache(dst, size);
        osPiStartDma(&mb, 0, 0, rom, dst, size, (OSMesgQueue *)&D_80035410);
        osRecvMesg((OSMesgQueue *)&D_80035410, NULL, 1);
    } else if ((&D_80123564)[type] == 1) {
        if (r->f5 != 0) {
            dst = *r->p0C;
        } else {
            dst = (u8 *)r->p0C;
        }
        lzss_decompress((&D_8011B5BC)[type], dst);
    } else {
        if (r->f5 != 0) {
            dst = *r->p0C;
        } else {
            dst = (u8 *)r->p0C;
        }
        inflate_decompress((&D_8011B5BC)[type], dst, 1);
    }
    r->f0 = 1;
    func_80096CA8(slot, 1);
}
