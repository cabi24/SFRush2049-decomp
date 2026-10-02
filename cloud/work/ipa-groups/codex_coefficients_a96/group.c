/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef float f32;
typedef struct Object {u8 other0[1468];f32 numerator;u8 other1472[8];f32 denominator;} Object;
typedef struct Coefficients {u8 other0[12];f32 input12,input16;u8 other20[4];f32 out24,out28,out32,out36,out40,out44,out48,out52,out56,out60,out64,out68;} Coefficients;
extern f32 D_80124138,D_8012413C,D_80124140,D_80124144;
void track_preview_handler(Object *object,Coefficients *output,f32 scale)
{
    f32 frequency=output->input16;
    f32 ratio,square,frequency3,unit;
    output->out24=(object->numerator*scale/object->denominator)*0.5f;
    frequency3=frequency*3.0f;
    ratio=output->input12/output->out24;
    output->out32=ratio;
    output->out28=frequency3*output->out24/output->input12;
    square=ratio*ratio;
    output->out36=square/frequency3;
    output->out40=(square*ratio)/((27.0f*frequency)*frequency);
    output->out44=output->out36+output->out36;
    output->out48=output->out40*3.0f;
    unit=D_80124138/frequency;
    output->out52=unit;
    square=unit*unit;
    output->out56=square*D_8012413C;
    output->out60=(square*unit)*D_80124140;
    output->out64=D_80124144;
    output->out68=0.0f;
}
