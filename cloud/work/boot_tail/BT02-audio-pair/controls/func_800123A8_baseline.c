/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioCacheNode {
    struct AudioCacheNode *next;
    struct AudioCacheNode *previous;
    unsigned int unknown08;
    unsigned int unknown0C;
    unsigned int unknown10;
    unsigned char *buffer;
} AudioCacheNode;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void func_800084E0(void *, int);
extern unsigned char *D_8003834C;
extern AudioCacheNode *D_80038350;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
void func_800123A8(unsigned short count)
{
    int i;
    int bytes;
    bytes = count * 1536;
    D_8003834C = D_80038018(bytes, 0);
    func_800084E0(D_8003834C, bytes);
    D_80038350 = D_80038018(count * sizeof(AudioCacheNode), 0);
    D_80038354 = 0;
    D_80038358 = 0;
    D_8003835C = D_80038350;
    D_80038350[0].previous = 0;
    D_80038350[0].buffer = D_8003834C;
    for (i = 1; i < count; i++) {
        D_80038350[i - 1].next = &D_80038350[i];
        D_80038350[i].previous = &D_80038350[i - 1];
        D_80038350[i].buffer = &D_8003834C[i * 1536];
    }
    D_80038350[i - 1].next = 0;
}
