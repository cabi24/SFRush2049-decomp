/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef float f32;
typedef int s32;
typedef struct Values96 {u8 opaque0[4]; f32 values[3][5]; u8 opaque64[32];} Values96;
typedef struct Handle60 {void *values[3][5];} Handle60;
typedef struct Descriptor {u8 opaque0[44]; f32 **data;} Descriptor;
typedef struct Input {Descriptor *descriptor;} Input;
extern Values96 D_80150F88[12];
extern Handle60 D_80151690[12];
void draw_text(Input *input) {
    s32 i,offset,k,shift;
    f32 *values; void **handles;
    f32 *data;
    f32 *source;
    if(input->descriptor->data) {
        data=*input->descriptor->data;
        for(i=0;i<12;i++) {
            for(offset=0;offset<60;offset+=20) {
                values=(f32 *)((u8 *)&D_80150F88[i]+offset+4);
                handles=(void **)((u8 *)&D_80151690[i]+offset);
                source=(f32 *)((u8 *)data+144+offset);
                for(k=0;k<5;k++) {
                    if(*source>0.0f && (values[k]==0.0f || *source<values[k])) {
                        for(shift=4;shift>k;shift--) {
                            values[shift]=values[shift-1];
                            handles[shift]=handles[shift-1];
                        }
                        values[k]=*source;
                        handles[shift]=input;
                        source++;
                    }
                }
            }
            data+=24;
        }
    }
}
