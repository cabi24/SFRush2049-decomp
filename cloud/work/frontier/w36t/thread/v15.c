/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Audio manager thread (entity_ai_pathfind) and _PeriodicMaintenance (func_80098FB8).
 * Provenance: MAT3 GameSource/src/audiomgr.c _ProcessCommand (GC 0x800A1F38),
 * _PeriodicMaintenance (GC 0x800A1EA4), _ProcessPriorities (GC 0x800A3208),
 * _PlayFxTag (GC 0x800A2740), _StopAllSequences (GC 0x800A44B0), _InsertSorted
 * (GC 0x800A3028); bstree.c ClearBST/InsertBSTE/WalkBST. The N64 build runs the
 * commands on a thread fed by a message queue, under a one-message mutex queue. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { s32 pad[6]; } OSMesgQueue;
typedef struct OSScClient { struct OSScClient *next; OSMesgQueue *msgQ; } OSScClient;
typedef struct OSSched OSSched;

typedef struct tDLLE { void *next; void *prev; } tDLLE;
typedef struct tDLList { char handle; char dbl; int numElements; void *head; void *tail; } tDLList;
typedef struct tOrderedList { int (*OrderFunc)(void *, void *); long numElements; void *root; } tOrderedList;
typedef struct tBSTE { void *left; void *right; } tBSTE;

typedef struct tFxSequence {
    tDLLE ll;
    tDLList *list;
    unsigned int id;
    int state;
    int type;
    signed char paused;
    signed char loop;
    unsigned char numCmds;
    unsigned char priority;
    int wpriority;
    float startTime;
    float vol;
    float lrpan;
    float fbpan;
    float pitch;
    int fxid;
    int subId;
    unsigned int sysId;
    void *data;
} tFxSequence;

typedef struct anon_te_tag_c { tBSTE te; tFxSequence *tag; } anon_te_tag_c;

typedef struct anon_ll_id_3c {
    tDLLE ll;
    unsigned int id;
    signed char free;
    signed char active;
    signed char stop;
    float startTime;
    float timeout;
    unsigned char next;
    int ids[4];
    int subId;
    int replace;
    unsigned char priority;
    int sndTag;
} anon_ll_id_3c;

typedef struct { signed short type; char cmd; signed char used; } anon_type_cmd_4;
typedef struct { anon_type_cmd_4 hdr; int song; signed char loop; } anon_hdr_song_c;
typedef struct { anon_type_cmd_4 hdr; int replace; float vol; float lrpan; float fbpan; tFxSequence *tag; } anon_hdr_replace_18;
typedef struct { anon_type_cmd_4 hdr; tFxSequence *tag; } anon_hdr_tag_8;
typedef struct { anon_type_cmd_4 hdr; float vol; float lrpan; float fbpan; float pitch; tFxSequence *tag; } anon_hdr_vol_18;
typedef struct { anon_type_cmd_4 hdr; float vol; float time; signed char music; signed char sfx; } anon_hdr_vol_10;
typedef struct { anon_type_cmd_4 hdr; signed char mono; } anon_hdr_mono_6;
typedef union tAudioMsg {
    anon_type_cmd_4 hdr;
    anon_hdr_song_c playSong;
    anon_hdr_replace_18 playEffect;
    anon_hdr_tag_8 startEffect;
    anon_hdr_vol_18 changeEffect;
    anon_hdr_tag_8 intEffect;
    anon_hdr_tag_8 stopEffect;
    anon_hdr_vol_10 setMasterVol;
    anon_hdr_mono_6 mono;
} tAudioMsg;

extern OSSched D_8002E8E8;                 /* scheduler */
extern OSMesgQueue D_80142728;             /* audio mutex queue */
extern OSMesgQueue D_801427A8;             /* audio command queue */
extern OSMesg D_80142B70[];                /* its 128 messages */
extern signed char D_8011023C;             /* gInited */
extern int D_80110240;                     /* gSong */
extern anon_te_tag_c *D_80110248;          /* gSortedFxTags */
extern int D_8011024C;                     /* gNumStartCmds */
extern int D_80110250;                     /* gNumStopCmds */
extern int D_80110278;                     /* gNumFxPlaying */
extern int D_8011027C;                     /* gNumFxPending */
extern int D_80110280;                     /* gNumFxInterrupted */
extern tAudioMsg *D_80143AE8[];            /* gStartCmds */
extern tAudioMsg *D_80143CF0[];            /* gStopCmds */
extern tOrderedList D_80143F58;            /* gSortedFxTagTree */
extern tDLList D_80144020;                 /* gPlayingFxTags */
extern tDLList D_80144C50;                 /* gInterruptedFxTags */
extern tDLList D_80149860;                 /* gActiveSequences */
extern float D_80152748;                   /* audio clock */

#define gInited D_8011023C
#define gSong D_80110240
#define gSortedFxTags D_80110248
#define gNumStartCmds D_8011024C
#define gNumStopCmds D_80110250
#define gNumFxPlaying D_80110278
#define gNumFxPending D_8011027C
#define gNumFxInterrupted D_80110280
#define gStartCmds D_80143AE8
#define gStopCmds D_80143CF0
#define gSortedFxTagTree D_80143F58
#define gPlayingFxTags D_80144020
#define gInterruptedFxTags D_80144C50
#define gActiveSequences D_80149860

void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
void osScAddClient(OSSched *, OSScClient *, OSMesgQueue *);
s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);

void func_800979A0(int song, int loop);                      /* AudioSongPlay */
void func_80096238(int song);                                /* AudioSongStop */
void audio_effect_setup(tFxSequence *tag);                   /* _FreeFxTag (IPA s0) */
void audio_pitch_adjust(tFxSequence *tag);                   /* _InterruptFxTag (IPA s0) */
unsigned int audio_state_save(int fxid, float vol, float lrpan); /* AudioPlayEffect */
void audio_timing_sync(unsigned int sysId);                  /* AudioStopEffect */
void audio_buffer_manage(unsigned int sysId, float vol);     /* AudioSetEffectVolume */
void audio_stream_control(unsigned int sysId, float lrpan);  /* AudioSetEffectLRPan */
void sound_priority_set(unsigned int sysId, float fbpan);    /* AudioSetEffectFBPan */
void audio_bus_route(unsigned int sysId, float pitch);       /* AudioSetEffectPitch */
void func_800958B8(void);                                    /* AudioStopAllEffects */
void music_track_control(float vol, unsigned int time, int music, int sfx); /* AudioSetMasterVolume */
void gfx_setup_e700(int mono);                               /* AudioSetMono */
void func_8009211C(tDLList *list, void *el);                 /* RemoveLLE */
void func_80091FBC(tDLList *list, void *el, void *before);   /* InsertLLE */
s32 entity_state_check(unsigned int sysId);                  /* AudioEffectPlaying */
int func_80098710(tFxSequence *tag);                         /* _WeightedPriority */
void camera_update_b(tOrderedList *tree, void *root, void *element);  /* _InsertBSTE */
void camera_update_a(tOrderedList *tree, void *root, int *num, void (*WalkFunc)(void *, int)); /* _WalkBST */
void engine_sound_sync(void);                                /* _ProcessEmitters */
void func_800988D8(void);                                    /* _ProcessSequences */
void camera_transform(void);                                 /* AudioPeriodicMaintenance */
void sound_position_update(void *element, int elNum);        /* _UpdateFx */

static void ClearBST(tOrderedList *tree)
{
    tree->root = 0;
    tree->numElements = 0;
}

static void InsertBSTE(tOrderedList *tree, void *element)
{
    camera_update_b(tree, &tree->root, element);
}

static void WalkBST(tOrderedList *tree, void (*WalkFunc)(void *, int))
{
    int num = 0;

    camera_update_a(tree, tree->root, &num, WalkFunc);
}

static void _PlayFxTag(tFxSequence *tag)
{
    func_8009211C(tag->list, tag);
    func_80091FBC(&gPlayingFxTags, tag, gPlayingFxTags.head);
    tag->list = &gPlayingFxTags;
}

static void _FreeCmd(tAudioMsg *cmd) { cmd->hdr.used = 0; }
static float GetAudioTime(void) { return D_80152748; }
static void _LockAudio(void) { osRecvMesg(&D_80142728, 0, 1); }
static void _UnlockAudio(void) { osSendMesg(&D_80142728, 0, 0); }
static void _StopAllSequences(void)
{
    anon_ll_id_3c *s;
    for (s = gActiveSequences.head; s; s = s->ll.next) s->stop = 1;
}

static void _InsertSorted(tFxSequence *tag)
{
    anon_te_tag_c *stag;
    stag = &gSortedFxTags[gSortedFxTagTree.numElements];
    stag->tag = tag;
    InsertBSTE(&gSortedFxTagTree, stag);
}

static void _SendCmd(tAudioMsg *cmd) { osSendMesg(&D_801427A8, cmd, 0); }

static void _ProcessPriorities(void)
{
    tFxSequence *tag, *next;
    int i;
    osRecvMesg(&D_80142728, 0, 1);
    ClearBST(&gSortedFxTagTree);
    gNumStopCmds = 0;
    gNumStartCmds = 0;
    for (tag = gPlayingFxTags.head; tag; tag = next) {
        next = tag->ll.next;
        if (!entity_state_check(tag->sysId)) {
            if (tag->loop) audio_pitch_adjust(tag);
            else audio_effect_setup(tag);
        } else {
            tag->wpriority = func_80098710(tag);
            _InsertSorted(tag);
        }
    }
    gNumFxPlaying = gPlayingFxTags.numElements;
    gNumFxPending = gNumFxInterrupted = 0;
    for (tag = gInterruptedFxTags.head; tag; tag = next) {
        next = tag->ll.next;
        if (!tag->loop) {
            tag->startTime -= 1.0f;
            if (tag->startTime < -10.0f) {
                audio_effect_setup(tag);
                continue;
            }
        }
        if (tag->state == 1) gNumFxPending++;
        else gNumFxInterrupted++;
        tag->wpriority = func_80098710(tag);
        _InsertSorted(tag);
    }
    WalkBST(&gSortedFxTagTree, sound_position_update);
    osSendMesg(&D_80142728, 0, 0);
    for (i = 0; i < gNumStopCmds; i++) _SendCmd(gStopCmds[i]);
    for (i = 0; i < gNumStartCmds; i++) _SendCmd(gStartCmds[i]);
}

void func_80098FB8(void)
{
    if (gInited) {
        engine_sound_sync();
        func_800988D8();
        _ProcessPriorities();
    }
    osRecvMesg(&D_80142728, 0, 1);
    camera_transform();
    osSendMesg(&D_80142728, 0, 0);
}

static void _ProcessCommand(tAudioMsg *cmd)
{
    tFxSequence *tag;
    tFxSequence *next;
    _LockAudio();
    switch (cmd->hdr.cmd) {
    case 0:
        if (gSong != cmd->playSong.song && gSong < 0) {
            gSong = cmd->playSong.song;
            func_800979A0(gSong, cmd->playSong.loop);
        }
        break;
    case 1:
        if (gSong >= 0) {
            func_80096238(gSong);
            gSong = -1;
        }
        break;
    case 2:
        cmd->playEffect.tag->numCmds--;
        if (cmd->playEffect.tag->state == 0) {
            audio_effect_setup(cmd->playEffect.tag);
            break;
        }
        cmd->playEffect.tag->vol = cmd->playEffect.vol;
        cmd->playEffect.tag->lrpan = cmd->playEffect.lrpan;
        cmd->playEffect.tag->fbpan = 0.0f;
        cmd->playEffect.tag->pitch = 1.0f;
        if (cmd->playEffect.replace != 0) {
            for (tag = gPlayingFxTags.head; tag; tag = tag->ll.next) {
                if (tag != cmd->playEffect.tag && tag->state != 0
                    && tag->fxid == cmd->playEffect.tag->fxid
                    && tag->subId == cmd->playEffect.tag->subId
                    && tag->type == cmd->playEffect.tag->type) break;
            }
            if (!tag) {
                for (tag = gInterruptedFxTags.head; tag; tag = tag->ll.next) {
                    if (tag != cmd->playEffect.tag && tag->state == 1
                        && tag->fxid == cmd->playEffect.tag->fxid
                        && tag->subId == cmd->playEffect.tag->subId
                        && tag->type == cmd->playEffect.tag->type) break;
                }
            }
            if (tag) {
                if (tag->state == 2 || tag->state == 1) {
                    if (cmd->playEffect.replace == 2) {
                        audio_effect_setup(cmd->playEffect.tag);
                        if (tag->state == 1) tag->startTime = -1.0f;
                        break;
                    } else if (tag->state == 2) audio_timing_sync(tag->sysId);
                }
                audio_effect_setup(tag);
            }
        }
        if (cmd->playEffect.fbpan != -2.0f)
            cmd->playEffect.tag->fbpan = cmd->playEffect.fbpan;
        else cmd->playEffect.tag->fbpan = 0.0f;
        cmd->playEffect.tag->state = 3;
        break;
    case 3:
        cmd->startEffect.tag->numCmds--;
        if (cmd->startEffect.tag->state == 0) {
            audio_effect_setup(cmd->startEffect.tag);
            break;
        }
        cmd->startEffect.tag->sysId = audio_state_save(cmd->startEffect.tag->fxid,
            cmd->startEffect.tag->vol, cmd->startEffect.tag->lrpan);
        if (cmd->startEffect.tag->sysId != -1) {
            _PlayFxTag(cmd->startEffect.tag);
            if (cmd->startEffect.tag->fbpan)
                sound_priority_set(cmd->startEffect.tag->sysId, cmd->startEffect.tag->fbpan);
            if (cmd->startEffect.tag->pitch != 1.0f)
                audio_bus_route(cmd->startEffect.tag->sysId, cmd->startEffect.tag->pitch);
            cmd->startEffect.tag->state = 2;
            cmd->startEffect.tag->startTime = GetAudioTime();
        }
        break;
    case 4:
        cmd->changeEffect.tag->numCmds--;
        if (cmd->changeEffect.tag->state == 0) {
            audio_effect_setup(cmd->changeEffect.tag);
            break;
        }
        if (cmd->changeEffect.vol != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                audio_buffer_manage(cmd->changeEffect.tag->sysId, cmd->changeEffect.vol);
            cmd->changeEffect.tag->vol = cmd->changeEffect.vol;
        }
        if (cmd->changeEffect.lrpan != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                audio_stream_control(cmd->changeEffect.tag->sysId, cmd->changeEffect.lrpan);
            cmd->changeEffect.tag->lrpan = cmd->changeEffect.lrpan;
        }
        if (cmd->changeEffect.fbpan != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                sound_priority_set(cmd->changeEffect.tag->sysId, cmd->changeEffect.fbpan);
            cmd->changeEffect.tag->fbpan = cmd->changeEffect.fbpan;
        }
        if (cmd->changeEffect.pitch != -2.0f) {
            if (cmd->changeEffect.tag->state == 2 && !cmd->changeEffect.tag->paused)
                audio_bus_route(cmd->changeEffect.tag->sysId, cmd->changeEffect.pitch);
            cmd->changeEffect.tag->pitch = cmd->changeEffect.pitch;
        }
        break;
    case 5:
        cmd->intEffect.tag->numCmds--;
        if (cmd->intEffect.tag->state == 0) {
            audio_effect_setup(cmd->intEffect.tag);
            break;
        }
        audio_timing_sync(cmd->intEffect.tag->sysId);
        if (cmd->intEffect.tag->loop) audio_pitch_adjust(cmd->intEffect.tag);
        else audio_effect_setup(cmd->intEffect.tag);
        break;
    case 6:
        cmd->stopEffect.tag->numCmds--;
        if (cmd->stopEffect.tag->state == 0) {
            audio_effect_setup(cmd->stopEffect.tag);
            break;
        }
        if (cmd->stopEffect.tag->state == 2) audio_timing_sync(cmd->stopEffect.tag->sysId);
        audio_effect_setup(cmd->stopEffect.tag);
        break;
    case 7:
        _StopAllSequences();
        func_800958B8();
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            audio_effect_setup(tag);
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            audio_effect_setup(tag);
        }
        break;
    case 8:
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            if (tag->state == 2) audio_bus_route(tag->sysId, 0.0f);
            tag->paused = 1;
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            tag->paused = 1;
        }
        break;
    case 9:
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            if (tag->paused && tag->state == 2) {
                tag->paused = 0;
                audio_bus_route(tag->sysId, tag->pitch);
            }
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            tag->paused = 0;
        }
        break;
    case 10:
        music_track_control(cmd->setMasterVol.vol, (unsigned int)(1000.0f * cmd->setMasterVol.time),
            cmd->setMasterVol.music, cmd->setMasterVol.sfx);
        break;
    case 11:
        gfx_setup_e700(cmd->mono.mono);
        break;
    }
    _FreeCmd(cmd);
    _UnlockAudio();
}

void entity_ai_pathfind(void *arg)
{
    OSScClient client;
    OSMesg msg;
    s32 type;

    osCreateMesgQueue(&D_801427A8, D_80142B70, 128);
    osScAddClient(&D_8002E8E8, &client, &D_801427A8);
    for (;;) {
        osRecvMesg(&D_801427A8, &msg, 1);
        type = ((tAudioMsg *)msg)->hdr.type;
        if (type == 1) {
            func_80098FB8();
        } else {
            _ProcessCommand(msg);
        }
    }
}
