/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80015318 - lib_15a10 (IDO 7.1). Register a descriptor list for sample/record ID:
 * data[0] is the u16 descriptor count, the descriptors start 4 bytes in; forwards to
 * func_800164D0(id, descriptors, count) (locked, lib_16320).
 * w18c: natural one-call form (IDO 7.1 copies data to a3 for the count load, as retail).
 * Prior: D10 BT03-low-B (5.3, 3 words; 7.1 true=105 with a count local). No shaping devices. */
extern int func_800164D0(unsigned short, void *, unsigned short);

void func_80015318(unsigned short id, unsigned short *data)
{
    func_800164D0(id, data + 2, data[0]);
}
