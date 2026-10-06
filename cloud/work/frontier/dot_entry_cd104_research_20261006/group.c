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
Handle track_process_main(s32, char *, char *, u32, s32, s32);
void slot_state_lookup(Handle, void *, u32);
s32 MaxPathZeroControls(Handle, s32);
s32 track_collision_setup(s32, s32);
void func_80091FBC(void *, Handle, Handle);
void func_800A473C(void *, char *);
void AdjustSpeed(Handle);
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
    s32 off;
    s8 saved;

    dirty = 0;
    save = *F(*h, Handle, 44);
    if (save[12] != 16) {
        if (repair) {
            AdjustSpeed(F(*h, Handle, 8));
            return 0;
        }
        dirty = 1;
    }
    if (format_string_parse(save + 4, (save + 28) - (save + 4)) != F(save, s32, 0)) {
        dirty = 1;
        if (repair) {
            F(save, s32, 0) = format_string_parse(save + 4, (save + 28) - (save + 4));
            slot_state_lookup(F(*h, Handle, 8), save, 4);
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
            slot_state_lookup(F(*h, Handle, 8), save + 56, 15);
        }
    }
    if (format_string_parse(save + 76, 16) != F(save, s32, 72)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 0;
            for (i = 21; i < 41; i++) {
                audio_bus_mix(h, i, D_80146108[i]);
            }
            D_8014978C = saved;
            memset(save + 72, 0, 20);
            slot_state_lookup(F(*h, Handle, 8), save + 72, 20);
        }
    }
    if (format_string_parse(save + 96, 12) != F(save, s32, 92)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 6;
            for (i = 21; i < 41; i++) {
                audio_bus_mix(h, i, D_80146108[i]);
            }
            D_8014978C = saved;
            memset(save + 92, 0, 16);
            slot_state_lookup(F(*h, Handle, 8), save + 92, 16);
        }
    }
    if (format_string_parse(save + 112, 16) != F(save, s32, 108)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 14;
            for (i = 21; i < 41; i++) {
                audio_bus_mix(h, i, D_80146108[i]);
            }
            D_8014978C = saved;
            memset(save + 108, 0, 20);
            slot_state_lookup(F(*h, Handle, 8), save + 108, 20);
        }
    }
    if (format_string_parse(save + 132, 8) != F(save, s32, 128)) {
        dirty = 1;
        if (repair) {
            saved = D_8014978C;
            D_8014978C = 18;
            for (i = 21; i < 41; i++) {
                audio_bus_mix(h, i, D_80146108[i]);
            }
            D_8014978C = saved;
            memset(save + 128, 0, 12);
            slot_state_lookup(F(*h, Handle, 8), save + 128, 12);
        }
    }
    for (off = 0; off < 576; off += 96) {
        if (format_string_parse(save + 144 + off, 92) != F(save + off, s32, 140)) {
            dirty = 1;
            if (repair) {
                memset(save + off + 140, 0, 96);
                slot_state_lookup(F(*h, Handle, 8), save + off + 140, 96);
            }
        }
    }
    for (off = 0; off < 256; off += 64) {
        if (format_string_parse(save + 1296 + off, 60) != F(save + off, s32, 1292)) {
            dirty = 1;
            if (repair) {
                memset(save + off + 1292, 0, 64);
                slot_state_lookup(F(*h, Handle, 8), save + off + 1292, 64);
            }
        }
    }
    for (off = 0; off < 96; off += 12) {
        if (format_string_parse(save + 1552 + off, 8) != F(save + off, s32, 1548)) {
            dirty = 1;
            if (repair) {
                memset(save + off + 1548, 0, 12);
                slot_state_lookup(F(*h, Handle, 8), save + off + 1548, 12);
            }
        }
    }
    for (off = 0; off < 24; off += 24) {
        if (format_string_parse(save + 1648 + off, 20) != F(save + off, s32, 1644)) {
            dirty = 1;
            if (repair) {
                memset(save + off + 1644, 0, 24);
                slot_state_lookup(F(*h, Handle, 8), save + off + 1644, 24);
            }
        }
    }
    for (off = 0; off < 112; off += 28) {
        if (format_string_parse(save + 1672 + off, 24) != F(save + off, s32, 1668)) {
            dirty = 1;
            if (repair) {
                memset(save + off + 1668, 0, 28);
                slot_state_lookup(F(*h, Handle, 8), save + off + 1668, 28);
            }
        }
    }
    if (format_string_parse(save + 1784, 72) != F(save, s32, 1780)) {
        dirty = 1;
        if (repair) {
            memset(save + 1780, 0, 76);
            slot_state_lookup(F(*h, Handle, 8), save + 1780, 76);
        }
    }
    for (i = 0; i < 13; i++) {
        p = save + i * 16;
        if (format_string_parse(p + 1860, 12) != F(p, s32, 1856)) {
            dirty = 1;
            if (repair) {
                draw_number(h, i);
                slot_state_lookup(F(*h, Handle, 8), p + 1856, 16);
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
            AdjustSpeed(F(*h, Handle, 8));
        }
    } else {
        func_800CCCCC(h);
    }
}

typedef struct { char c[9]; } Name9;
extern char D_801211F8[];
extern char D_801211DC[], D_801211E0[], D_80121204[];
extern u8 D_80144D60[][16];
extern Handle D_8013C128[][16];
extern u8 D_8013E5D8[][16][48];
extern u8 D_8012E6D8[];
extern s32 D_8002E8E8[];

Handle func_800CD104(s32 id, char *name) {
    Handle entry;
    u8 *data;
    Handle created;
    Handle node;
    u8 *rec;
    u8 *list;
    s32 attempt;
    s32 i;
    s32 power;
    s32 k;
    char *src;
    char digits[8];
    char file[17];
    s32 c;

    list = D_80144D60[id];
    attempt = 0;
    do {
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
                power = 1;
                k = 4 - i;
                while (k--) {
                    power *= 10;
                }
                if (attempt >= power || *src != '0' || file[9 + i] == 0) {
                    file[9 + i] = *src;
                }
                src--;
            }
        }
        for (node = F(list, Handle, 8); node != NULL; node = *(Handle *)*node) {
            if (func_8008AD04((u8 *)*node + 18, file) == 0 && func_8008AD04((u8 *)*node + 53, D_801211DC) == 0) {
                break;
            }
        }
        attempt++;
    } while (node != NULL);
    created = track_process_main(id, file, D_801211E0, 2064, 0x3544, 0x4E525545);
    if (created == NULL) {
        return NULL;
    }
    entry = &D_8013C128[((u8 *)*created)[16]][((u8 *)*created)[17]];
    rec = D_8013E5D8[((u8 *)*created)[16]][((u8 *)*created)[17]];
    *entry = rec;
    F(rec, Handle, 44) = F(*created, Handle, 72);
    data = *F(*created, Handle, 72);
    func_800CCCCC(entry);
    F(rec, Handle, 8) = created;
    F(*created, void *, 8) = audio_volume_pan;
    F(*F(rec, Handle, 8), void *, 12) = func_800949D4;
    F(data, s32, 4) = D_8002E8E8[159];
    F(data, s32, 8) = ((u32)entry << 3) | id;
    func_800A473C(data + 13, name);
    F(data, s32, 0) = format_string_parse(data + 4, (data + 28) - (data + 4));
    memcpy(rec + 20, data + 13, 13);
    memcpy(rec + 12, data + 4, 8);
    slot_state_lookup(F(rec, Handle, 8), data, 2064);
    MaxPathZeroControls(F(rec, Handle, 8), 0);
    track_collision_setup(((u8 *)*F(rec, Handle, 8))[16], 1);
    for (node = F(D_8012E6D8, Handle, 8); node != NULL; node = *(Handle *)*node) {
        if (func_8008AD04((u8 *)*node + 20, rec + 20) > 0) {
            break;
        }
    }
    func_80091FBC(D_8012E6D8, entry, node);
    return entry;
}
