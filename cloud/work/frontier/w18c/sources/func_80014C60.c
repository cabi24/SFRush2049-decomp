/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014C60 - lib_11640 (IDO 7.1). Sample-buffer wrap guard: copy the first four samples
 * of BUFFER to just past its end (buffer[samples..samples+3]) and write the 8 bytes back from
 * the data cache. Caller: func_8001C3CC's loop (lib_1cf90, locked, `(short *, unsigned int)`).
 * w18c: indexed stores (no destination local: retail recomputes buffer+samples for the call).
 * Prior: D10 BT03-low-followon (destination local; 7.1 true=300). No shaping devices. */
extern void osWritebackDCache(void *, int);

void func_80014C60(short *buffer, unsigned int samples)
{
    buffer[samples + 0] = buffer[0];
    buffer[samples + 1] = buffer[1];
    buffer[samples + 2] = buffer[2];
    buffer[samples + 3] = buffer[3];
    osWritebackDCache(&buffer[samples], 8);
}
