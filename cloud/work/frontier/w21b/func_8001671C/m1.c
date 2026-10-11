/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003E228;
#pragma pack()
typedef union { RecordFields_8003E228 fields; unsigned int words[2]; } RecordStorage_8003E228;
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern RecordStorage_8003E228 D_8003E228[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_8001671C(u16 id, void *payload)
{
    long main;
    long pos;
    long base;
    long i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count == 0) {
        pos = base = D_8003DA28[main].first = D_8003DA20;
    } else {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && ((RecordFields_8003E228 *)&D_8003E228[base + i])->id < id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            pos = base + i;
            if (id == ((RecordFields_8003E228 *)&D_8003E228[pos])->id) {
                ((RecordFields_8003E228 *)&D_8003E228[pos])->references++;
                func_800145DC();
                return 0;
            }
        } else {
            pos = base + i;
        }
    }
    if (D_8003DA20 < 2048) {
        for (i = 0; i < 512; ++i) {
            if (D_8003DA28[i].first > base) {
                D_8003DA28[i].first++;
            }
        }
        i = D_8003DA20 - 1;
        for (; i >= pos; --i) {
            D_8003E228[i + 1] = D_8003E228[i];
        }
        ((RecordFields_8003E228 *)&D_8003E228[pos])->id = id;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->payload = payload;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->references = 1;
        D_8003DA28[main].count++;
        D_8003DA20++;
        func_800145DC();
        return 1;
    }
    func_800145DC();
    return 0;
}
