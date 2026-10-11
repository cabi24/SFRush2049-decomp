/* context: provisional audio_effect_setup from cloud/work/ipa-groups/codex_effect_typed_b132/group.c (frozen nonmatch) */
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
void audio_effect_setup(Effect *effect) {
    Owner *owner;
    Link *pending;
    effect->handle &= D_80146104;
    effect->kind=0;
    if (effect->mode==1) {
        owner=effect->owner;
        if (owner) {
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
