OLD = "    audio_reverb_update((u32)h, 1);"
forms = ["(u32)&h->pad0", "(u32)&h[0]", "(u32)(h + 0)", "(u32)(char *)h", "(u32)&*h", "(u32)&h->next - 4", "(s32)h", "*(u32 *)&h"]
V = {}
for i, f in enumerate(forms):
    V['n%d' % i] = "    audio_reverb_update(%s, 1);" % f
