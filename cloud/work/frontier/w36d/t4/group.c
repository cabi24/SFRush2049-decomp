typedef int s32; typedef unsigned int u32;
extern void *audio_dma_sync(s32, s32);
extern s32 K;
extern void *raw[7]; extern void *al[7];
extern s32 g;
extern void use(void *);
void *helper(s32 size)
{
    return audio_dma_sync(0, size + 7);
}
void inner(void)
{
    raw[g] = helper(K * 64 + 64);
    al[g] = (void *)(((u32)raw[g] + 7) & ~7);
    use(al[g]);
}
void root(void) { inner(); }
