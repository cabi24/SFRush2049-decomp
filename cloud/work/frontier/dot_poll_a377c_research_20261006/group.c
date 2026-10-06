/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Observed local polling research candidate; complete context, no accepted coverage claim. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
#ifndef PAK_TYPES_H
#define PAK_TYPES_H
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {
    OSThread *mtqueue, *fullqueue;
    s32 validCount, first, msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32], label[32];
    s32 version, dir_size, inode_table, minode_table, dir_table, inode_start_page;
    u8 banks, activebank;
} OSPfs;
typedef struct OSPfsState {
    u32 file_size, game_code;
    u16 company_code;
    u8 ext_name[4], game_name[16];
} OSPfsState;
typedef struct PakFile40 {
    s8 active;
    u8 flag1, flag2, opaque3;
    u16 bitmap_bytes, flags;
    OSPfsState state;
} PakFile40;
typedef struct Pak772 {
    u8 changed;
    s8 enabled, present, rescan, cached, blocked, motor;
    s8 flag7, refresh, ready, flag10, dirty;
    OSPfs pfs;
    s32 error, free_bytes;
    u8 rumble124, rumble125, rumble126, opaque127;
    s16 rumble128;
    u8 opaque130[2];
    PakFile40 files[16];
} Pak772;
typedef struct PakNode PakNode;
typedef struct PakRequest {
    PakNode *next;
    u32 opaque4;
    void (*done)(PakNode *);
    u32 opaque12;
    u8 port, file;
    u8 name[35], extension[11];
    u32 file_size, pages;
    u8 **buffer;
} PakRequest;
struct PakNode { PakRequest *request; };
typedef struct PakList {
    u8 indirect, doubly, opaque2[2];
    u32 count;
    PakNode *head, *tail;
} PakList;
typedef struct Heap Heap;
typedef s32 (*PakEnabledPredicate)(s32);
typedef void (*PakErrorCallback)(s32, s32, u32, s32, s8 *, s8 *, PakEnabledPredicate);
extern Pak772 D_80144030[];
extern PakList D_80144D60[], D_801460E0;
extern s8 D_8011194C, D_8011EAE8;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern PakErrorCallback D_80144008;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
/* Historical SDK labels, with their actual verified interfaces. */
extern s32 osPfsReadWriteFile(OSPfs *, u16, u32, u8 *, u8 *, s32, s32 *);
extern s32 osPfsDeleteFile(OSPfs *, s32, OSPfsState *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern void func_800A150C(u8 *, u8 *, u8);
extern void func_800A1644(u8 *, u8 *, u8);
extern s32 func_8008AD04(u8 *, u8 *);
extern s32 func_8008A6A4(s32);
extern void AdjustSpeed(PakNode *);
extern void func_8009211C(PakList *, PakNode *);
extern void func_80091FBC(PakList *, PakNode *, PakNode *);
extern void *audio_task_complete(Heap *, u32);
extern void *memset(void *, s32, u32);

typedef struct Controller16 { s8 present; u8 opaque1[15]; } Controller16;
extern s8 D_8011EAE0;
extern Controller16 D_80156CF0[];
extern OSMesgQueue D_80035458;
extern void func_8008A704(void);
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, s32);
/* Historical labels: motor detection/initialization and Pak-ID repair. */
extern s32 osMotorStart(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorStop(OSPfs *);
extern s32 func_800A1910(void *, u8 *, s32);
extern s32 track_collision_setup(s32, s32);
extern void func_800A3640(s32);
extern void func_800A3724(u8);
extern void no_catchup(PakNode *);
extern s32 func_800A1E94(s32);
extern void *memcpy(void *, const void *, u32);
#endif
/* Exact accepted status-conversion body, read from the recorded base. */
s32 func_800A1E94(s32 error) {
    switch (error) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;
        case 4: return 5;
        case 5: return 7;
        case 7: return 6;
        case 8: return 6;
        case 9: return 8;
        case 6: return 9;
        case 10:
        case 11: return 10;
        default: return 15;
    }
}

/* Genuine shared queue operations, also present in accepted A1A60. */
static void pak_queue_init(void)
{
    if (!D_8011194C) {
        D_8011194C = 1;
        osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osJamMesg(&D_801497D0, 0, 0);
    }
}
static void pak_lock(void)
{
    OSMesg message;
    pak_queue_init();
    osRecvMesg(&D_801497D0, &message, 1);
}
static void pak_unlock(void)
{
    osJamMesg(&D_801497D0, 0, 0);
}

/* Complete observed insertion semantics. Single formal is a source hypothesis. */
void no_catchup(PakNode *node)
{
    PakList *list = &D_80144D60[node->request->port];
    func_80091FBC(list, node, list->head);
}

/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Natural full source closure of A3640's private heap-release callee.
 * Reconstructed from native bodies and the base commit's genuine heap group.
 * This is semantic context; no retained-helper, inlining or register claims.
 * Include after pak_types.h, before helpers.c.
 */
typedef struct HeapBlock HeapBlock;
typedef struct HeapPool HeapPool;
struct HeapBlock {
    u32 magic;
    HeapBlock *next, *prev;
    u32 size;
    u32 *owner;
    s8 used, tag;
    u8 counter, opaque23[9];
};
struct HeapPool {
    s32 count;
    u32 *base;
    HeapPool *next;
};
struct Heap {
    u32 magic;
    Heap *next;
    HeapBlock *blocks, *tail;
    u8 *end;
    u32 opaque20;
    HeapPool pool;
};
extern Heap *D_801527C8;

void func_80095EC0(void *memory, u32 size)
{
    u32 *word;
    u32 i, count;

    word = memory;
    count = size >> 2;
    for (i = 0; i < count; i++) *word++ = 0x7FFF0BAD;
}

Heap *func_80095F8C(u32 address)
{
    Heap *heap, *found;

    found = 0;
    for (heap = D_801527C8; heap != 0; heap = heap->next) {
        if (address >= (u32)heap->blocks && address < (u32)heap->end)
            found = heap;
    }
    return found;
}

HeapBlock *func_80095EF4(Heap *heap, u32 address, s32 tag)
{
    HeapPool *pool;
    HeapBlock *block;

    pool = &heap->pool;
    while (pool != 0 && pool->base != 0) {
        if (address >= (u32)pool->base &&
            address < (u32)(pool->base + pool->count)) {
            address = *(u32 *)address;
            break;
        }
        pool = pool->next;
    }
    block = heap->blocks;
    while (block != 0) {
        if (tag != block->tag || address < (u32)block ||
            (block->next != 0 && address >= (u32)block->next))
            block = block->next;
        else break;
    }
    return block;
}

void audio_reverb_update(u32 address, s32 tag)
{
    Heap *heap;
    HeapBlock *block, *next, *prev;

    heap = func_80095F8C(address);
    block = func_80095EF4(heap, address, tag);
    func_80095EC0((u8 *)block + 32, block->size);
    if (block->owner != 0) *block->owner = 0;
    next = block->next;
    if (next != 0 && !next->used) {
        block->next = next->next;
        if (block->next != 0) block->next->prev = block;
        else heap->tail = block;
        block->size += next->size + 32;
        func_80095EC0(next, 32);
    }
    prev = block->prev;
    block->owner = 0;
    block->used = 0;
    block->tag = 0;
    block->counter = 0;
    if (prev != 0 && !prev->used) {
        prev->next = block->next;
        if (prev->next != 0) prev->next->prev = prev;
        else heap->tail = prev;
        prev->size += block->size + 32;
        func_80095EC0(block, 32);
    }
}
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Authored semantic reconstruction, not a compiled or matching submission.
 * Include after pak_types.h. PakList has indirect, doubly and count fields.
 * audio_reverb_update requires its real source in a compiled group: its native
 * address/tag interface is private (a1/a2), not an ordinary external O32 leaf.
 */
extern OSMesgQueue D_80152770;
extern void audio_reverb_update(u32 address, s32 tag);

void func_800A3640(s32 port)
{
    PakNode *node, *next;
    PakRequest *request;
    u8 **buffer;

    node = D_80144D60[port].head;
    while (node != 0) {
        request = node->request;
        next = request->next;
        if (request->done != 0) request->done(node);
        buffer = request->buffer;
        if (buffer != 0) {
            osRecvMesg(&D_80152770, 0, 1);
            audio_reverb_update((u32)buffer, 0);
            osJamMesg(&D_80152770, 0, 0);
            request->buffer = 0;
        }
        func_8009211C(&D_80144D60[node->request->port], node);
        func_80091FBC(&D_801460E0, node, D_801460E0.head);
        node = next;
    }
}

/* Native takes this sole logical port in a3 and does not preserve s0/s1.
 * No artificial leading parameters are part of this source contract.
 */
void func_800A3724(u8 port)
{
    Pak772 *pak;
    s8 blocked;

    pak = &D_80144030[port];
    blocked = pak->blocked;
    memset(pak, 0, sizeof *pak);
    pak->blocked = blocked;
}
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete semantic source for car_lod_select's no-argument status callee.
 * The native function unrolls this four-port loop in pairs.
 */
typedef struct PakControllerStatus4 {
    u16 type;
    u8 status, error;
} PakControllerStatus4;
extern PakControllerStatus4 D_80149440[4];

void func_800A43FC(void)
{
    s32 port;
    Pak772 *pak;

    if (!D_8011EAE0) return;
    for (port = 0; port < 4; port++) {
        pak = &D_80144030[port];
        if ((D_80149440[port].status & 1) &&
            !(D_80149440[port].status & 2)) {
            if (!pak->present) pak->present = 1;
        } else if (pak->present) {
            pak->present = 0;
            pak->rescan = 1;
        }
    }
}
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Genuine 0x800A4508 caller reconstruction. Despite its historical label,
 * this initializes the Controller Pak service. It is an external root:
 * overlay-A func_8039A2A0 calls it with callback address 0x800DE20C.
 * Include after pak_types.h; no synthetic pressure callers are present.
 */
extern PakNode D_80144C60[64];
extern PakRequest D_80144DC0[64];
extern OSThread D_800349F0;
extern u8 D_800329F0[];
extern void audio_queue_process(void *);
extern void osCreateThread(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void osStartThread(OSThread *);
extern void func_800A43FC(void);

void car_lod_select(PakErrorCallback callback)
{
    PakNode *node;
    PakRequest *request;
    s32 port;
    PakList *list;

    D_80144008 = callback;
    D_801460E0.indirect = 1;
    D_801460E0.doubly = 1;
    D_801460E0.count = 0;
    D_801460E0.tail = 0;
    D_801460E0.head = 0;
    node = D_80144C60;
    request = D_80144DC0;
    while (request < &D_80144DC0[64]) {
        node->request = request;
        func_80091FBC(&D_801460E0, node, D_801460E0.head);
        request++;
        node++;
    }
    for (port = 0; port < 4; port++) {
        list = &D_80144D60[port];
        list->indirect = 1;
        list->doubly = 1;
        list->head = 0;
        list->tail = 0;
        list->count = 0;
        D_80144030[port].blocked = 0;
        func_800A3724((u8)port);
    }
    osCreateThread(&D_800349F0, 2, audio_queue_process,
                   0, D_800329F0, 6);
    osStartThread(&D_800349F0);
    D_8011EAE0 = 1;
    func_800A43FC();
}
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
PakNode *track_process_main(s32 port, u8 *name, u8 *extension,
                           u32 size, s32 company, s32 game_code)
{
    Pak772 *pak;
    PakNode *node;
    PakRequest *request;
    OSPfsState *state;
    s32 file, error;
    s8 retry, status;
    u8 game_name[16], ext_name[4];
    u32 file_size;

    pak_lock();
    func_800A150C(game_name, name, 16);
    func_800A150C(ext_name, extension, 4);
    pak = &D_80144030[port];
    for (;;) {
        error = osPfsReadWriteFile(&pak->pfs, (u16)company, (u32)game_code,
                                  game_name, ext_name, (size + 255) & ~255U, &file);
        if (error == 0) break;
        pak->error = func_800A1E94(error);
        pak_unlock();
        D_8011EAE8 = port;
        D_80144008(port, pak->error, (size + 255) >> 8, 0, &retry, &status,
                    pak->error == 3 ? 0 : func_8008A6A4);
        D_8011EAE8 = -1;
        if (pak->error != 3 && !pak->enabled) return 0;
        pak_lock();
        if (pak->error == 8 && retry) {
            for (file = 0; file < 16; file++) {
                state = &D_80144030[port].files[file].state;
                if (func_8008AD04(state->game_name, game_name) == 0 &&
                    func_8008AD04(state->ext_name, ext_name) == 0 &&
                    state->game_code == (u32)game_code &&
                    company == state->company_code) break;
            }
            if (file == 16) continue;
            pak_unlock();
            for (node = D_80144D60[port].head; node != 0;
                 node = node->request->next) {
                if (node->request->file == file) {
                    AdjustSpeed(node);
                    break;
                }
            }
            pak_lock();
        }
        if (pak->error == 6 && retry) retry = 0;
        if (retry) continue;
        pak_unlock();
        return 0;
    }
    osPfsFreeBlocks(&pak->pfs, &pak->free_bytes);
    for (;;) {
        error = osPfsDeleteFile(&pak->pfs, file, &pak->files[file].state);
        if (!error && D_80144030[port].files[file].state.file_size != 0) break;
        pak->error = func_800A1E94(error);
        pak_unlock();
        D_8011EAE8 = port;
        D_80144008(port, pak->error, (size + 255) >> 8, 0,
                    &retry, &status, func_8008A6A4);
        D_8011EAE8 = -1;
        if (!pak->enabled) {
            D_80144030[port].files[file].state.file_size = 0;
            return 0;
        }
        pak_lock();
        if (!retry) {
            pak_unlock();
            D_80144030[port].files[file].state.file_size = 0;
            return 0;
        }
    }
    node = D_801460E0.head;
    func_8009211C(&D_801460E0, node);
    request = node->request;
    request->done = 0;
    request->opaque12 = 0;
    request->port = port;
    request->file = file;
    file_size = D_80144030[port].files[file].state.file_size;
    request->pages = (file_size + 255) >> 8;
    request->file_size = file_size;
    D_80144030[port].files[file].bitmap_bytes = (((file_size + 31) >> 5) + 7) >> 3;
    request->buffer = audio_task_complete(0,
        D_80144030[port].files[file].bitmap_bytes + request->file_size);
    memset(*request->buffer, 0, request->file_size);
    memset(*request->buffer + request->file_size, 255,
        D_80144030[port].files[file].bitmap_bytes);
    D_80144030[port].files[file].active = 0;
    D_80144030[port].files[file].flag1 = 0;
    D_80144030[port].files[file].flag2 = 0;
    func_800A1644(request->name, D_80144030[port].files[file].state.game_name, 16);
    func_800A1644(request->extension, D_80144030[port].files[file].state.ext_name, 4);
    no_catchup(node);
    pak_unlock();
    return node;
}
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Full Controller Pak/motor polling body at 0x800A377C, 3,200 native bytes.
 * Read source/contract.md before choosing compilation-unit visibility.
 * arg0 is genuinely passed by both callers but is not read in the native body.
 * Flag names below are descriptive hypotheses; offsets are the contract.
 */
static s32 pak_has_dirty_bitmap(PakRequest *request)
{
    u8 *bitmap;
    u32 byte;
    if (!request->buffer) return 0;
    bitmap = *request->buffer + request->file_size;
    for (byte = 0; byte < D_80144030[request->port].files[request->file].bitmap_bytes; byte++) {
        if (*bitmap++) return 1;
    }
    return 0;
}

void track_render_process(s32 arg0, s32 suppress_motor_error)
{
    s8 changed[4];
    s8 *change;
    s8 retry = 0, repair = 0;
    s8 same_pak, transfer_pending;
    OSPfs temporary;
    Pak772 *pak, *other;
    PakNode *node;
    PakRequest *request;
    PakFile40 *file;
    s32 port, other_port, result, slot;

    if (!D_8011EAE0) return;
    change = changed;
    do { *change++ = 0; } while (change < changed + 4);
    func_8008A704();
    for (port = 0; port < 4; port++) {
        if (D_8011EAE8 >= 0 && port != D_8011EAE8) continue;
        pak = &D_80144030[port];
        if (!D_80156CF0[port].present || !pak->present || pak->rescan) {
            pak->rescan = 0;
            if (pak->enabled) {
                pak->enabled = 0;
                pak->ready = 0;
                pak->error = 0;
                if (pak->motor) {
                    pak->motor = 0;
                    pak->rumble128 = 0;
                    pak->rumble126 = 0;
                    pak->rumble125 = 0;
                    pak->rumble124 = 0;
                    changed[port] = 1;
                    continue;
                }
                if (!pak->flag10 && !pak->dirty && !pak->flag7) {
                    func_800A3640(port);
                    func_800A3724((u8)port);
                }
                pak->blocked = 0;
                changed[port] = 1;
            } else if (pak->cached && !pak->flag10 && !pak->dirty && !pak->flag7) {
                func_800A3640(port);
                func_800A3724((u8)port);
            }
            continue;
        }
        if (pak->enabled) continue;
        changed[port] = 1;
        pak->enabled = 1;
        same_pak = 0;
        transfer_pending = 0;
        memcpy(&temporary, &pak->pfs, sizeof temporary);
initialize:
        temporary.queue = 0;
        result = osPfsInitPak(&D_80035458, &temporary, port);
        if (!temporary.queue) result = 1;
        pak->error = func_800A1E94(result);
        if (pak->error == 10) {
            if (!osMotorStart(&D_80035458, &temporary, port)) {
                if (!pak->motor) pak->motor = 1;
                else changed[port] = 0;
                continue;
            }
            if (suppress_motor_error) {
                pak->enabled = 0;
                changed[port] = 0;
                continue;
            }
            pak_unlock();
error_prompt:
            D_8011EAE8 = port;
            D_80144008(port, 10, 0, 0, &retry, &repair, func_8008A6A4);
            D_8011EAE8 = -1;
            if (!pak->enabled) {
                pak_lock();
                changed[port] = 0;
                continue;
            }
            if (repair) {
                pak_lock();
                result = osPfsInitPak(&D_80035458, &temporary, port);
                if (!result) goto initialized;
                result = osMotorStop(&temporary);
                pak_unlock();
                if (result) {
                    D_8011EAE8 = port;
                    D_80144008(port, 12, 0, 0, &retry, &repair, 0);
                    D_8011EAE8 = -1;
                    goto error_prompt;
                }
                D_8011EAE8 = port;
                D_80144008(port, 13, 0, 0, &retry, &repair, 0);
                D_8011EAE8 = -1;
            } else if (retry) {
                pak_lock();
                pak->blocked = 1;
                pak->ready = 0;
                changed[port] = 0;
                continue;
            } else {
                D_8011EAE8 = port;
                D_80144008(port, 11, 0, 0, &retry, &repair, 0);
                D_8011EAE8 = -1;
            }
            pak_lock();
            goto initialize;
        }
initialized:
        pak->motor = 0;
        pak->rumble128 = 0;
        pak->rumble126 = 0;
        pak->rumble125 = 0;
        pak->rumble124 = 0;
        if (result == 2) pak->error = 0;
        else if (result) {
            changed[port] = 0;
            pak->blocked = 1;
            pak->ready = 0;
            continue;
        }
        if (!pak->cached) {
            pak->cached = 1;
        } else if (!func_800A1910(pak->pfs.id, temporary.id, 32)) {
            changed[port] = 0;
            memcpy(&pak->pfs, &temporary, sizeof temporary);
            osPfsFreeBlocks(&pak->pfs, &pak->free_bytes);
            pak->ready = 1;
            if (pak->flag7) continue;
            for (node = D_80144D60[port].head; node; node = node->request->next) {
                request = node->request;
                if (pak_has_dirty_bitmap(request)) {
                    pak->files[request->file].flag1 = 1;
                    pak->dirty = 1;
                }
            }
            same_pak = 1;
            goto confirm;
        }
        for (other_port = 0; other_port < 4; other_port++) {
            if (other_port == port) continue;
            other = &D_80144030[other_port];
            if (!other->cached || func_800A1910(temporary.id, other->pfs.id, 32)) continue;
            if (pak->cached) {
                transfer_pending = 1;
                goto transfer_confirm;
            }
transfer:
            other = &D_80144030[other_port];
            memcpy(&pak->pfs, &temporary, sizeof temporary);
            osPfsFreeBlocks(&pak->pfs, &pak->free_bytes);
            pak->ready = 1;
            memcpy(&D_80144D60[port], &D_80144D60[other_port], sizeof(PakList));
            D_80144D60[other_port].head = 0;
            D_80144D60[other_port].tail = 0;
            D_80144D60[other_port].count = 0;
            for (node = D_80144D60[port].head; node; node = node->request->next)
                node->request->port = port;
            memcpy(pak->files, other->files, sizeof pak->files);
            changed[port] = 0;
            other->cached = 0;
            other->ready = 0;
            if (other->enabled && !other->blocked) other->rescan = 1;
            changed[other_port] = 1;
            break;
        }
        if (other_port < 4) continue;
        if (pak->flag7) {
            changed[port] = 0;
            continue;
        }
transfer_confirm:
        if (pak->flag10) {
            changed[port] = 0;
            continue;
        }
confirm:
        pak_unlock();
        if (!track_collision_setup(port, 0)) {
            changed[port] = 0;
            pak_lock();
            continue;
        }
        pak_lock();
        if (pak->refresh && !same_pak) {
            transfer_pending = 0;
            same_pak = 0;
            goto initialize;
        }
        if (same_pak) continue;
        func_800A3640(port);
        if (transfer_pending) goto transfer;
        pak->ready = 1;
        pak->flag10 = 0;
        pak->dirty = 0;
        memcpy(&pak->pfs, &temporary, sizeof temporary);
        osPfsFreeBlocks(&pak->pfs, &pak->free_bytes);
        for (slot = 0; slot < 16; slot++) {
            file = &pak->files[slot];
            result = osPfsDeleteFile(&pak->pfs, slot, &file->state);
            if (result) file->state.file_size = 0;
            file->active = 0;
            file->flag1 = 0;
            file->flag2 = 0;
            file->bitmap_bytes = (((file->state.file_size + 31) >> 5) + 7) >> 3;
            pak->error = func_800A1E94(result);
            if (!file->state.file_size) continue;
            node = D_801460E0.head;
            func_8009211C(&D_801460E0, node);
            request = node->request;
            request->done = 0;
            request->opaque12 = 0;
            request->port = port;
            request->file = slot;
            request->file_size = file->state.file_size;
            request->pages = (file->state.file_size + 255) >> 8;
            func_800A1644(request->name, file->state.game_name, 16);
            func_800A1644(request->extension, file->state.ext_name, 4);
            if (request->extension[0]) request->extension[1] = 0;
            request->buffer = 0;
            no_catchup(node);
        }
    }
    pak_unlock();
    for (port = 0; port < 4; port++) D_80144030[port].changed = changed[port];
}
