/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800C8918 @ 0x800C8918 (628 B): audio/resource shutdown (w17h; whole-program unit EQUAL).
 *   if the shutdown flag D_8011023C is set: release the two pending effect handles (func_800C885C, inlined),
 *   post a type-1 and a type-7 message to the slot queue D_801427A8 (24-byte slots from func_80091B00, taken
 *   under the slot lock D_80142728), busy-wait until no slot of D_80142DD8[128] is in use, clear the flag,
 *   init_wait_completion(), and free three more heap blocks (func_800960CC, the internal heap-free helper).
 * Structure facts (traced, w17h RESULTS.md):
 *   - every helper is umerge-inlined: func_800C885C and func_800960CC (internal, a25 group), the slot
 *     lock/unlock helpers func_80091AF0/func_80091AF8 (given bodies here: the retail caller-less stubs next to
 *     func_80091B00; each nested inline level adds 8 bytes per site, which is what makes the two message
 *     areas 32 bytes and the frame 208), and two file-static copies of object_type1_create/object_type7_create
 *     (the kept locked functions are not inlined by umerge; func_800CB748's static release_handle precedent).
 *   - the busy-wait is an index loop `for (i...) if (used) break; } while (i < 128)`: strength-reduced it
 *     gives retail's END copy (move a1,v1) and the rematerialised table start; pointer loops never do.
 *   - D_80142DD8 is volatile as in the locked allocator func_80091B00.c.
 * SHAPING (disclosed): `temp_v0->type = N; func_80091AF8();` on one line. Statements keep their own source
 *   line through umerge inlining and as1 breaks ready-list ties by line; retail schedules the unlock's
 *   `addiu a0` before the `sb` (the standalone object_type1_create does the opposite), which only a shared line
 *   reproduces (2 words otherwise).
 * No unused locals, no pad, no stand-ins.
 * PACKAGING (w17j): post_type1/post_type7 are non-static (not on the keep list, so uld makes them internal and
 *   umerge inlines and deletes them exactly as the static copies; codegen identical). As file statics their two
 *   deleted-procedure stubs had no symbol and landed directly after the member stub func_80091AF0 in the group
 *   object, which blob_group's extent check reads as `2 extra words`; as globals the stubs carry their own
 *   symbols, so every member's slice ends at the next symbol.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)

typedef struct ShutdownMsg { s16 id; s8 type, used; u8 payload[20]; } ShutdownMsg;
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;

extern volatile ShutdownMsg D_80142DD8[128];
extern s8 D_8011023C;
extern u32 D_8011025C, D_80110260, D_80110244, D_80110248, D_80110270;
extern s32 D_801427A8;
void init_wait_completion(void);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
ShutdownMsg *func_80091B00(void);

void func_800960CC(u32 address);
void func_800C885C(void);
void func_80091AF0(void);
void func_80091AF8(void);
void post_type1(void);
void post_type7(void);

void func_800C8918(void) {
    s32 i;

    if (D_8011023C) {
        func_800C885C();
        post_type1();
        post_type7();
        do {
            for (i = 0; i < 128; i++) {
                if (D_80142DD8[i].used) {
                    break;
                }
            }
        } while (i < 128);
        D_8011023C = 0;
        init_wait_completion();
        func_800960CC(D_80110244);
        func_800960CC(D_80110248);
        func_800960CC(D_80110270);
    }
}

void post_type1(void) {
    ShutdownMsg *sp1C;
    ShutdownMsg *temp_v0;

    func_80091AF0();
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 1; func_80091AF8();
    osSendMesg((OSMesgQueue *) &D_801427A8, sp1C, 0);
}

void post_type7(void) {
    ShutdownMsg *sp1C;
    ShutdownMsg *temp_v0;

    func_80091AF0();
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 7; func_80091AF8();
    osSendMesg((OSMesgQueue *) &D_801427A8, sp1C, 0);
}

