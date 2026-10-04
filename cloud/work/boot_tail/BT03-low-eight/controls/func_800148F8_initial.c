/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioSlot {
    unsigned char active;
    unsigned char pending;
    unsigned char unknown02[2];
    unsigned short rate;
    unsigned char unknown06[14];
    unsigned int position;
    unsigned char unknown18[8];
    unsigned short value20;
    unsigned short value22;
    float value24;
    unsigned short release_count;
    unsigned char unknown2A[62];
} AudioSlot;
extern AudioSlot *D_80038294;
void func_800148F8(int index, unsigned char *data)
{
    D_80038294[index].value20 = (data[0] << 8) | data[1];
    D_80038294[index].value22 = (data[2] << 8) | data[3];
    D_80038294[index].value24 = (float)(unsigned int)((data[4] << 8) | data[5]) * (1.0 / 4096.0);
    D_80038294[index].release_count = (data[6] << 8) | data[7];
}
