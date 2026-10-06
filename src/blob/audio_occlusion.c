/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_occlusion (historical label) -- controller-state reset.
 * Under the pad message-queue lock (osRecvMesg(&D_801497A8, NULL, OS_MESG_BLOCK) ...
 * osJamMesg(..., NULL, OS_MESG_NOBLOCK)) it clears the three combined pad masks
 * and, for each of the four pads, the stick vector and the four per-pad masks.
 * It is the same block controller_poll runs in its skip-frames branch.
 * No arcade ancestor (N64 SI controller code).
 *
 * Shaping (both visible in the code, see cloud/work/frontier/w8b/RESULTS.md):
 *  - the three combined masks are one chained assignment; the chain is what
 *    puts &D_80156944 / &D_80149784 in v0/v1 (separate statements give lui at).
 *  - the stick vector array is *defined* (tentative/common definition) in this
 *    translation unit: as1 then shares one `lui at` between its two swc1 stores
 *    per pad; with an extern declaration every store gets its own lui.
 */
typedef float f32;
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;

extern OSMesgQueue D_801497A8;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osJamMesg(OSMesgQueue *, void *, int);

extern s32 D_80156944;      /* all pads: held mask */
extern s32 D_80149784;      /* all pads: repeat-pressed mask */
extern s32 D_8015694C;      /* all pads: pressed mask */
f32 D_80156958[4][2];       /* per pad stick x, y (defined in this TU) */
extern s32 D_80149B10[4];   /* per pad pressed latch */
extern s32 D_80143A00[4];   /* per pad repeat-pressed mask */
extern s32 D_80156978[4];   /* per pad held mask */
extern s32 D_80156998[4];   /* per pad pressed mask */

void audio_occlusion(void)
{
    s32 i;

    osRecvMesg(&D_801497A8, 0, 1);
    D_8015694C = D_80149784 = D_80156944 = 0;
    for (i = 0; i < 4; i++) {
        D_80156958[i][0] = D_80156958[i][1] = 0.0f;
        D_80149B10[i] = 0;
        D_80143A00[i] = 0;
        D_80156978[i] = 0;
        D_80156998[i] = 0;
    }
    osJamMesg(&D_801497A8, 0, 0);
}
