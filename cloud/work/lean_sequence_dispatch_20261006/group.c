/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Sole research target: func_800988D8. Real B132 caller/list/cleanup context. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Link {struct Link *next, *prev;} Link;
typedef struct List {u8 indirect, doubly, pad[2];u32 count;Link *head, *tail;} List;
void func_80091FBC(List *list, Link *object, Link *before) {
    Link *node, *at, *current, *header, *next;
    if (object == 0) return;
    if (list->indirect) {
        node = object->next;
        if (before) at = before->next;
        else at = 0;
    } else {
        node = object;
        at = before;
    }
    if (before) {
        if (list->doubly) {
            if (at->prev) {
                if (list->indirect) at->prev->next->next = object;
                else at->prev->next = object;
            } else list->head = object;
            node->prev = at->prev;
            at->prev = object;
        } else if (before == list->head) list->head = object;
        else if (list->indirect) {
            current = list->head;
            while (current) {
                header = current->next;
                next = header->next;
                if (before == next) {
                    header->next = object;
                    next = current->next->next;
                }
                current = next;
            }
        } else {
            current = list->head;
            while (current) {
                header = current->next;
                if (before == header) {
                    current->next = object;
                    header = object;
                }
                current = header;
            }
        }
    } else {
        if (list->doubly) node->prev = list->tail;
        if (list->tail) {
            if (list->indirect) list->tail->next->next = object;
            else list->tail->next = object;
        } else list->head = object;
        list->tail = object;
    }
    node->next = before;
    list->count++;
}
void func_8009211C(List *list, Link *object) {
    Link *node, *current, *header, *next;
    if (object == 0) return;
    if (list->indirect) node = object->next;
    else node = object;
    if (list->doubly) {
        if (node->next) {
            if (list->indirect) node->next->next->prev = node->prev;
            else node->next->prev = node->prev;
        } else list->tail = node->prev;
        if (node->prev) {
            if (list->indirect) node->prev->next->next = node->next;
            else node->prev->next = node->next;
        } else list->head = node->next;
    } else if (object == list->head) {
        list->head = node->next;
        if (list->head == 0) list->tail = 0;
    } else if (list->indirect) {
        current = list->head;
        while (current) {
            header = current->next;
            next = header->next;
            if (object == next) {
                header->next = node->next;
                if (node->next == 0) list->tail = current;
                break;
            }
            current = next;
        }
    } else {
        current = list->head;
        while (current) {
            header = current->next;
            if (object == header) {
                current->next = node->next;
                if (node->next == 0) list->tail = current;
                break;
            }
            current = header;
        }
    }
    list->count--;
}

typedef int s32;
typedef signed char s8;
typedef float f32;
#define M2C_FIELD(expr,type_ptr,offset) (*(type_ptr)((s8 *)(expr)+(offset)))
extern u32 D_80146104;
extern List D_80149860,D_80149808,D_801461E8,D_801461B0,D_80143FF8,D_80144C50,D_80144020;

extern f32 D_80152748;
extern s8 D_8011023C;
extern s32 D_80110250,D_8011024C,D_80110278,D_80110280,D_8011027C;
extern void *D_80143CF0[],*D_80143AE8[];
extern s32 *D_80144C58;
struct Effect;
extern struct Effect *func_80091BA8(s32);
extern s32 entity_state_check(s32);
extern void scheduler_recv(s32);
extern s32 entity_flags_apply(s32,s32,s32,u8);
extern void engine_sound_sync(void);
extern s32 osRecvMesg(void *,void *,s32);
extern s32 osJamMesg(void *,void *,s32);
extern u8 D_80142728[],D_801427A8[];
typedef struct TreeNode { struct TreeNode *left,*right; s32 *object; } TreeNode;
typedef struct Tree { s32 (*compare)(TreeNode *,TreeNode *); s32 count; TreeNode *root; } Tree;
extern Tree D_80143F58;
extern TreeNode *D_80110248;
extern s32 func_80098710(s32 *);
extern void camera_update_b(Tree *,TreeNode **,TreeNode *);
extern void camera_update_a(Tree *,TreeNode *,s32 *,void (*)(TreeNode *,s32));
extern void func_800952E0(TreeNode *,s32);
extern void camera_transform(void);
void entity_flag_check(s32 *);
typedef struct Owner { Link link; s8 active,queued; } Owner;
typedef struct Effect { Link link; List *list; u32 handle; s32 kind,mode; s8 state,delay; u8 persistent,priority; u32 rest[9]; Owner *owner; } Effect;
void audio_effect_setup(Effect *);
void audio_pitch_adjust(s32 *);
void func_800988D8(void);
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
void audio_pitch_adjust(s32 *effect) {
    M2C_FIELD(effect,s32 *,16)=3;
    func_8009211C(M2C_FIELD(effect,List **,8),(Link *)effect);
    func_80091FBC(&D_80144C50,(Link *)effect,D_80144C50.head);
    M2C_FIELD(effect,List **,8)=&D_80144C50;
}
void entity_flag_check(s32 *ipa_s0) {
    M2C_FIELD(ipa_s0, s32 *, 8) = (s32) (M2C_FIELD(ipa_s0, s32 *, 8) & D_80146104);
    if (M2C_FIELD(ipa_s0, s8 *, 0xD) != 0) {
        func_8009211C(&D_80149860, ipa_s0);
        M2C_FIELD(ipa_s0, s8 *, 0xD) = 0;
    }
    func_80091FBC(&D_80149808, ipa_s0, M2C_FIELD(&D_80149808, s32 **, 8));
    M2C_FIELD(ipa_s0, s8 *, 0xC) = 1;
}

s32 func_80095198(f32 x,f32 delta) {f32 sum=x+delta;if(D_80152748<x)sum-=14400.0f;return sum<D_80152748;}

typedef struct Sequence {
    struct Sequence *next, *prev;
    u32 handle;
    s8 active, queued, cancel;
    u8 padF;
    f32 start, duration;
    u8 index, pad19[3];
    union {
        s32 window[5];
        struct { s32 effects[4], arg1; } fields;
    } schedule;
    s32 arg2;
    u8 arg3, pad35[3];
    s32 current;
} Sequence;
extern Sequence *D_80149868;

void func_800988D8(void) {
    Sequence *sequence, *next;
    Effect *effect;
    s32 index;
    u8 next_index;
    s32 kind;
    sequence=D_80149868;
    if (sequence) {
        do {
            next=sequence->next;
            if (sequence->cancel || (sequence->current==-1 && func_80095198(sequence->start,sequence->duration))) {
                scheduler_recv(sequence->current);
                entity_flag_check((s32 *)sequence);
                goto advance;
            }
            effect=func_80091BA8(sequence->current);
            if (effect) {
                kind=effect->kind;
                if (kind!=2) goto advance;
                if (effect->persistent) goto advance;
                if (entity_state_check(effect->rest[8])) goto advance;
                audio_effect_setup(effect);
            }
            index=sequence->index;
            while (sequence->schedule.window[index]==-1) {
                next_index=index+1;
                index=next_index;
                sequence->index=next_index;
                if (index>=4) break;
            }
            if (index>=4) {
                entity_flag_check((s32 *)sequence);
            } else {
                sequence->index=index+1;
                sequence->current=entity_flags_apply(sequence->schedule.window[index],sequence->schedule.fields.arg1,sequence->arg2,sequence->arg3);
                (func_80091BA8(sequence->current))->owner=(Owner *)sequence;
            }
advance:
            sequence=next;
        } while (sequence);
    }
}


void func_80098FB8(void) {
    s32 *effect,*next;
    TreeNode *node;
    s32 count,i;
    if (D_8011023C) {
        engine_sound_sync();
        func_800988D8();
        osRecvMesg(D_80142728,0,1);
        D_80143F58.root=0;
        D_80143F58.count=0;
        D_80110250=0;
        D_8011024C=0;
        effect=(s32 *)D_80144020.head;
        while (effect) {
            next=M2C_FIELD(effect,s32 **,0);
            if (!entity_state_check(M2C_FIELD(effect,s32 *,60))) {
                if (M2C_FIELD(effect,s8 *,25)) audio_pitch_adjust(effect);
                else audio_effect_setup(effect);
            } else {
                M2C_FIELD(effect,s32 *,28)=func_80098710(effect);
                node=&D_80110248[D_80143F58.count];
                node->object=effect;
                camera_update_b(&D_80143F58,&D_80143F58.root,node);
            }
            effect=next;
        }
        D_80110278=D_80144020.count;
        D_80110280=0;
        D_8011027C=0;
        effect=D_80144C58;
        while (effect) {
            next=M2C_FIELD(effect,s32 **,0);
            if (!M2C_FIELD(effect,s8 *,25)) {
                M2C_FIELD(effect,f32 *,32)-=1.0f;
                if (M2C_FIELD(effect,f32 *,32)<-10.0f) {
                    audio_effect_setup(effect);
                    effect=next;
                    continue;
                }
            }
            if (M2C_FIELD(effect,s32 *,16)==1) D_8011027C++;
            else D_80110280++;
            M2C_FIELD(effect,s32 *,28)=func_80098710(effect);
            node=&D_80110248[D_80143F58.count];
            node->object=effect;
            camera_update_b(&D_80143F58,&D_80143F58.root,node);
            effect=next;
        }
        count=0;
        camera_update_a(&D_80143F58,D_80143F58.root,&count,func_800952E0);
        osJamMesg(D_80142728,0,0);
        for(i=0;i<D_80110250;i++) osJamMesg(D_801427A8,D_80143CF0[i],0);
        for(i=0;i<D_8011024C;i++) osJamMesg(D_801427A8,D_80143AE8[i],0);
    }
    osRecvMesg(D_80142728,0,1);
    camera_transform();
    osJamMesg(D_80142728,0,0);
}
