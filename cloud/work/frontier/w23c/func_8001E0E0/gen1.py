import itertools, sys
HDR = '''/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
typedef unsigned int u32;
extern const float D_8002CA40[129];
extern const float D_8002CC44[4];

void func_8001E0E0(u16 *left, u16 *right, u32 volume, u32 pan,
                   u32 span, u16 *span_output, u32 aux, u16 *aux_output)
{
    float fraction;
    float gain;
    float factor;
    const float *entry;

'''
def interp(dst, x, mask, k, tab, shift, mode):
    e = "(float)(%s & %s) * %s" % (x, mask, k)
    if mode:  # via fraction var
        return ("    fraction = %s;\n    entry = &%s[%s >> %d];\n    %s = (1.0f - fraction) * entry[0] +\n           entry[1] * fraction;\n"
                % (e, tab, x, shift, dst))
    return ("    entry = &%s[%s >> %d];\n    %s = (1.0f - %s) * entry[0] +\n           entry[1] * (%s);\n" % (tab, x, shift, dst, e, e))
V = ("D_8002CA40", 16, "0xFFFFU", "(1.0f / 65536.0f)")
P = ("D_8002CC44", 22, "0x3FFFFFU", "(1.0f / 4194304.0f)")
def body(m):
    s = "    if (volume > 0x7F0000U) {\n        volume = 0x7F0000U;\n    }\n"
    s += interp("gain", "volume", V[2], V[3], V[0], V[1], m[0])
    s += interp("factor", "span", P[2], P[3], P[0], P[1], m[1])
    s += "    *span_output = (int)(gain * factor * 0.7079f * 32767.0f);\n"
    s += "    span = 0x800000U - span;\n    if (span >= 0x800000U) {\n        span = 0x7F0000U;\n    }\n"
    s += interp("factor", "span", P[2], P[3], P[0], P[1], m[2])
    s += "    gain *= factor;\n    if (pan == 0x800000U) {\n        *left = (int)(gain * 0.7079f * 32767.0f);\n        *right = (int)(gain * -0.7079f * 32767.0f);\n    } else {\n"
    s += interp("factor", "pan", P[2], P[3], P[0], P[1], m[3]).replace("\n    ", "\n        ").replace("    ", "        ", 1)
    s += "        *left = (int)(gain * factor * 32767.0f);\n        pan = 0x800000U - pan;\n        if (pan >= 0x800000U) {\n            pan = 0x7F0000U;\n        }\n"
    s += interp("factor", "pan", P[2], P[3], P[0], P[1], m[4]).replace("\n    ", "\n        ").replace("    ", "        ", 1)
    s += "        *right = (int)(gain * factor * 32767.0f);\n    }\n"
    s += "    if (aux > 0x7F0000U) {\n        aux = 0x7F0000U;\n    }\n"
    s += interp("gain", "aux", V[2], V[3], V[0], V[1], m[5])
    s += "    *aux_output = (int)(gain * 0.7079f * 32767.0f);\n}\n"
    return HDR + s
out = sys.argv[1]
for m in itertools.product((0, 1), repeat=6):
    open("%s/m%s.c" % (out, "".join(map(str, m))), "w").write(body(m))
