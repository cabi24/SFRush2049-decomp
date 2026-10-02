/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned short u16;
typedef struct Track80 {
    unsigned char prefix[28];
    s16 flags;
    s16 timer_a;
    s16 timer_b;
    s16 cells[20];
    unsigned char tail[6];
} Track80;
typedef struct Track812 {
    s16 limit;
    s16 selected_a;
    s16 selected_b;
    s16 selected_c;
    s16 count;
    s16 reserved;
    Track80 tracks[10];
} Track812;
extern Track812 D_80151CE8;
extern void *D_80153F20;
extern void *memcpy(void *,const void *,unsigned int);
void audio_dsp_process(void)
{
    s16 i,j;
    memcpy(&D_80151CE8,D_80153F20,812);
    D_80151CE8.selected_c=-1;
    D_80151CE8.selected_a=D_80151CE8.selected_c;
    D_80151CE8.selected_b=D_80151CE8.selected_a;
    D_80151CE8.limit=90;
    for(i=0;i<D_80151CE8.count;i++) {
        /* The original executes this three-iteration no-write loop. */
        for(j=0;j<3;j++) {}
        if(D_80151CE8.selected_b<0 && (D_80151CE8.tracks[i].flags&1))
            D_80151CE8.selected_b=i;
        if(D_80151CE8.selected_a<0 && (D_80151CE8.tracks[i].flags&4))
            D_80151CE8.selected_a=i;
        if(D_80151CE8.selected_c<0 && (D_80151CE8.tracks[i].flags&2))
            D_80151CE8.selected_c=i;
        for(j=0;j<20;j++) D_80151CE8.tracks[i].cells[j]=0;
        D_80151CE8.tracks[i].timer_b=45;
        D_80151CE8.tracks[i].timer_a=D_80151CE8.tracks[i].timer_b;
    }
    if(D_80151CE8.selected_b<0)D_80151CE8.selected_b=0;
    if(D_80151CE8.selected_a<0)D_80151CE8.selected_a=0;
    if(D_80151CE8.selected_c<0)D_80151CE8.selected_c=0;
}
