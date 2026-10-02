typedef signed int s32;typedef unsigned int u32;typedef unsigned char u8;typedef float f32;
typedef struct View152 {u8 pad0[36];f32 position[3];u8 pad48[104];} View152;
extern View152 D_80150B70[];
typedef struct FixedMatrix {u32 integer[8],fraction[8];} FixedMatrix;
#define PAIR(row,index,a,b) do { \
    first=(s32)((a)*65536.0f);second=(s32)((b)*65536.0f); \
    (row)->integer[index]=((u32)first&0xffff0000)|((u32)second>>16); \
    (row)->fraction[index]=((u32)first<<16)|((u32)second&0xffff); \
} while(0)
#define THIRD(row,index,a) do { \
    first=(s32)((a)*65536.0f); \
    (row)->integer[index]=(u32)first&0xffff0000; \
    (row)->fraction[index]=(u32)first<<16; \
} while(0)
s32 func_8009D99C(s32 view,f32 matrix[3][4],f32 position[3],FixedMatrix *output,f32 scale,s32 absolute) {
    f32 x,y,z;
    s32 first,second,fz;
    if(absolute){x=position[0];y=position[1];z=position[2];}
    else{x=position[0]-D_80150B70[view].position[0];y=position[1]-D_80150B70[view].position[1];z=position[2]-D_80150B70[view].position[2];}
    if(x<=-2048.0f || x>=2048.0f)return 0;
    if(y<=-2048.0f || y>=2048.0f)return 0;
    if(z<=-2048.0f || z>=2048.0f)return 0;
    first=(s32)(x*1048576.0f);second=(s32)(y*1048576.0f);fz=(s32)(z*1048576.0f);
    output->integer[6]=((u32)first&0xffff0000)|((u32)second>>16);
    output->fraction[6]=((u32)first<<16)|((u32)second&0xffff);
    second=(s32)(scale*65536.0f);
    output->integer[7]=((u32)fz&0xffff0000)|((u32)second>>16);
    output->fraction[7]=((u32)fz<<16)|((u32)second&0xffff);
    PAIR(output,0,matrix[0][0],matrix[1][0]);THIRD(output,1,matrix[2][0]);
    PAIR(output,2,matrix[0][1],matrix[1][1]);THIRD(output,3,matrix[2][1]);
    PAIR(output,4,matrix[0][2],matrix[1][2]);THIRD(output,5,matrix[2][2]);
    return 1;
}
