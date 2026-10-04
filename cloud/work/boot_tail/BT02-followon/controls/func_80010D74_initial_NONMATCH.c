/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* REJECTED INITIAL NONMATCH: wrong message API and scalar byte home; never submit. */
typedef struct OSMesgQueue_s OSMesgQueue;
typedef void *OSMesg;
extern int osSendMesg(OSMesgQueue *, OSMesg, int);
extern unsigned char D_800381F8[];
extern void (*D_8003801C)(void *);
extern void *D_80038228;
extern void *D_800381F0;
void func_80010D74(void)
{
    unsigned char command = 255;
    osSendMesg((OSMesgQueue *)D_800381F8, &command, 1);
    D_8003801C(D_80038228);
    D_8003801C(D_800381F0);
}
