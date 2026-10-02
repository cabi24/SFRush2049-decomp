/* Complete N64 wrapper: narrow the third input to a signed 16-bit index.
 * The other three 32-bit inputs pass through unchanged to the real allocator.
 * Build through group.json with the standard whole-program IDO O3 pipeline.
 * N64-specific resource-record code; no arcade source equivalent established.
 */
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

s32 func_8008E26C(u32 value, u32 data, s16 index, u32 flags);

void sign_extend_call(u32 value, u32 data, s32 index, u32 flags)
{
    func_8008E26C(value, data, (s16) index, flags);
}
