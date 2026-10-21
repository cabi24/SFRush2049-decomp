typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64]; extern volatile short gWait; extern int gObj;
extern void RemoveBlit(Blit *); extern void Other(int); extern void RemoveObj(short);

void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
}
void clear(void)
{
    unload();
    if (gObj != -1) { RemoveObj(gObj); gObj = -1; }
    while (gWait) {}
}

void big(void)
{
    if (gX[0] > 0) Other(0);
    if (gX[1] > 1) Other(1);
    if (gX[2] > 2) Other(2);
    if (gX[3] > 3) Other(3);
    if (gX[4] > 4) Other(4);
    if (gX[5] > 5) Other(5);
    if (gX[6] > 6) Other(6);
    if (gX[7] > 7) Other(7);
    if (gX[8] > 8) Other(8);
    if (gX[9] > 9) Other(9);
    if (gX[3]) clear();
    if (gX[4]) clear();
}

void small(void)
{
    gX[1] = 2;
    clear();
    Other(9);
}

void root(void)
{
    big();
    big();
    small();
    small();
}
