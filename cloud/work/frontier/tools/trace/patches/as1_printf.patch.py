# Toolkit copy of cloud/work/frontier/w4a/tools/patch_as1_printf.py (unchanged logic). Run in the dir holding libc_impl.c.
# patch ido-static-recomp libc_impl.c so the recompiled as1 can print its -R scheduler trace (printf wrapper)
s=open("libc_impl.c").read()
old='    assert(0 && "printf not implemented");\n    return 0;'
if old in s:
    s=s.replace(old,'    return w4a_vprintf(mem, format_addr, sp + 4);')
else:
    s=s.replace('    return wrapper_fprintf(mem, STDOUT_ADDR, format_addr, sp - 4);','    return w4a_vprintf(mem, format_addr, sp + 4);')
fn=r'''
static int w4a_vprintf(uint8_t* mem, uint32_t format_addr, uint32_t sp) {
    int ret = 0;
    for (;;) {
        char ch = MEM_S8(format_addr);
        if (ch == '\0') break;
        if (ch != '%') { putchar(ch); format_addr++; continue; }
        char spec[64]; int n = 0; spec[n++] = '%'; format_addr++;
        for (;;) {
            ch = MEM_S8(format_addr++);
            if (ch == 'l') continue;
            spec[n++] = ch; spec[n] = 0;
            if (strchr("diouxXcsfeEgGp%", ch)) break;
            if (n > 60) break;
        }
        switch (ch) {
            case '%': putchar('%'); break;
            case 'd': case 'i': case 'o': case 'u': case 'x': case 'X': case 'c':
                printf(spec, MEM_U32(sp)); sp += 4; ret++; break;
            case 'p': spec[n-1] = 'x'; printf(spec, MEM_U32(sp)); sp += 4; ret++; break;
            case 's': {
                uint32_t a = MEM_U32(sp); char tmp[4096]; int k = 0; sp += 4;
                if (a) { while (k < 4095 && MEM_S8(a + k)) { tmp[k] = MEM_S8(a + k); k++; } }
                tmp[k] = 0; printf(spec, a ? tmp : "(null)"); ret++; break; }
            case 'f': case 'e': case 'E': case 'g': case 'G':
                sp = (sp + 7) & ~7u; printf(spec, MEM_F64(sp)); sp += 8; ret++; break;
            default: printf("%s", spec); break;
        }
    }
    fflush(stdout);
    return ret;
}
'''
anchor='int wrapper_printf(uint8_t* mem, uint32_t format_addr, uint32_t sp) {'
pre='int wrapper_fprintf(uint8_t* mem, uint32_t fp_addr, uint32_t format_addr, uint32_t sp);\n'
s=s.replace(pre+anchor, anchor)
s=s.replace(anchor, fn+anchor,1)
open("libc_impl.c","w").write(s)
