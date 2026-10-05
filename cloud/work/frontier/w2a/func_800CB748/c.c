/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Header88 {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s8 player;
    /* 0x07 */ u8 mode;
    /* 0x08 */ u8 pad8[14];
    /* 0x16 */ u8 name[13];
    /* 0x23 */ u8 pad23[5];
    /* 0x28 */ u8 ident[8];
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ f32 score;
    /* 0x3C */ u32 bytes;
    /* 0x40 */ u8 pad40[12];
    /* 0x4C */ u32 offset;
    /* 0x50 */ u32 head;
    /* 0x54 */ u32 tail;
} Header88; /* 0x58 */

typedef struct Record44 {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s8 player;
    /* 0x09 */ u8 mode;
    /* 0x0A */ u8 name[13];
    /* 0x17 */ u8 pad17;
    /* 0x18 */ u8 ident[8];
    /* 0x20 */ f32 score;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ Header88 **handle;
} Record44; /* 0x2C */

typedef struct Row12 {
    Record44 *slots[3];
} Row12;

typedef struct OSMesgQueue OSMesgQueue;

extern s8 D_80156994;
extern Row12 *D_8012E6F8;
extern u8 D_8013FECA;
extern s32 D_80152770;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
void *func_800A47C0(void *dst, const void *src, u32 n);
void *memcpy(void *, const void *, u32);
void *audio_task_complete(s32, s32);

void func_800CB748(Header88 *entry, void *payload) {
    Row12 *row;
    Header88 **old;
    s32 i;
    s32 mode;

    if (D_80156994 == 0) {
        row = D_8012E6F8;
    } else {
        row = &D_8012E6F8[(entry->mode ? 6 : 0) + entry->player];
    }
    for (i = 0; i < D_8013FECA; i++) {
        old = row->slots[i]->handle;
        if (old != 0 && entry->player == (*old)->player && entry->mode == (*old)->mode && !(entry->score < (*old)->score)) {
            break;
        }
    }
    i--;
    if (i >= 0) {
        old = row->slots[0]->handle;
        if (old != 0) {
            osRecvMesg((OSMesgQueue *) &D_80152770, 0, 1);
            audio_reverb_update((u32) old, 0);
            osJamMesg((OSMesgQueue *) &D_80152770, 0, 0);
        }
        if (i > 0) {
            func_800A47C0(row->slots[0], row->slots[1], i * sizeof(Record44));
        }
        row->slots[i]->player = entry->player;
        row->slots[i]->mode = entry->mode;
        memcpy(row->slots[i]->name, entry->name, 13);
        memcpy(row->slots[i]->ident, entry->ident, 8);
        row->slots[i]->score = entry->score;
        row->slots[i]->handle = audio_task_complete(0, entry->bytes + sizeof(Header88));
        memcpy(*row->slots[i]->handle, entry, sizeof(Header88));
        memcpy((u8 *) *row->slots[i]->handle + sizeof(Header88), payload, entry->bytes);
        (*row->slots[i]->handle)->offset = 0;
        (*row->slots[i]->handle)->tail = 0;
        (*row->slots[i]->handle)->head = (*row->slots[i]->handle)->tail;
    }
}
