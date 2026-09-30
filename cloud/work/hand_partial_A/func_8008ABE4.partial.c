/* 11/36 words differ; needs head typedefs:
   typedef struct { u32 a; u32 b; u32 c; } DmaEnt8ABE4;
   typedef struct { u16 pad; u16 count; u16 idx; u16 pad2; DmaEnt8ABE4 *ents; } DmaQ8ABE4;
   extern DmaQ8ABE4 D_80153F10;   (replaces extern s32 D_80153F10;)
   remaining diff: temp register numbering (target wraps t9->t6). */
s32 func_8008ABE4(void) { DmaEnt8ABE4 *e; if (D_80153F10.idx < D_80153F10.count) { D_80153F10.idx += 1; e = (DmaEnt8ABE4 *)((s32) D_80153F10.ents + D_80153F10.idx * 0xC); __osPiRawStartDma(&D_80161438, 1, 0, e[-1].b, (void *) e[-1].c, e[-1].a, (OSMesgQueue *) &D_80153E68); return 1; } return 0; }
