/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w32c: CreatePlayer best = the w30c/w31c body unchanged (group.c context copy), extracted standalone for the unit:
 * blob_unit --tag w32c score CreatePlayer --with THIS --internal func_800CD0FC --internal func_800C7EC0 -> FAIL 241 of
 * 272 words; body 267/272; rows 142, ops 44; frame 224/224. Lead with fewer rows: lead_f2.c (see RESULTS.md). */
typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;
#define NULL ((void *)0)
#define F(expr,type,offset) (*(type *)((u8 *)(expr)+(offset)))
typedef void **Handle;

void *memset(void *, s32, u32);
void *memcpy(void *, const void *, u32);
s32 sprintf(char *, const char *, ...);
u32 format_string_parse(u8 *, u32);
void func_800C77C4(void *);
void audio_bus_mix(Handle, u8, s8);
s8 func_800CCB40(s32);
void func_800CCA04(Handle, s32);
void func_800CC8C8(Handle, s32);
s32 func_8008AD04(void *, void *);
Handle PS_CreateFile(s32, char *, char *, u32, s32, s32);
void PS_WriteFile(Handle, void *, u32);
s32 PS_FlushFile(Handle, s32);
s32 PS_FlushPak(s32, s32);
void func_80091FBC(void *, Handle, Handle);
void func_800A473C(void *, char *);
void pak_delete_file_retry(Handle);
void audio_volume_pan(void);
void func_800949D4(void);
void func_800CCCCC(Handle);
typedef struct { char c[9]; } Name9;
extern Handle D_8013C128[][16];
extern u8 D_8013E5D8[][16][48];
extern u8 D_8012E6D8[];
extern u8 D_80144D60[][16];
extern s32 D_8002E8E8[];
extern char D_801211F8[], D_801211DC[], D_801211E0[], D_80121204[];

/* deleted static at 0x800CD0FC (caller-less stub): GC save.c _Pow(int num, int exp) */
s32 func_800CD0FC(s32 exp) {
    s32 result = 1;
    while (exp--) {
        result *= 10;
    }
    return result;
}

Handle func_800C7EC0(s32 pak, s32 slot) {   /* deleted static at 0x800C7EC0: GC save.c _NewPlayer (N64: pak, slot) */
    Handle player;

    player = &D_8013C128[pak][slot];
    *player = D_8013E5D8[pak][slot];
    return player;
}

/* GC save.c CreatePlayer(int pak, unsigned char *name) */
void **CreatePlayer(s32 pak, u8 *name) {
    Handle player;
    Handle before;
    u8 *p;
    u8 *data;
    Handle fileh;
    char file[17];
    s32 i;
    s32 attempt;
    char *src;
    s32 c;
    char digits[32];

    for (attempt = 0; ; attempt++) {
        memset(file, 0, 17);
        *(Name9 *)file = *(Name9 *)D_801211F8;
        for (i = 0; i < 6; i++) {
            c = name[i];
            if (c == 0) {
                break;
            }
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '.' || c == ':' || c == '!' ||
                c == '@' || c == '*' || c == '-' || c == '=' || c == '\'' || c == ' ') {
                file[8 + i] = name[i];
            } else {
                file[8 + i] = '*';
            }
        }
        if (attempt > 0) {
            sprintf(digits, D_80121204, 5, attempt);
            src = &digits[4];
            for (i = 4; i >= 0; i--) {
                if (attempt >= func_800CD0FC(4 - i) || *src != '0' || file[9 + i] == 0) {
                    file[9 + i] = *src;
                }
                src--;
            }
        }
        for (before = F(D_80144D60[pak], Handle, 8); before != NULL; before = *(Handle *)*before) {
            if (func_8008AD04((u8 *)*before + 18, file) == 0 && func_8008AD04((u8 *)*before + 53, D_801211DC) == 0) {
                break;
            }
        }
        if (before == NULL) {
            break;
        }
    }
    fileh = PS_CreateFile(pak, file, D_801211E0, 2064, 0x3544, 0x4E525545);
    if (fileh == NULL) {
        return NULL;
    }
    player = func_800C7EC0(((u8 *)*fileh)[16], ((u8 *)*fileh)[17]);
    p = *player;
    data = *(F(p, Handle, 44) = F(*fileh, Handle, 72));
    func_800CCCCC(player);
    F(p, Handle, 8) = fileh;
    F(*fileh, void *, 8) = audio_volume_pan;
    F(*F(p, Handle, 8), void *, 12) = func_800949D4;
    F(data, s32, 4) = D_8002E8E8[159];
    F(data, s32, 8) = ((u32)player << 3) | pak;
    func_800A473C(data + 13, name);
    F(data, s32, 0) = format_string_parse(data + 4, (data + 28) - (data + 4));
    memcpy(p + 20, data + 13, 13);
    memcpy(p + 12, data + 4, 8);
    PS_WriteFile(F(p, Handle, 8), data, 2064);
    PS_FlushFile(F(p, Handle, 8), 0);
    PS_FlushPak(((u8 *)*F(p, Handle, 8))[16], 1);
    for (before = F(D_8012E6D8, Handle, 8); before != NULL; before = *(Handle *)*before) {
        if (func_8008AD04((u8 *)*before + 20, p + 20) > 0) {
            break;
        }
    }
    func_80091FBC(D_8012E6D8, player, before);
    return player;
}
