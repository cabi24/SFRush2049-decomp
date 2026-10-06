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

static void heap_unlink(Heap *h)
{
    Heap *p;

    for (p = D_801527C8; p != 0; p = p->next) {
        if (p->next == h) {
            p->next = h->next;
            return;
        }
    }
}

void func_800E7A98(Heap *heap)
{
    func_80095CF4();
    if (heap == 0) {
        heap = D_801527C8;
    }
    heap_unlink(heap);
    audio_reverb_update((u32)heap, 1);
    func_80095CFC();
}
