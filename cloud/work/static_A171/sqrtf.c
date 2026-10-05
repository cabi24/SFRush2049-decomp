/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
float sqrtf(float);
#pragma intrinsic(sqrtf)
float sqrtf(float value)
{
    return sqrtf(value);
}
