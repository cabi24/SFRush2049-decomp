#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "candidate.c"
ProximityEnabled D_8014AA3A[8];
ProximityPlayer player_array[8];
typedef char enabled_stride[(sizeof(ProximityEnabled)==0x808)?1:-1];
typedef char player_stride[(sizeof(ProximityPlayer)==0x3B8)?1:-1];
typedef char position_offset[(offsetof(ProximityPlayer,position)==8)?1:-1];
/* data: point[3], player position[3], radius, separate output; bit patterns. */
int run_case(int index, int enabled, uint32_t *data, int alias) {
    float point[3], radius, output, *destination;
    int result; short player_index=(short)index;
    memset(D_8014AA3A,0xA5,sizeof(D_8014AA3A));
    memset(player_array,0xA5,sizeof(player_array));
    D_8014AA3A[index].enabled=(signed char)enabled;
    memcpy(point,data,12); memcpy(player_array[index].position,data+3,12);
    memcpy(&radius,data+6,4); memcpy(&output,data+7,4);
    destination = alias < 0 ? 0 : alias < 3 ? &point[alias] :
        alias < 6 ? &player_array[index].position[alias-3] :
        alias == 6 ? &radius : &output;
    result=TARGET(&player_index,point,&radius,destination);
    assert(player_index==index);
    memcpy(data,point,12); memcpy(data+3,player_array[index].position,12);
    memcpy(data+6,&radius,4); memcpy(data+7,&output,4);
    {
        unsigned i,j; unsigned char *p;
        for(i=0;i<8;i++) {
            p=(unsigned char *)&D_8014AA3A[i];
            for(j=0;j<sizeof(ProximityEnabled);j++)
                assert(p[j]==((i==(unsigned)index && j==0)?(unsigned char)enabled:0xA5));
            p=(unsigned char *)&player_array[i];
            for(j=0;j<sizeof(ProximityPlayer);j++)
                if(i!=(unsigned)index || j<8 || j>=20) assert(p[j]==0xA5);
        }
    }
    return result;
}
