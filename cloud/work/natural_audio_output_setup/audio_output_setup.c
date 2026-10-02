/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef signed char s8;
typedef unsigned char u8;
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
extern OSMesgQueue D_80152770;
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
typedef struct AudioEntry AudioEntry;
struct AudioEntry {
    s32 reserved0;
    AudioEntry *next;
    s32 reserved8;
    unsigned int count;
    s32 reserved10;
    s8 excluded;
};
typedef struct AudioList {
    s32 reserved0, reserved4;
    AudioEntry *head;
} AudioList;
extern AudioList *D_801527C8;
unsigned int audio_output_setup(AudioList *list) {
    unsigned int count;
    AudioEntry *p;
    osRecvMesg(&D_80152770, (OSMesg *)0, 1);
    p = (list ? list : D_801527C8)->head;
    count = 0;
    for (; p; p = p->next) {
        if (!p->excluded) count += p->count;
    }
    osJamMesg(&D_80152770, (OSMesg)0, 0);
    return count;
}
