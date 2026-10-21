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

void root(void)
{
    caller0();
    caller0();
}
