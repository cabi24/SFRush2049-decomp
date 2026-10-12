/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_effect_setup (0x80095800, 184 B; provisional): IPA register parameter `effect` in s0, s1/s2 unsaved.
 * Body = the lean form of cloud/work/lean_audio_effect_setup_20261006/group.c (consumed `Effect *effect = input;`
 * view and the owner-field test before binding `owner`, both disclosed there) on the B132 typed source.
 * EQUAL in the whole-program unit next to engine_sound_sync (w21h):
 *   blob_unit --tag w21h score engine_sound_sync audio_effect_setup --with aes.c --with best.c
 *     --internal audio_effect_setup -> EQUAL audio_effect_setup: 46 words (internal, c_aes.c).
 * Its other callers (func_800988D8, func_80098FB8, entity_ai_pathfind) are unmatched asm; it can land only in a
 * group with engine_sound_sync (forced_internal). */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;
typedef struct Link {struct Link *next, *prev;} Link;
typedef struct List {u8 indirect, doubly, pad[2];u32 count;Link *head, *tail;} List;
void func_80091FBC(List *list, Link *object, Link *before);
void func_8009211C(List *list, Link *object);
extern u32 D_80146104;
extern List D_801461E8,D_801461B0,D_80143FF8;
typedef struct Owner { Link link; s8 active,queued; } Owner;
typedef struct Effect { Link link; List *list; u32 handle; s32 kind,mode; s8 state,delay; u8 persistent,priority; u32 rest[9]; Owner *owner; } Effect;
void audio_effect_setup(Effect *input) {
    Effect *effect=input;
    Owner *owner;
    Link *pending;
    effect->handle &= D_80146104;
    effect->kind=0;
    if (effect->mode==1) {
        if (effect->owner) {
            owner=effect->owner;
            pending=&owner->link;
            if (owner->queued) {
                func_8009211C(&D_801461E8,&owner->link);
                ((Owner *)pending)->queued=0;
            }
            func_80091FBC(&D_801461B0,pending,D_801461B0.head);
            ((Owner *)pending)->active=1;
            effect->owner=0;
        }
    }
    if (!effect->persistent) {
        func_8009211C(effect->list,&effect->link);
        func_80091FBC(&D_80143FF8,&effect->link,D_80143FF8.head);
        effect->list=&D_80143FF8;
    }
}
