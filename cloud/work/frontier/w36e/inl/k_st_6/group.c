typedef struct Blit Blit;
extern Blit *gA, *gB; extern int gX[64];
extern void RemoveBlit(Blit *); extern void Other(int);
void unload(void)
{
    if (gA) { RemoveBlit(gA); gA = 0; }
    if (gB) { RemoveBlit(gB); gB = 0; }
    gX[0] = 0;
    gX[1] = 1;
    gX[2] = 2;
    gX[3] = 3;
    gX[4] = 4;
    gX[5] = 5;
}
void c1(void) { int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(1); }
void c2(void) { int i; for (i = 0; i < 9; i++) Other(i); unload(); Other(2); }
void root(void) { c1(); c1(); c2(); c2(); }
