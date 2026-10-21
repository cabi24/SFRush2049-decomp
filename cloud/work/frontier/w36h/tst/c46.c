typedef float f32;
typedef struct St { int a, b; unsigned flags; } St;
typedef struct Sh { int pad[2]; f32 life; int pad2[20]; St *streak; } Sh;
extern void f(Sh *);
void g1(Sh *shot)
{
    f(shot);
    shot->life = 0.0000001f;
    if (shot->streak)
        shot->streak->flags |= 0x1000;
}
void g2(Sh *shot, int k)
{
    while (k--) {
        f(shot);
        shot->life = 0.0000001f;
        if (shot->streak)
            shot->streak->flags |= 0x1000;
    }
}
