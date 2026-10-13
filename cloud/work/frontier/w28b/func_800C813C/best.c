/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w28b func_800C813C = GC save.c SaveServer(int forceUpdate, int rumbleOnly) -- complete draft, NOT a match.
 * Unit (alone, callees o32): 211/225 words, frame 72/160. Retail is the head of a 3-function IPA unit:
 * func_800C7EC8 takes fileh in s0 and clobbers s0,s3-s5 (retail saves s0-s8, f20/f22 for it), draw_ui_element
 * takes (ghost t0, repair t1, t2). It cannot be scored meaningfully until both have C bodies in the unit.
 * GC mapping: PS_PakServer = track_render_process, sInitCountdown = D_8011742C, sReadingPak[4] = D_80117434,
 * gPS_Files = D_80144D68, strncmp = func_800950AC, "RUSH2049"/"P" = D_801211BC/D_801211C8; the "P" (player) branch
 * calls func_800C7EC8 (GC _AlreadyReadPlayer/_ReadPlayer), the other branch is GC _AlreadyReadGhost +
 * _ReadGhostHeader inlined: PS_ReadFile = drone_set_catchup(fileh, 0, 88), ghost entry from func_800C7200's pool,
 * _CheckAndRepairGhost-like draw_ui_element(ghost, 0/1), PS_CloseFile? = AdjustSteer, InsertLLE sorted by time
 * into gGhostFiles = D_80152020. N64 differs from GC: one pass over the files (GC: players then ghosts), lastPak
 * D_80117430 skip, channel present/state/busy test.
 * draft_check (saves, attract_pak, race; --callee draw_ui_element=t0,t1,t2, func_800C7EC8=s0/...): race DIVERGES at
 * call #1 (one extra stack word gGameThreadStack+0x1228, frame-size artefact suspected); saves and attract_pak ERROR
 * (host -11) -- logic of the pak paths unverified.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)

typedef struct PakFile PakFile;
typedef struct PakHandle { PakFile *file; } PakHandle;      /* GC tPS_File ** */
struct PakFile {
    PakHandle *next;          /* 0x00 */
    u8 unk04[4];
    void (*remove)(void);     /* 0x08 */
    void (*move)(void);       /* 0x0C */
    u8 unk10[2];
    char name[35];            /* 0x12 */
    char ext[19];             /* 0x35 */
    u8 **data;                /* 0x48 */
};
typedef struct PakChannel {
    s8 present;               /* 0x00 */
    s8 state;                 /* 0x01 */
    u8 unk02[4];
    s8 busy;                  /* 0x06 */
    u8 unk07[0x304 - 7];
} PakChannel;
typedef struct PakList { s32 unk0, unk4; void **head; s32 unkC; } PakList;
typedef struct GhostEntry {   /* 44 bytes, GC ghost list entry */
    void **next;              /* 0x00 */
    PakHandle *fileh;         /* 0x04 */
    s8 track;                 /* 0x08 */
    u8 car;                   /* 0x09 */
    char name[13];            /* 0x0A */
    u8 pad17;
    s32 unk18;                /* 0x18 */
    s32 unk1C;                /* 0x1C */
    f32 time;                 /* 0x20 */
    s32 unk24;                /* 0x24 */
    u8 **data;                /* 0x28 */
} GhostEntry;
typedef struct GhostHeader {
    u8 unk00[6];
    s8 track;                 /* 0x06 */
    u8 car;                   /* 0x07 */
    u8 unk08[14];
    char name[13];            /* 0x16 */
    u8 unk23[5];
    s32 unk28;                /* 0x28 */
    s32 unk2C;                /* 0x2C */
    u8 unk30[8];
    f32 time;                 /* 0x38 */
    u8 unk3C[16];
    s32 unk4C;                /* 0x4C */
} GhostHeader;

extern s8 D_8011742C;         /* GC sInitCountdown */
extern s8 D_80117430;
extern s8 D_80117434[];       /* GC sReadingPak[4] */
extern char D_801211BC[];     /* "RUSH2049" */
extern char D_801211C8[];     /* "P" */
extern PakChannel D_80144030[];
extern PakList D_80144D68[];  /* gPS_Files */
extern PakList D_80152020;    /* gGhostFiles */

void track_render_process(void);                 /* GC PS_PakServer */
s32 func_800950AC(char *, char *, s32);          /* strncmp */
void drone_set_catchup(PakHandle *, s32, s32);   /* GC PS_ReadFile(fileh, offset, size) */
void **func_800C7200(void);                      /* ghost-entry pool allocator */
s32 draw_ui_element(void **ghost, s32 repair, s32 unk);   /* IPA t0, t1, t2 */
void func_800C7EC8(PakHandle *fileh);            /* IPA s0: read a player file */
void AdjustSteer(PakHandle *);
void func_80091FBC(PakList *, void **, void **); /* InsertLLE */
void *memset(void *, s32, u32);
void *memcpy(void *, const void *, u32);
void audio_volume_pan(void);
void func_800949D4(void);

/* GC save.c SaveServer(int forceUpdate, int rumbleOnly) */
void SaveServer(s32 forceUpdate, s32 rumbleOnly) {
    s32 pak;
    s8 *reading;
    s8 lastPak;
    PakHandle *fileh;
    PakHandle *next;
    PakChannel *chan;
    void **ghost;
    void **before;
    GhostEntry *g;
    GhostHeader *header;

    track_render_process();
    if (D_8011742C > 1) {
        D_8011742C--;
        return;
    }
    lastPak = D_80117430;
    for (pak = 0, reading = D_80117434; pak != 4; pak++, reading++) {
        if (D_8011742C == 0 && *reading == 0) {
            chan = &D_80144030[pak];
            if (chan->present == 0 || chan->state == 0 || chan->busy != 0) {
                continue;
            }
        }
        *reading = 1;
        if (lastPak == pak) {
            continue;
        }
        fileh = (PakHandle *)D_80144D68[pak].head;
        lastPak = pak;
        if (fileh != NULL) {
            chan = &D_80144030[pak];
            D_80117430 = lastPak;
            do {
                if (chan->state == 0) {
                    break;
                }
                next = fileh->file->next;
                if (func_800950AC(D_801211BC, fileh->file->name, 8) == 0) {
                    if (func_800950AC(D_801211C8, fileh->file->ext, 1) == 0) {
                        func_800C7EC8(fileh);
                    } else {
                        for (ghost = D_80152020.head; ghost != NULL; ghost = ((GhostEntry *)*ghost)->next) {
                            if (((GhostEntry *)*ghost)->fileh == fileh) {
                                goto next_file;
                            }
                        }
                        drone_set_catchup(fileh, 0, 88);
                        if (fileh->file->data == NULL) {
                            goto next_file;
                        }
                        ghost = func_800C7200();
                        g = *ghost;
                        memset(g, 0, 44);
                        g->fileh = fileh;
                        fileh->file->remove = audio_volume_pan;
                        g->fileh->file->move = func_800949D4;
                        g->unk24 = 0;
                        header = (GhostHeader *)*(g->data = fileh->file->data);
                        header->unk4C = 0;
                        if (draw_ui_element(ghost, 0, 0) && !draw_ui_element(ghost, 1, 0)) {
                            *ghost = NULL;
                            goto next_file;
                        }
                        g->track = header->track;
                        g->car = header->car;
                        memcpy(g->name, header->name, 13);
                        g->unk18 = header->unk28;
                        g->unk1C = header->unk2C;
                        g->time = header->time;
                        AdjustSteer(fileh);
                        g->data = NULL;
                        for (before = D_80152020.head; before != NULL; before = ((GhostEntry *)*before)->next) {
                            if (g->time < ((GhostEntry *)*before)->time) {
                                break;
                            }
                        }
                        func_80091FBC(&D_80152020, ghost, before);
                    }
                }
next_file:
                fileh = next;
            } while (fileh != NULL);
        }
        *reading = 0;
        lastPak = -1;
    }
    D_80117430 = lastPak;
    D_8011742C = 0;
}
