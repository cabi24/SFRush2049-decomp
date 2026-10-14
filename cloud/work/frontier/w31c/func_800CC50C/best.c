/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w31c func_800CC50C = GC save.c CompressGhost(header, blocks), N64 form -- NOT a match (draft at the END of this
 * file; the rest is the packet's group.c with _ReadGhostHeader renamed RGH31 so it can be --with'ed into the unit).
 * Unit: blob_unit --tag w31c score func_800CC50C --with best.c --internal RGH31 --internal DeallocateMemory31
 *   --internal PS_GetBlocks31 -> FAIL 9 of 190 words; rows 12, ops 8; frame 104/104.
 * draft_check (dc.args): EQUIVALENT over 2 calls / 2 scripts (ghost_save 1, ghost_pak 1).
 * Residual: (1) the first heap free's `lw s2,76(s3)` (DeallocateMemory31 parameter) lands in osRecvMesg's delay
 *   slot (retail: before the lock's argument set-up, delay slot = li a2,1) -- as1/ugen placement; (2) the
 *   `gh` (= ghost->header) web gets v1, retail a3 (colour). Disclosed: `gh` is a named local declared between
 *   `num` and `unzippedChecksum` (retail's checksum home is top-36; without it the frame is 96, retail 104).
 */
/*
 * frontier_menu_save_options (wave 30, lane w30c): the N64 save unit (GC GameSource/src/save.c counterparts).
 * Supersedes the locked group of the same name (members func_800CD058, func_800CCEFC, func_800CCCCC,
 * menu_save_options, func_800A44E8, func_800A370C: unchanged; this file was its group.c, w2d/w28b/w29d history).
 * New members (all EQUAL in the whole-program unit, 0 locked bodies differing):
 *   draw_speedometer  = GC _CheckAndRepairPlayer(player, repair); IPA player in s8 (w30c: separate `j` counter for the
 *                       option-repair loops, stats loops indexed by `i`).
 *   func_800C7EC8     = GC _ReadPlayer(fileh); IPA fileh in s0 (w29d).
 *   func_800C7EC0     = caller-less stub: GC _NewPlayer, N64 form (pak, slot), inlined into func_800C7EC8 and CreatePlayer.
 *   draw_ui_element   = GC _CheckAndRepairGhost(ghosth, data, repair); IPA t0, t2, t1 (w29d).
 *   func_800CBF2C     = GC _ReadGhostData(ghosth, decode) + N64 flag `select`; IPA decode s0, ghosth s1, select s3.
 *   func_800C7300     = caller-less stub: GC _FreeGhost(ghosth) (`*ghosth = NULL`), inlined into RGH31.
 *   func_800CD0FC     = caller-less stub: GC _Pow(exp) (num 10 folded), inlined into CreatePlayer (context).
 *   func_800A361C, func_800A1A3C, func_80094FC4 = locked pak-TU getters of D_80144030[pak] bytes 0/1/6, re-typed
 *                       here on the D_80144030 record (same bodies) so umerge's inlined copies in SaveServer share
 *                       one &D_80144030[pak] address web; their single files are superseded (--revert-single).
 * Context (not members): SaveServer (GC SaveServer: EQUAL in the whole-program unit, 176/225 in the group compile;
 *   see w30c RESULTS), CreatePlayer (GC CreatePlayer, w29d draft, 241/272 words), draw_number (locked elsewhere).
 * Context files: ctx_hash.c, ctx_slot.c, ctx_newghost.c (locked group sources; see group.json note).
 * Non-member helper: RGH31 (GC static RGH31 with GC _AlreadyReadGhost's loop inside it):
 *   non-static, not keep; umerge inlines it into SaveServer; retail has no stub for it.
 * Decisions / devices to review:
 *   - SaveServer is defined (void): retail never reads GC's (forceUpdate, rumbleOnly); with those parameters ugen stores
 *     a0/a1 to their homes (2 extra words). Locked callers (save_settings) declare (s32, s32) and call (0, 0): a
 *     parameter-list mismatch across TUs (no false return type). Alternative also EQUAL: (s32 forceUpdate,
 *     s32 rumbleOnly) plus a compiled-out `if (forceUpdate | rumbleOnly) {}` at the top.
 *   - SaveServer's sInitCountdown/sLastPak/sReadingPak are function-local statics: the TU's own .data at 0x8011742C
 *     (= 3), 0x80117430 (= -1), 0x80117434 ({0,0,0,0}); only SaveServer references them in the image.
 *   - RGH31 declares GC's `pak` local nowhere; no unused locals in any member.
 * GC-derived names: _CalcChecksum = format_string_parse, PS_ReadFile = drone_set_catchup, PS_WriteFile, PS_FlushFile,
 *   PS_FlushPak, PS_CreateFile, PS_PakServer = track_render_process, InsertLLE = func_80091FBC, strncmp = func_800950AC,
 *   strcmp = func_8008AD04, _NewGhost = func_800C7200, SavePlayerOption = audio_bus_mix, gOptions = D_80146108,
 *   gTrackNum = D_8014978C, _DecodeGhost = menu_back, gGhostFiles = D_80152020, gPlayerFiles = D_8012E6D8,
 *   gPS_Files = D_80144D60 (+8 = head).
 */
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

/* ---- draw_number: copied from src/blob/groups/draw_number/dn.c (matched there) ---- */
extern s8 D_8011103C[];
extern s8 D_80111080[];
extern s8 D_8011119C[];
extern s8 D_80111230[];
extern s8 D_8011128C[];
extern s8 D_80111604[];
extern s8 D_80111648[];
extern s8 D_8011168C[];
extern s8 D_8011157C[];
extern s8 D_801115C0[];
extern float D_801112DC[];
extern float D_801113E0[];

extern void func_800C7578(void *p, u8 idx, u8 slot, s8 val);

void draw_number(void *p, s32 idx)
{
    func_800C7578(p, (u8)idx, 0, D_8011103C[idx]);
    func_800C7578(p, (u8)idx, 1, D_80111080[idx]);
    func_800C7578(p, (u8)idx, 2, D_8011119C[idx]);
    func_800C7578(p, (u8)idx, 3, D_80111230[idx]);
    func_800C7578(p, (u8)idx, 4, D_8011128C[idx]);
    func_800C7578(p, (u8)idx, 5, D_80111604[idx]);
    func_800C7578(p, (u8)idx, 6, D_80111648[idx]);
    func_800C7578(p, (u8)idx, 7, D_8011168C[idx]);
    func_800C7578(p, (u8)idx, 8, D_8011157C[idx]);
    func_800C7578(p, (u8)idx, 9, D_801115C0[idx]);
    func_800C7578(p, (u8)idx, 10, (s8)(D_801112DC[idx] * 100.0f - 75.0f));
    func_800C7578(p, (u8)idx, 11, (s8)(D_801113E0[idx] * 100.0f - 75.0f));
}

extern s8 D_8014978C;

void func_800CCCCC(Handle h) {
    u8 *data;
    s32 i;
    s8 saved;

    memset(*h, 0, 44);
    data = *F(*h, Handle, 44);
    memset(data, 0, 2064);
    data[12] = 16;
    func_800C77C4(data + 60);
    data[70] = 1;
    F(data, s32, 56) = format_string_parse(data + 60, 11);
    memcpy((u8 *)*h + 33, data + 60, 11);
    for (i = 0; i < 13; i++) {
        draw_number(h, i);
    }
    for (i = 0; i < 21; i++) {
        audio_bus_mix(h, i, func_800CCB40(i));
    }
    saved = D_8014978C;
    for (D_8014978C = 0; D_8014978C < 19; D_8014978C++) {
        for (i = 21; i < 41; i++) {
            audio_bus_mix(h, i, func_800CCB40(i));
        }
    }
    D_8014978C = saved;
    for (data[71] = 0; data[71] < 7; data[71]++) {
        func_800CCA04(h, 2);
        func_800CC8C8(h, 2);
    }
    data[71] = 0;
}

extern s8 D_80146108[];
void drone_set_catchup(Handle, s32, s32);
void func_800CCE5C(Handle, void *);
extern char D_801211CC[];
extern s8 D_801164A1;
extern s16 D_801164A4[];
extern s8 D_801148E4;

s32 draw_speedometer(Handle h, s32 repair) {
    s32 dirty;
    u8 *save;
    u8 *p;
    s32 i;
    s32 j;
    s8 saved;

    dirty = 0;
    save = *F(*h, Handle, 44);
    if (save[12] != 16) {
        if (repair) {
            pak_delete_file_retry(F(*h, Handle, 8));
            return 0;
        }
        dirty = 1;
    }
    if (format_string_parse(save + 4, (save + 28) - (save + 4)) != F(save, s32, 0)) {
        dirty = 1;
        if (repair) {
            F(save, s32, 0) = format_string_parse(save + 4, (save + 28) - (save + 4));
            PS_WriteFile(F(*h, Handle, 8), save, 4);
        }
    }
    if (format_string_parse(save + 32, 21) != F(save, s32, 28)) {
        dirty = 1;
        if (repair) {
            for (i = 0; i < 21; i++) {
                audio_bus_mix(h, i, D_80146108[i]);
            }
        }
    }
    if (format_string_parse(save + 60, 11) != F(save, s32, 56)) {
        dirty = 1;
        if (repair) {
            func_800C77C4((u8 *)*h + 33);
            ((u8 *)*h)[43] = 1;
            memcpy(save + 60, (u8 *)*h + 33, 11);
            F(save, s32, 56) = format_string_parse(save + 60, 11);
            PS_WriteFile(F(*h, Handle, 8), save + 56, 15);
        }
    }
    if (format_string_parse(save + 76, 16) != F(save, s32, 72)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 0;
            for (j = 21; j < 41; j++) {
                audio_bus_mix(h, j, D_80146108[j]);
            }
            D_8014978C = saved;
            memset(save + 72, 0, 20);
            PS_WriteFile(F(*h, Handle, 8), save + 72, 20);
        }
    }
    if (format_string_parse(save + 96, 12) != F(save, s32, 92)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 6;
            for (j = 21; j < 41; j++) {
                audio_bus_mix(h, j, D_80146108[j]);
            }
            D_8014978C = saved;
            memset(save + 92, 0, 16);
            PS_WriteFile(F(*h, Handle, 8), save + 92, 16);
        }
    }
    if (format_string_parse(save + 112, 16) != F(save, s32, 108)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 14;
            for (j = 21; j < 41; j++) {
                audio_bus_mix(h, j, D_80146108[j]);
            }
            D_8014978C = saved;
            memset(save + 108, 0, 20);
            PS_WriteFile(F(*h, Handle, 8), save + 108, 20);
        }
    }
    if (format_string_parse(save + 132, 8) != F(save, s32, 128)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 18;
            for (j = 21; j < 41; j++) {
                audio_bus_mix(h, j, D_80146108[j]);
            }
            D_8014978C = saved;
            memset(save + 128, 0, 12);
            PS_WriteFile(F(*h, Handle, 8), save + 128, 12);
        }
    }
    for (i = 0; i < 6; i++) {
        if (format_string_parse(save + 144 + i * 96, 92) != F(save + i * 96, s32, 140)) {
            dirty = 1;
            if (repair) {
                memset(save + 140 + i * 96, 0, 96);
                PS_WriteFile(F(*h, Handle, 8), save + 140 + i * 96, 96);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (format_string_parse(save + 1296 + i * 64, 60) != F(save + i * 64, s32, 1292)) {
            dirty = 1;
            if (repair) {
                memset(save + 1292 + i * 64, 0, 64);
                PS_WriteFile(F(*h, Handle, 8), save + 1292 + i * 64, 64);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (format_string_parse(save + 1552 + i * 12, 8) != F(save + i * 12, s32, 1548)) {
            dirty = 1;
            if (repair) {
                memset(save + 1548 + i * 12, 0, 12);
                PS_WriteFile(F(*h, Handle, 8), save + 1548 + i * 12, 12);
            }
        }
    }
    for (i = 0; i < 1; i++) {
        if (format_string_parse(save + 1648 + i * 24, 20) != F(save + i * 24, s32, 1644)) {
            dirty = 1;
            if (repair) {
                memset(save + 1644 + i * 24, 0, 24);
                PS_WriteFile(F(*h, Handle, 8), save + 1644 + i * 24, 24);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (format_string_parse(save + 1672 + i * 28, 24) != F(save + i * 28, s32, 1668)) {
            dirty = 1;
            if (repair) {
                memset(save + 1668 + i * 28, 0, 28);
                PS_WriteFile(F(*h, Handle, 8), save + 1668 + i * 28, 28);
            }
        }
    }
    if (format_string_parse(save + 1784, 72) != F(save, s32, 1780)) {
        dirty = 1;
        if (repair) {
            memset(save + 1780, 0, 76);
            PS_WriteFile(F(*h, Handle, 8), save + 1780, 76);
        }
    }
    for (i = 0; i < 13; i++) {
        p = save + i * 16;
        if (format_string_parse(p + 1860, 12) != F(p, s32, 1856)) {
            dirty = 1;
            if (repair) {
                draw_number(h, i);
                PS_WriteFile(F(*h, Handle, 8), p + 1856, 16);
            }
        }
    }
    return dirty;
}

s32 func_800CCEFC(Handle h) {
    u8 *obj;
    void **data;
    s32 i;

    obj = *h;
    if (F(obj, void *, 44) != NULL) {
        return 1;
    }
    drone_set_catchup(F(obj, Handle, 8), 0, F(*F(obj, Handle, 8), s32, 64));
    data = F(*F(obj, Handle, 8), void **, 72);
    if (data == NULL) {
        return 0;
    }
    F(obj, void **, 44) = data;
    if (draw_speedometer(h, 0) && !draw_speedometer(h, 1)) {
        return 0;
    }
    func_800CCE5C(h, obj + 33);
    if (func_8008AD04(obj + 20, D_801211CC) == 0) {
        D_801164A1 = 1;
        for (i = 1; i < 21; i++) {
        }
        D_801164A4[14] = 1;
        D_801164A4[15] = 1;
        D_801164A4[16] = 1;
        D_801148E4 = 1;
    }
    return 1;
}

void func_800CD058(Handle h) {
    if (F(*h, s32, 8) != 0) {
        if (func_800CCEFC(h)) {
            pak_delete_file_retry(F(*h, Handle, 8));
        }
    } else {
        func_800CCCCC(h);
    }
}

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


/* ===== w29d: func_800C7EC8 = GC save.c _ReadPlayer(fileh); IPA: fileh in s0 ===== */
s8 reverb_setup(Handle player, u8 option);      /* GC GetPlayerOption */
void update_viewport(s8 x, s8 y);               /* GC VideoSetOffset */
void func_800A510C(void);                       /* GC LoadTextWave */
void draw_text(Handle player);                  /* GC AddPlayerScores */
void object_render_cleanup(Handle player);      /* GC PurgePlayerData */
extern s8 D_80117428;                           /* GC gGlobalsSet */
extern s16 D_8013FEC8;                          /* GC gLang */

void **func_800C7EC8(void **fileh) {
    Handle player;
    u8 *p;
    Handle before;
    s32 i;

    for (player = F(D_8012E6D8, Handle, 8); player != NULL; player = *(Handle *)*player) {
        if (F(*player, Handle, 8) == fileh) {
            return NULL;
        }
    }
    drone_set_catchup(fileh, 0, F(*fileh, s32, 64));
    if (F(*fileh, void *, 72) == NULL) {
        return NULL;
    }
    player = func_800C7EC0(((u8 *)*fileh)[16], ((u8 *)*fileh)[17]);
    p = *player;
    F(p, Handle, 8) = fileh;
    F(*fileh, void *, 8) = audio_volume_pan;
    F(*F(p, Handle, 8), void *, 12) = func_800949D4;
    F(p, Handle, 44) = F(*F(p, Handle, 8), Handle, 72);
    if (draw_speedometer(player, 0) && !draw_speedometer(player, 1)) {
        return NULL;
    }
    memcpy(p + 20, (u8 *)*F(p, Handle, 44) + 13, 13);
    memcpy(p + 12, (u8 *)*F(p, Handle, 44) + 4, 8);
    memcpy(p + 33, (u8 *)*F(p, Handle, 44) + 60, 11);
    if (!D_80117428 && F(D_8012E6D8, Handle, 8) == NULL) {
        for (i = 0; i < 21; i++) {
            D_80146108[i] = reverb_setup(player, i);
        }
        D_80146108[17] = 1;
        D_8013FEC8 = D_80146108[16] = reverb_setup(player, 16);
        func_800A510C();
        update_viewport(D_80146108[19], D_80146108[20]);
    }
    for (before = F(D_8012E6D8, Handle, 8); before != NULL; before = *(Handle *)*before) {
        if (func_8008AD04((u8 *)*before + 20, p + 20) > 0) {
            break;
        }
    }
    func_80091FBC(D_8012E6D8, player, before);
    draw_text(player);
    object_render_cleanup(player);
    return player;
}

/* ===== w29d: draw_ui_element = GC _CheckAndRepairGhost ===== */
typedef struct GhostHeader {      /* GC __anon_49776; N64 layout 0x58 (from the retail words) */
    u32 headerChecksum;           /* 0x00 */
    u8 revision;                  /* 0x04 */
    s8 flag;                      /* 0x05 */
    s8 track;                     /* 0x06 */
    u8 backward;                  /* 0x07 */
    u8 pad08[0x16 - 0x08];
    u8 name[13];                  /* 0x16 */
    u8 pad23[0x28 - 0x23];
    u32 playerId[2];              /* 0x28 */
    u8 pad30[0x38 - 0x30];
    f32 finish;                   /* 0x38 */
    u32 zippedSize;               /* 0x3C */
    u32 unzippedSize;             /* 0x40 */
    u32 zippedChecksum;           /* 0x44 */
    u32 unzippedChecksum;         /* 0x48 */
    u8 *start;                    /* 0x4C */
    u32 offset;                   /* 0x50 */
    u32 size;                     /* 0x54 */
} GhostHeader;

typedef struct Ghost {            /* GC __anon_49a9c; N64 layout 0x2C */
    void *ll;                     /* 0x00 */
    void **fileh;                 /* 0x04 */
    s8 track;                     /* 0x08 */
    u8 backward;                  /* 0x09 */
    u8 name[13];                  /* 0x0A */
    u8 pad17;
    u32 playerId[2];              /* 0x18 */
    f32 finish;                   /* 0x20 */
    u8 **datah;                   /* 0x24 */
    GhostHeader **header;         /* 0x28 */
} Ghost;


s32 draw_ui_element(Ghost **ghosth, s32 data, s32 repair) {
    s32 result;
    GhostHeader *header;

    result = 0;
    header = *(*ghosth)->header;
    if (header->revision != 17) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    if (header->headerChecksum != format_string_parse(&header->revision, (u8 *)&header->zippedChecksum - (u8 *)header - 4)) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    if (!data) {
        return result;
    }
    if (header->zippedChecksum != format_string_parse((u8 *)(header + 1), header->zippedSize)) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    return result;
}

/* ===== w30c: func_800CBF2C = GC save.c _ReadGhostData(ghosth, decode) + an N64-only third flag;
 * IPA: decode s0, ghosth s1, select s3 ===== */
void menu_transition(Ghost **ghosth);            /* frees the ghost data (GC _FreeGhostData-like) */
void menu_back(Ghost **ghosth);                  /* GC _DecodeGhost */
void menu_item_select(GhostHeader **headerh, s32 offset);
void func_800CB9D0(void *h);

s32 func_800CBF2C(Ghost **ghosth, s32 decode, s32 select) {
    Ghost *ghost;

    ghost = *ghosth;
    if (ghost->header != NULL) {
        if (ghost->fileh != NULL) {
            menu_transition(ghosth);
        } else {
            if (decode) {
                menu_back(ghosth);
            }
            return 1;
        }
    }
    drone_set_catchup(ghost->fileh, 0, F(*ghost->fileh, s32, 64));
    if (F(*ghost->fileh, GhostHeader **, 72) == NULL) {
        return 0;
    }
    ghost->header = F(*ghost->fileh, GhostHeader **, 72);
    (*ghost->header)->start = NULL;
    if (draw_ui_element(ghosth, 1, 0) && !draw_ui_element(ghosth, 1, 1)) {
        return 0;
    }
    if (decode) {
        menu_back(ghosth);
    }
    if (select) {
        menu_item_select(ghost->header, 88);
        func_800CB9D0(ghost->header);
        func_800CB9D0(ghost->datah);
        (*ghost->header)->start = *ghost->datah;
    }
    return 1;
}

/* ===== w29d: func_800C813C = GC save.c SaveServer(forceUpdate, rumbleOnly) ===== */
extern char D_801211BC[];      /* "RUSH2049" */
extern char D_801211C8[];      /* "P" */
typedef struct PakChannel {
    s8 present;                /* 0x00 */
    s8 state;                  /* 0x01 */
    u8 unk02[4];
    s8 busy;                   /* 0x06 */
    u8 unk07[0x304 - 7];
} PakChannel;
extern PakChannel D_80144030[];
/* pak-TU channel getters (locked singles, re-typed on the D_80144030 record so the inlined copies share one
 * address CSE in SaveServer; each still compiles to its locked body) */
s8 func_800A361C(s32 pak) {
    return D_80144030[pak].present;
}

s8 func_800A1A3C(s32 pak) {
    return D_80144030[pak].state;
}

s8 func_80094FC4(s32 pak) {
    return D_80144030[pak].busy;
}
extern u8 D_80152020[];        /* GC gGhostFiles (list head at +8) */
void track_render_process(void);
                 /* GC PS_PakServer */
s32 func_800950AC(char *, char *, s32);          /* strncmp */
Handle func_800C7200(void);                      /* GC _NewGhost */
void AdjustSteer(Handle fileh);

/* deleted static at 0x800C7300 (caller-less stub): GC save.c RGH31(fileh), with GC _AlreadyReadGhost
 * written inline (one `ghosth` variable for the search and the new ghost) */
void func_800C7300(Handle ghosth) {   /* GC _FreeGhost */
    *ghosth = NULL;
}

Handle RGH31(Handle fileh) {
    Handle ghosth;
    Ghost *ghost;
    Handle before;
    GhostHeader *header;

    for (ghosth = F(D_80152020, Handle, 8); ghosth != NULL; ghosth = *(Handle *)*ghosth) {
        if (((Ghost *)*ghosth)->fileh == fileh) {
            return NULL;
        }
    }
    drone_set_catchup(fileh, 0, 88);
    if (F(*fileh, void *, 72) == NULL) {
        return NULL;
    }
    ghosth = func_800C7200();
    ghost = *ghosth;
    memset(ghost, 0, 44);
    ghost->fileh = fileh;
    F(*fileh, void *, 8) = audio_volume_pan;
    F(*ghost->fileh, void *, 12) = func_800949D4;
    ghost->datah = NULL;
    ghost->header = F(*fileh, GhostHeader **, 72);
    header = *ghost->header;
    header->start = NULL;
    if (draw_ui_element((Ghost **)ghosth, 0, 0) && !draw_ui_element((Ghost **)ghosth, 0, 1)) {
        func_800C7300(ghosth);
        return NULL;
    }
    ghost->track = header->track;
    ghost->backward = header->backward;
    memcpy(ghost->name, header->name, 13);
    ghost->playerId[0] = header->playerId[0];
    ghost->playerId[1] = header->playerId[1];
    ghost->finish = header->finish;
    AdjustSteer(fileh);
    ghost->header = NULL;
    for (before = F(D_80152020, Handle, 8); before != NULL; before = *(Handle *)*before) {
        if (ghost->finish < ((Ghost *)*before)->finish) {
            break;
        }
    }
    func_80091FBC(D_80152020, ghosth, before);
    return ghosth;
}

void SaveServer(s32 forceUpdate, s32 rumbleOnly) {
    s32 pak;
    Handle fileh;
    Handle next;
    static s8 sInitCountdown = 3;              /* 0x8011742C */
    static s8 sLastPak = -1;                   /* 0x80117430 */
    static s8 sReadingPak[4] = { 0, 0, 0, 0 }; /* 0x80117434 */

    if (forceUpdate | rumbleOnly) {
    }
    track_render_process();
    if (sInitCountdown > 1) {
        sInitCountdown--;
        return;
    }
    for (pak = 0; pak != 4; pak++) {
        if (sInitCountdown == 0 && sReadingPak[pak] == 0) {
            if (func_800A361C(pak) == 0 || func_800A1A3C(pak) == 0 || func_80094FC4(pak) != 0) {
                continue;
            }
        }
        sReadingPak[pak] = 1;
        if (sLastPak == pak) {
            continue;
        }
        fileh = F(D_80144D60[pak], Handle, 8);
        sLastPak = pak;
        while (fileh != NULL) {
            if (func_800A1A3C(pak) == 0) {
                break;
            }
            next = *(Handle *)*fileh;
            if (func_800950AC(D_801211BC, (char *)*fileh + 18, 8) == 0) {
                if (func_800950AC(D_801211C8, (char *)*fileh + 53, 1) == 0) {
                    func_800C7EC8(fileh);
                } else {
                    RGH31(fileh);
                }
            }
            fileh = next;
        }
        sReadingPak[pak] = 0;
        sLastPak = -1;
    }
    sInitCountdown = 0;
}

/* ===== w31c: func_800CC50C = GC save.c CompressGhost(header, blocks) (N64 form) ===== */
void *func_800A47C0(void *dst, const void *src, u32 n);            /* memmove */
void *sound_play_menu(void *heap, u32 size);                        /* GC AllocateMemoryHeapHigh */
u32 car_angular_velocity_clamp(u8 *next_in, u32 avail_in, u8 *next_out, u32 avail_out,
                               s32 level, s32 windowBits, s32 memLevel);   /* GC zip */
Handle audio_task_complete(void *heap, u32 size);                   /* GC AllocateHandleHeap */
void *audio_buffer_sync(u32 address);                               /* GC GetPtrSize */
void audio_reverb_update(u32 address, s32 tag);
typedef struct OSMesgQueue OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
extern OSMesgQueue D_80152770;
extern f32 D_80111754[];                                            /* GC ghostRaceTimes */
extern f32 D_8002AFB4;                                              /* GC gRetraceRate */

void DeallocateMemory31(void *ptr) {   /* GC DeallocateMemory */
    osRecvMesg(&D_80152770, NULL, 1);
    audio_reverb_update((u32)ptr, 0);
    osSendMesg(&D_80152770, NULL, 0);
}
s32 PS_GetBlocks31(s32 size) {   /* GC PS_GetBlocks */
    return (size + 255) >> 8;
}

Handle CompressGhost(GhostHeader *header, s8 *blocks) {
    Ghost *ghost;
    Handle ghosth;
    s32 maxsize;
    u8 *sorted;
    u8 *gdata;
    u32 dataSize;
    u32 num;
    GhostHeader **gh;
    u32 unzippedChecksum;

    sorted = header->start;
    maxsize = (f32)(D_80111754[D_8014978C] + 10.0) * D_8002AFB4;
    func_800A47C0(header->start + header->size, header->start + maxsize, header->size);
    func_800A47C0(header->start + header->size * 2, header->start + maxsize * 2, header->size);
    num = header->size * 3;
    unzippedChecksum = format_string_parse(sorted, num);
    *(s16 *)((u8 *)header + 36) = 0;
    gdata = sound_play_menu(NULL, num);
    dataSize = car_angular_velocity_clamp(sorted, num, gdata, num, 9, 10, 4);
    DeallocateMemory31(header->start);
    header->start = NULL;
    ghosth = func_800C7200();
    ghost = *ghosth;
    memset(ghost, 0, 44);
    ghost->header = (GhostHeader **)audio_task_complete(NULL, dataSize + 88);
    memcpy(*ghost->header, header, 88);
    (*ghost->header)->revision = 17;
    (*ghost->header)->start = NULL;
    (*ghost->header)->offset = 0;
    (*ghost->header)->size = 0;
    (*ghost->header)->zippedSize = dataSize;
    (*ghost->header)->unzippedSize = num;
    (*ghost->header)->headerChecksum = format_string_parse(&(*ghost->header)->revision,
        (u8 *)&(*ghost->header)->zippedChecksum - (u8 *)*ghost->header - 4);
    (*ghost->header)->zippedChecksum = format_string_parse(gdata, dataSize);
    (*ghost->header)->unzippedChecksum = unzippedChecksum;
    memcpy((u8 *)*ghost->header + 88, gdata, dataSize);
    osRecvMesg(&D_80152770, NULL, 1);
    audio_reverb_update((u32)gdata, 0);
    osSendMesg(&D_80152770, NULL, 0);
    gh = ghost->header;
    ghost->track = (*gh)->track;
    ghost->backward = (*gh)->backward;
    memcpy(ghost->name, (*gh)->name, 13);
    ghost->playerId[0] = (*ghost->header)->playerId[0];
    ghost->playerId[1] = (*ghost->header)->playerId[1];
    ghost->finish = (*ghost->header)->finish;
    *blocks = PS_GetBlocks31((s32)audio_buffer_sync((u32)ghost->header));
    return ghosth;
}
