s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
extern ? D_8000D298;
extern void *D_8002C3DC;

void func_80006F30(void *arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5) {
    s32 sp20;
    s32 temp_s0;

    arg0->unk14 = arg1;
    arg0->unk4 = arg5;
    arg0->unk0 = 0;
    arg0->unk8 = 0;
    arg0->unk11C = arg2;
    arg0->unk38 = (s32) (arg3 >> 0x1F);
    arg0->unk3C = arg3;
    arg0->unkF4 = (s32) (arg4 - 0x10);
    arg0->unkF0 = (s32) (((s32) arg4 >> 0x1F) - (arg4 < 0x10U));
    arg0->unk100 = (s32) ((s32) &D_8000D298 >> 0x1F);
    arg0->unk104 = &D_8000D298;
    sp20 = 0x3FFF01;
    arg0->unk118 = 0xFF03;
    arg0->unk128 = (u32) ((u32) (sp20 & 0x3F0000) >> 0x10);
    arg0->unk12C = 0x01000800;
    arg0->unk18 = 0;
    arg0->unk10 = 1;
    arg0->unk12 = 0;
    temp_s0 = func_8000C4B0();
    arg0->unkC = (void *) D_8002C3DC;
    D_8002C3DC = arg0;
    func_8000C520(temp_s0);
}
