/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Input-controller field wrapper; see cloud/work/boot_tail/C13-small/README.md. */
extern unsigned short func_80021150(void *voice, void *control);

unsigned short func_80021448(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 214);
}
