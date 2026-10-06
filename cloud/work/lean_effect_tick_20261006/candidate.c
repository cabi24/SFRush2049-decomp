/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete native twelve-command effect dispatcher; historical label retained.
 * The real thread argument is homed but unused in the native body.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct Effect Effect;
typedef struct List {u8 indirect,doubly,reserved[2];u32 count;Effect *head,*tail;} List;
struct Effect {
    Effect *next,*prev;
    List *list;
    u32 handle;
    s32 kind,mode;
    s8 paused,delayed;
    u8 pending,priorityByte;
    s32 score;
    f32 time,value,stream,priority,route;
    s32 resource,group,voice;
    void *owner;
};
typedef struct Owner {struct Owner *next;u8 reserved[10];s8 stopped;} Owner;
typedef struct Message {
    s16 id;
    u8 type,used;
    union {
        s32 word;
        Effect *effect;
        struct {s32 duplicate;f32 value,stream,priority;Effect *effect;} start;
        struct {f32 value,stream,priority,route;Effect *effect;} values;
        struct {s32 index;s8 option;} sequence;
        struct {f32 value,seconds;s8 first,second;} music;
        s8 signedByte;
    } payload;
} Message;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *queue;} OSScClient;
typedef struct OSSched OSSched;
extern OSSched D_8002E8E8;
extern OSMesgQueue D_80142728,D_801427A8;
extern void *D_80142B70[];
extern List D_80144020;
extern Effect *D_80144C58;
extern Owner *D_80149868;
extern s32 D_80110240;
extern f32 D_80152748;
void osCreateMesgQueue(OSMesgQueue *,void **,s32);
void osScAddClient(OSSched *,OSScClient *,OSMesgQueue *);
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
void func_80098FB8(void);
void func_800979A0(s32,s32);
void func_80096238(); /* Native caller supplies a handle; accepted body ignores it. */
void audio_effect_setup(Effect *);
void audio_pitch_adjust(Effect *);
s32 audio_state_save(s32,f32,f32);
void audio_timing_sync(s32);
void audio_buffer_manage(s32,f32);
void audio_stream_control(s32,f32);
void sound_priority_set(s32,f32);
void audio_bus_route(s32,f32);
void func_80091FBC(List *,Effect *,Effect *);
void func_8009211C(List *,Effect *);
void func_800958B8(void);
void music_track_control(f32,u16,s32,s32);
void gfx_setup_e700(s32);

void entity_ai_pathfind(void *threadArgument)
{
    OSScClient client;
    Message *message;
    Effect *effect,*other,*next;
    Owner *owner;
    s32 duplicateMode,kind,voice;
    f32 value;

    osCreateMesgQueue(&D_801427A8,D_80142B70,128);
    osScAddClient(&D_8002E8E8,&client,&D_801427A8);
    for (;;) {
        osRecvMesg(&D_801427A8,(void **)&message,1);
        if (message->id==1) {
            func_80098FB8();
            continue;
        }
        osRecvMesg(&D_80142728,0,1);
        switch (message->type) {
        case 0:
            if (D_80110240!=message->payload.sequence.index && D_80110240<0) {
                D_80110240=message->payload.sequence.index;
                func_800979A0(message->payload.sequence.index,message->payload.sequence.option);
            }
            break;
        case 1:
            if (D_80110240>=0) {
                func_80096238(D_80110240);
                D_80110240=-1;
            }
            break;
        case 2:
            message->payload.start.effect->pending--;
            effect=message->payload.start.effect;
            if (effect->kind==0) {
                audio_effect_setup(effect);
                break;
            }
            effect->value=message->payload.start.value;
            message->payload.start.effect->stream=message->payload.start.stream;
            message->payload.start.effect->priority=0.0f;
            message->payload.start.effect->route=1.0f;
            duplicateMode=message->payload.start.duplicate;
            if (duplicateMode) {
                other=D_80144020.head;
                if (other) {
                    effect=message->payload.start.effect;
                    do {
                        if (other!=effect && other->kind!=0 && effect->resource==other->resource &&
                            effect->group==other->group && effect->mode==other->mode) break;
                        other=other->next;
                    } while (other);
                }
                if (!other) {
                    other=D_80144C58;
                    if (other) {
                        effect=message->payload.start.effect;
                        do {
                            if (other!=effect && other->kind==1 && effect->resource==other->resource &&
                                effect->group==other->group && effect->mode==other->mode) break;
                            other=other->next;
                        } while (other);
                    }
                }
                if (other) {
                    kind=other->kind;
                    if (kind==2 || kind==1) {
                        if (duplicateMode==2) {
                            audio_effect_setup(message->payload.start.effect);
                            if (other->kind==1) other->time=-1.0f;
                            break;
                        }
                        if (kind==2) audio_timing_sync(other->voice);
                    }
                    audio_effect_setup(other);
                }
            }
            value=message->payload.start.priority;
            if (value!=-2.0f) message->payload.start.effect->priority=value;
            else message->payload.start.effect->priority=0.0f;
            message->payload.start.effect->kind=3;
            break;
        case 3:
            message->payload.effect->pending--;
            effect=message->payload.effect;
            if (effect->kind==0) {
                audio_effect_setup(effect);
                break;
            }
            voice=audio_state_save(effect->resource,effect->value,effect->stream);
            message->payload.effect->voice=voice;
            effect=message->payload.effect;
            if (effect->voice!=-1) {
                func_8009211C(effect->list,effect);
                func_80091FBC(&D_80144020,effect,D_80144020.head);
                effect->list=&D_80144020;
                effect=message->payload.effect;
                value=effect->priority;
                if (value!=0.0f) sound_priority_set(effect->voice,value);
                value=message->payload.effect->route;
                if (value!=1.0f) audio_bus_route(message->payload.effect->voice,value);
                message->payload.effect->kind=2;
                message->payload.effect->time=D_80152748;
            }
            break;
        case 4:
            message->payload.values.effect->pending--;
            effect=message->payload.values.effect;
            kind=effect->kind;
            if (kind==0) {
                audio_effect_setup(effect);
                break;
            }
            value=message->payload.values.value;
            if (value!=-2.0f) {
                if (kind==2) audio_buffer_manage(effect->voice,value);
                message->payload.values.effect->value=message->payload.values.value;
            }
            value=message->payload.values.stream;
            if (value!=-2.0f) {
                effect=message->payload.values.effect;
                if (effect->kind==2) audio_stream_control(effect->voice,value);
                message->payload.values.effect->stream=message->payload.values.stream;
            }
            value=message->payload.values.priority;
            if (value!=-2.0f) {
                effect=message->payload.values.effect;
                if (effect->kind==2) sound_priority_set(effect->voice,value);
                message->payload.values.effect->priority=message->payload.values.priority;
            }
            value=message->payload.values.route;
            if (value!=-2.0f) {
                effect=message->payload.values.effect;
                if (effect->kind==2 && !effect->paused) audio_bus_route(effect->voice,value);
                message->payload.values.effect->route=message->payload.values.route;
            }
            break;
        case 5:
            message->payload.effect->pending--;
            effect=message->payload.effect;
            if (effect->kind==0) {
                audio_effect_setup(effect);
                break;
            }
            audio_timing_sync(effect->voice);
            effect=message->payload.effect;
            if (effect->delayed) audio_pitch_adjust(effect);
            else audio_effect_setup(effect);
            break;
        case 6:
            message->payload.effect->pending--;
            effect=message->payload.effect;
            kind=effect->kind;
            if (kind==0) audio_effect_setup(effect);
            else {
                if (kind==2) audio_timing_sync(effect->voice);
                audio_effect_setup(message->payload.effect);
            }
            break;
        case 7:
            owner=D_80149868;
            while (owner) {owner->stopped=1;owner=owner->next;}
            func_800958B8();
            other=D_80144020.head;
            while (other) {next=other->next;audio_effect_setup(other);other=next;}
            other=D_80144C58;
            while (other) {next=other->next;audio_effect_setup(other);other=next;}
            break;
        case 8:
            other=D_80144020.head;
            while (other) {
                next=other->next;
                if (other->kind==2) audio_bus_route(other->voice,0.0f);
                other->paused=1;
                other=next;
            }
            other=D_80144C58;
            while (other) {next=other->next;other->paused=1;other=next;}
            break;
        case 9:
            other=D_80144020.head;
            while (other) {
                next=other->next;
                if (other->paused && other->kind==2) {
                    other->paused=0;
                    audio_bus_route(other->voice,other->route);
                }
                other=next;
            }
            other=D_80144C58;
            while (other) {next=other->next;other->paused=0;other=next;}
            break;
        case 10:
            music_track_control(message->payload.music.value,(u32)(message->payload.music.seconds*1000.0f),
                                message->payload.music.first,message->payload.music.second);
            break;
        case 11:
            gfx_setup_e700(message->payload.signedByte);
            break;
        }
        message->used=0;
        osJamMesg(&D_80142728,0,0);
    }
}
