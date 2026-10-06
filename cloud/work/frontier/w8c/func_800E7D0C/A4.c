typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct Heap {
    u32 magic;
    struct Heap *next;
    void *first;
    void *last;
    u32 end;
} Heap;

extern Heap *D_801527C8;
extern char D_80152770[];
extern u8 D_8017A640[];
extern int D_801527A0;
extern volatile s8 D_80116488;
extern s8 D_80156994;
extern u32 D_80000318;
void osCreateMesgQueue(void *, void *, int);
int osJamMesg(void *, void *, int);
void func_800E7B44(Heap *heap, u16 count, u16 a);

void func_800E7D0C(u16 count, u16 a)
{
    Heap *h;

    if (D_80116488 == 0) {
        D_80116488 = 1;
        osCreateMesgQueue(D_80152770, &D_801527A0, 1);
        osJamMesg(D_80152770, 0, 0);
    }
    D_801527C8 = (Heap *)(((u32)D_8017A640 + 31) & ~31);
    D_801527C8->end = D_80000318 | 0x80000000;
    if (D_801527C8->end >= 0x80400001) {
        D_80156994 = 1;
    }
    func_800E7B44(D_801527C8, count, a);
}
