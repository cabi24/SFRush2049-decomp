/* Host fixture includes the unchanged complete source. Unused context is linker-GC'd. */
#include <string.h>
#include "group.c"
Section D_80151CE8[5];
Track D_8012E5E8[4];
f32 D_8012443C;
static TrackPt points[4][64];
int check_case(const s16 *pos, s16 prev, s16 point, s16 row, s16 cur,
               const s16 *starts, const u16 *counts, const s16 *coordinates,
               s16 last, s16 sections, unsigned int literal) {
    int i,j,result;
    Section before_sections[5];
    TrackPt before_points[4][64];
    Track before_tracks[4];
    memset(D_80151CE8,0xa5,sizeof(D_80151CE8));
    memset(points,0xa5,sizeof(points));
    D_80151CE8[0].last=last;
    D_80151CE8[0].count=sections;
    for(i=0;i<5;i++)for(j=0;j<16;j++)D_80151CE8[i].start[j]=starts[i*16+j];
    for(i=0;i<4;i++) {
        D_8012E5E8[i].numPoints=counts[i];
        D_8012E5E8[i].pad2=0xa5a5;
        D_8012E5E8[i].points=points[i];
        for(j=0;j<64;j++) {
            points[i][j].x=coordinates[(i*64+j)*3];
            points[i][j].y=coordinates[(i*64+j)*3+1];
            points[i][j].z=coordinates[(i*64+j)*3+2];
        }
    }
    memcpy(&D_8012443C,&literal,4);
    memcpy(before_sections,D_80151CE8,sizeof(before_sections));
    memcpy(before_points,points,sizeof(points));
    memcpy(before_tracks,D_8012E5E8,sizeof(before_tracks));
    result=func_800E4300((s16 *)pos,prev,point,row,cur);
    if(memcmp(before_sections,D_80151CE8,sizeof(before_sections)) ||
       memcmp(before_points,points,sizeof(points)) ||
       memcmp(before_tracks,D_8012E5E8,sizeof(before_tracks)))return -99999;
    return result;
}
