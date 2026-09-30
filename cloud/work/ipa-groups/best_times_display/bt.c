typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

typedef struct { float a; s32 pad[3]; float b; s32 c; } Ent;      /* 24 bytes */
typedef struct { Ent e[5]; } Slot;                                 /* 120 bytes */

extern Slot D_80140808[];
extern s16 D_80140A08[];
extern s32 D_80140AE0[];
extern float D_80140B10[];
extern float D_80140BE0[];
extern float D_80142518[];
extern void scheduler_recv(s32 h);

void best_times_display(s16 idx)
{
    Slot *s = &D_80140808[idx];
    s32 i;
    s->e[0].c = 0;
    s->e[0].a = 0.0f;
    s->e[0].b = 0.0f;
    i = 1;
    do {
        s->e[i].c = 0;
        s->e[i].a = 0.0f;
        s->e[i].b = 0.0f;
        i++;
    } while (i < 5);
    scheduler_recv(D_80140AE0[idx]);
    D_80140AE0[idx] = -1;
    D_80140A08[idx] = 0;
    D_80140B10[idx] = 0.0f;
    D_80140BE0[idx] = 0.0f;
    D_80142518[idx] = 0.0f;
}

void caller_a(s16 i, s32 x) { if (x) best_times_display(i); }
void caller_b(s16 i, s32 x) { if (x) best_times_display(i); else scheduler_recv(x); }
