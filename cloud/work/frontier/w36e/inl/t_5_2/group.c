typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait;
extern void RemoveBlit(Blit *); extern void Other(int);

void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
}

void caller0(void)
{
    gX[0] += gX[0] * 1;
    gX[7] += gX[3] * 2;
    unload();
    Other(0);
}

void caller1(void)
{
    gX[1] += gX[1] * 1;
    gX[8] += gX[4] * 2;
    unload();
    Other(1);
}

void caller2(void)
{
    gX[2] += gX[2] * 1;
    gX[9] += gX[5] * 2;
    unload();
    Other(2);
}

void caller3(void)
{
    gX[3] += gX[3] * 1;
    gX[10] += gX[6] * 2;
    unload();
    Other(3);
}

void caller4(void)
{
    gX[4] += gX[4] * 1;
    gX[11] += gX[7] * 2;
    unload();
    Other(4);
}

void root(void)
{
    caller0();
    caller0();
    caller1();
    caller1();
    caller2();
    caller2();
    caller3();
    caller3();
    caller4();
    caller4();
}
