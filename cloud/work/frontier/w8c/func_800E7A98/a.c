typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Heap { u32 pad0; struct Heap *next; } Heap;

extern char D_80152770[];
extern Heap *D_801527C8;
int osRecvMesg(OSMesgQueue *, void **, int);
int osJamMesg(OSMesgQueue *, void *, int);
void audio_reverb_update(u32 address, int tag);

void func_80095CF4(void)
{
    osRecvMesg((OSMesgQueue *)D_80152770, 0, 1);
}

void func_80095CFC(void)
{
    osJamMesg((OSMesgQueue *)D_80152770, 0, 0);
}

void func_800E7A98(Heap *heap)
{
    Heap *p;

    func_80095CF4();
    if (heap == 0) {
        heap = D_801527C8;
    }
    for (p = D_801527C8; p != 0; p = p->next) {
        if (p->next == heap) {
            p->next = heap->next;
            break;
        }
    }
    audio_reverb_update((u32)heap, 1);
    func_80095CFC();
}
