/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * waves.c (lane w36x5, wave 36): _SetupWaveModel (SetupWaveModel 0x80096CA8, 1,204 B), the inlined DMA-read helper
 * (func_8009715C, deleted stub 0x8009715C), _ReadWave (func_80097164 0x80097164, 344 B) and the handle-wave re-setup
 * (fp_call_wrapper 0x800AC6F4, 104 B).
 *
 * Provenance: MAT3 GameSource/src/waves.c _SetupWaveModel (GC 0x8005BF8C) / _ReadWave (GC 0x8005BE10) for the
 * statement order, names and types; Dreamcast src/waves.c func_8C06588C (= _SetupWaveModel; DC status `differs`,
 * used as a structural lead) for the `modelIdx` copy: every gRomObjDefPtrs/gTexDefPtrs/gPalDefPtrs index goes
 * through modelIdx, gWaveInstance through idx. uopt copy-propagates modelIdx -> idx outside the loop back-edges and
 * counts every propagated occurrence toward idx's colour priority: that is what gives retail's IPA convention
 * (idx in s8, first in t0) and the `sw s8,68(sp)` / reloaded bound of the model loops. Dreamcast func_8C065862 is
 * fp_call_wrapper (re-relocates a handle wave after the heap moved it: first = 0).
 * Builds on w27e/w28d/w29d/w31d (resource-loader drafts: offsets, the uninitialised `offset` read on the first path,
 * tables as {ptr,count} pairs, gTargetPaths as one struct, `x = x + (first ? WADptr : offset)` fixups).
 * N64 differences: no byte swapping (big-endian), no AddTexture/RegisterObjName/strstr arms; the model display lists
 * are walked by entity_lod_select (N64 DL relocation) with render flags computed up front (player count, D_80152570)
 * and gMirrorWaves (D_80140A04); texture and palette data pointers are relocated by IMAGptr; target-path waves are
 * 101..119; _ReadWave loads raw ROM (DMA), lzss or inflate data per wave.
 * DISCLOSED SHAPING DEVICE: the empty `do {} while (0);` between the texture and palette loops (an empty debug-macro
 * expansion, as in the locked func_800A79F4) ends a uopt basic block: it lengthens IMAGptr's and offset's live ranges
 * (colour divisors 3->4 and 14->15) so the gTexDefPtrs address outranks IMAGptr (s0/s1) and i*88 outranks offset
 * (s4/s5) as in retail; force.sh proved those two inversions were the whole residual. `if (IMAGptr) {}` at the same
 * place is equivalent. No unused locals, no pad, no volatile.
 * func_8009715C: the caller-less locked 8-byte stub between SetupWaveModel and _ReadWave is this inlined static. Its
 * inline site is _ReadWave's frame (112 exact; w27e needed three unused locals) and, defined before _ReadWave, its
 * lower .loc lines give as1's tie-break retail's `move a0,s0` before `lw s3`.
 * All are private IPA callees (SetupWaveModel: idx s8, first t0; _ReadWave: idx s8): internal, not on keep.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)

typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct OSIoMesgHdr { u16 type; u8 pri; u8 status; OSMesgQueue *retQueue; } OSIoMesgHdr;
typedef struct OSIoMesg { OSIoMesgHdr hdr; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } OSIoMesg;

typedef struct anon_loaded_ready_14 {   /* gWaveInstance[64], 0x14 */
    signed char loaded;     /* 0x0 */
    signed char ready;      /* 0x1 */
    signed char locked;     /* 0x2 */
    signed char model;      /* 0x3 */
    unsigned char mirrored; /* 0x4 */
    signed char handle;     /* 0x5 */
    unsigned char wave;     /* 0x6 (u16 on GC) */
    unsigned int size;      /* 0x8 */
    unsigned char *ptr;     /* 0xc */
    unsigned char *lastPtr; /* 0x10 */
} anon_loaded_ready_14;

typedef struct anon_base_TOCentries_c { u32 *base; s32 TOCentries; void *TOC; } anon_base_TOCentries_c; /* mb_wad.c */

typedef struct tLod {               /* N64 0x10 (GC anon_texidx_flags_c 0xc) */
    u16 texidx;
    u16 flags;
    f32 lodRad;
    u32 *displayList;               /* 0x8 */
    u32 *vtxList;                   /* 0xc N64 only, relocated with the display list */
} tLod;
typedef struct tModelDef {          /* N64 0x58 */
    char name[16];
    f32 radius;
    s16 unused;
    s16 lodCount;                   /* 0x16 */
    tLod lod[4];                    /* 0x18 */
} tModelDef;
typedef struct tTexture {           /* N64 0x24 */
    char name[16];
    u16 width;
    u16 height;
    u8 size;
    u8 format;
    s16 palIndex;
    char *texBase;                  /* 0x18 */
    u32 flags;
    u32 xflags;
} tTexture;
typedef struct anon_name_start_18 { /* palette, 0x18 */
    char name[16];
    u8 start;
    u8 end;
    u16 format;
    char *palBase;                  /* 0x14 */
} anon_name_start_18;
typedef struct tBasicTargetPath {   /* 0x24 */
    char name[16];
    u32 flags;
    s16 numPts;
    s16 collIdx;
    char *trigPtr;                  /* 0x18 */
    char *pts;                      /* 0x1c */
    void *parent;
} tBasicTargetPath;
typedef struct anon_romObjDefs_count_8 { tModelDef *romObjDefs; s32 count; } anon_romObjDefs_count_8;
typedef struct anon_texDefs_count_8 { tTexture *texDefs; s32 count; } anon_texDefs_count_8;
typedef struct anon_palDefs_count_8 { anon_name_start_18 *palDefs; s32 count; } anon_palDefs_count_8;
typedef struct anon_paths_count_8 { tBasicTargetPath *paths; s32 count; } anon_paths_count_8;

extern anon_loaded_ready_14 gWaveInstance[];        /* 0x80156D38 */
extern anon_romObjDefs_count_8 gRomObjDefPtrs[];    /* 0x801161F4 */
extern anon_texDefs_count_8 gTexDefPtrs[];          /* 0x80151AE8 */
extern anon_palDefs_count_8 gPalDefPtrs[];          /* 0x80138670 */
extern anon_paths_count_8 D_801392D0;               /* gTargetPaths */
#define gTargetPaths D_801392D0
extern s16 active_player_count;                     /* gNumPlayers */
extern s8 D_80152570;                               /* render mode flag: DL flags 0x10 / 0x08 */
extern s8 D_80140A04;                               /* entity_lod_select flag */
extern s32 D_80123564[];                            /* per-wave storage kind: 0 raw ROM, 1 lzss, 2 inflate */
extern s32 D_8011B5BC[];                            /* per-wave ROM address */
extern s32 D_8002E580[];                            /* gWaveSizes */
extern OSMesgQueue D_80035410;                      /* PI DMA completion queue */

void func_80096C28(anon_base_TOCentries_c *wad, u8 *WADptr);              /* MBSetupWad */
void func_80096BBC(anon_base_TOCentries_c *wad, s32 WADptr, s32 offset);  /* MBOffsetWad */
s32 lookup_with_output(anon_base_TOCentries_c *wad, s32 tag, s32 *count); /* MBGetFromWad */
void entity_cull_check(u32 start, u32 end, s32 IMAGptr);
void entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask);
void entity_render_mode(s32 idx);                                         /* MirrorWaveInstance */
void osInvalICache(void *vaddr, s32 nbytes);
void osInvalDCache(void *vaddr, s32 nbytes);
s32 osPiStartDma(OSIoMesg *mb, s32 priority, s32 direction, u32 devAddr, void *vAddr, u32 nbytes, OSMesgQueue *mq);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
void lzss_decompress(s32 src, void *dst);
void inflate_decompress(s32 src, void *dst, s32 flag);

void SetupWaveModel(s32 idx, s32 first) {
    anon_base_TOCentries_c wad;
    u8 *WADptr;
    s32 TXLDptr;
    s32 TXLDcnt;
    s32 IMAGptr;
    u8 *offset;
    s32 i;
    s32 j;
    s32 modelIdx;
    s32 flags;

    flags = 0;
    if (gWaveInstance[idx].model == 0) {
        gWaveInstance[idx].ready = 1;
        return;
    }
    if (first == 0 && gWaveInstance[idx].handle == 0) {
        return;
    }
    if (active_player_count == 1) {
        flags = 2;
    } else if (active_player_count == 2) {
        flags = 4;
    }
    if (D_80152570 != 0) {
        flags |= 0x10;
    } else {
        flags |= 8;
    }
    if (gWaveInstance[idx].handle != 0) {
        WADptr = *(u8 **)gWaveInstance[idx].ptr;
    } else {
        WADptr = gWaveInstance[idx].ptr;
    }
    modelIdx = idx;
    if (first != 0) {
        func_80096C28(&wad, WADptr);
    } else {
        offset = *(u8 **)gWaveInstance[idx].ptr - (u32)gWaveInstance[idx].lastPtr;
        func_80096BBC(&wad, (s32)WADptr, (s32)offset);
    }
    gWaveInstance[idx].lastPtr = *(u8 **)gWaveInstance[idx].ptr;
    gRomObjDefPtrs[modelIdx].romObjDefs = (tModelDef *)lookup_with_output(&wad, 0x4F424844, &gRomObjDefPtrs[modelIdx].count);
    gTexDefPtrs[modelIdx].texDefs = (tTexture *)lookup_with_output(&wad, 0x54584844, &gTexDefPtrs[modelIdx].count);
    gPalDefPtrs[modelIdx].palDefs = (anon_name_start_18 *)lookup_with_output(&wad, 0x504C4844, &gPalDefPtrs[modelIdx].count);
    if (gWaveInstance[idx].wave >= 101 && gWaveInstance[idx].wave < 120) {
        gTargetPaths.paths = (tBasicTargetPath *)lookup_with_output(&wad, 0x50544844, &gTargetPaths.count);
    }
    TXLDptr = lookup_with_output(&wad, 0x54584C44, &TXLDcnt);
    IMAGptr = lookup_with_output(&wad, 0x494D4147, NULL);
    entity_cull_check(TXLDptr, TXLDptr + TXLDcnt, IMAGptr);
    for (i = 0; i < gTexDefPtrs[modelIdx].count; i++) {
        gTexDefPtrs[modelIdx].texDefs[i].texBase += IMAGptr;
    }
    do {
    } while (0);
    for (i = 0; i < gPalDefPtrs[modelIdx].count; i++) {
        gPalDefPtrs[modelIdx].palDefs[i].palBase += IMAGptr;
    }
    if (first != 0) {
        for (i = 0; i < gRomObjDefPtrs[modelIdx].count; i++) {
            for (j = 0; j < gRomObjDefPtrs[modelIdx].romObjDefs[i].lodCount; j++) {
                gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList = (u32 *)((u32)gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList + (u32)(first ? WADptr : offset));
                gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].vtxList = (u32 *)((u32)gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].vtxList + (u32)(first ? WADptr : offset));
                entity_lod_select(gRomObjDefPtrs[modelIdx].romObjDefs[i].lod[j].displayList, (s32)(first ? WADptr : offset), -1, D_80140A04 ? 1 : 0, flags);
            }
        }
    }
    if (gWaveInstance[idx].wave >= 101 && gWaveInstance[idx].wave < 120) {
        for (i = 0; i < gTargetPaths.count; i++) {
            gTargetPaths.paths[i].pts = gTargetPaths.paths[i].pts + (u32)(first ? WADptr : offset);
            gTargetPaths.paths[i].trigPtr = gTargetPaths.paths[i].trigPtr + (u32)(first ? WADptr : offset);
        }
    }
    gWaveInstance[idx].ready = 1;
    entity_render_mode(idx);
}

void func_8009715C(u8 *dst, s32 rom, s32 size) {
    OSIoMesg mb;

    osInvalICache(dst, size);
    osInvalDCache(dst, size);
    osPiStartDma(&mb, 0, 0, rom, dst, size, &D_80035410);
    osRecvMesg(&D_80035410, NULL, 1);
}

void func_80097164(s32 slot) {
    anon_loaded_ready_14 *r;
    u8 *dst;
    s32 size;
    s32 rom;
    u8 type;

    r = &gWaveInstance[slot];
    type = r->wave;
    if (D_80123564[type] == 0) {
        if (r->handle != 0) {
            dst = *(u8 **)r->ptr;
        } else {
            dst = r->ptr;
        }
        rom = D_8011B5BC[type];
        size = D_8002E580[type];
        func_8009715C(dst, rom, size);
    } else if (D_80123564[type] == 1) {
        if (r->handle != 0) {
            dst = *(u8 **)r->ptr;
        } else {
            dst = r->ptr;
        }
        lzss_decompress(D_8011B5BC[type], dst);
    } else {
        if (r->handle != 0) {
            dst = *(u8 **)r->ptr;
        } else {
            dst = r->ptr;
        }
        inflate_decompress(D_8011B5BC[type], dst, 1);
    }
    r->loaded = 1;
    SetupWaveModel(slot, 1);
}

/* Re-relocates a handle wave after the heap moved it (Dreamcast func_8C065862; no GC counterpart):
 * `first` = 0, so _SetupWaveModel applies the offset path. */
void fp_call_wrapper(s32 idx) {
    SetupWaveModel(idx, 0);
}
