/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_effect_setup is the sole match candidate. The rest is genuine B132
 * context, not new matching claims. A used local effect-pointer view and an
 * owner-field guard before binding owner reproduce the observed lifetimes.
 * This compile-affecting source spelling is disclosed; it adds no input,
 * unused storage, artificial volatile, synthetic caller, or runtime operation.
 */
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
extern s32 D_80149868;
extern f32 D_80152748;
extern s8 D_8011023C;
extern s32 D_80110250,D_8011024C,D_80110278,D_80110280,D_8011027C;
extern void *D_80143CF0[],*D_80143AE8[];
extern s32 *D_80144C58;
extern s32 func_80091BA8(s32);
extern s32 entity_state_check(s32);
extern void scheduler_recv(s32);
extern s32 entity_flags_apply(s32,s32,s32,u8);
extern void engine_sound_sync(void);
extern s32 osRecvMesg(void *,void *,s32);
extern s32 osSendMesg(void *,void *,s32);
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

typedef struct Sequence { Link ll; u32 id; s8 free,active,stop; f32 startTime,timeout; u8 next; s32 ids[4]; s32 subId,replace; u8 priority; s32 sndTag; } Sequence;
typedef struct FxTag { Link ll; List *list; u32 id; s32 state,type; s8 paused,loop; u8 numCmds,priority; s32 wpriority; f32 startTime,vol,lrpan,fbpan,pitch; s32 fxid,subId; u32 sysId; void *data; } FxTag;
s32 func_80095198(f32,f32);
void func_800988D8(void) {
 Sequence *s,*next;
 FxTag *t;
 for(s=(Sequence *)D_80149860.head;s;s=next) {
  next=(Sequence *)s->ll.next;
  if(s->stop || (s->sndTag == -1 && func_80095198(s->startTime,s->timeout))) {
   scheduler_recv(s->sndTag);
   entity_flag_check((s32 *)s);
  } else {
   t=(FxTag *)func_80091BA8(s->sndTag);
   if(t) {
    s32 state;
    state=t->state;
    if(state==2 && !t->numCmds && !entity_state_check(t->sysId))
     audio_effect_setup((Effect *)t);
    else continue;
   }
   while(s->ids[s->next]==-1) {
    s->next++;
    if(s->next>=4) break;
   }
   if(s->next>=4) entity_flag_check((s32 *)s);
   else {
    s->sndTag=entity_flags_apply(s->ids[s->next++],s->subId,s->replace,s->priority);
    ((FxTag *)func_80091BA8(s->sndTag))->data=s;
   }
  }
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
        osSendMesg(D_80142728,0,0);
        for(i=0;i<D_80110250;i++) osSendMesg(D_801427A8,D_80143CF0[i],0);
        for(i=0;i<D_8011024C;i++) osSendMesg(D_801427A8,D_80143AE8[i],0);
    }
    osRecvMesg(D_80142728,0,1);
    camera_transform();
    osSendMesg(D_80142728,0,0);
}
