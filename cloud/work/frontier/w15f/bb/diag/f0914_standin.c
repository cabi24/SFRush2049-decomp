/* DIAGNOSTIC STAND-IN ONLY (not a reconstruction): an internal func_800F0914 that clobbers
 * s0-s8 and f20/f22, to test whether billboard_render's residual is only F0914's IPA effect. */
extern int sprintf(char *, const char *, ...);
extern float D_8012zz[];
extern int D_80145zz[];
extern char D_80143Fzz[];
void func_800F0914(void)
{
    float a = D_8012zz[0], b = D_8012zz[1];
    int i, j = D_80145zz[0], k = D_80145zz[1], l = D_80145zz[2], m = D_80145zz[3], n = D_80145zz[4], o = D_80145zz[5], p = D_80145zz[6], q = D_80145zz[7];
    for (i = 0; i < 8; i++) {
        sprintf(D_80143Fzz, "%d", (int)(a * b));
        a += b; b *= a;
        j += k; k += l; l += m; m += n; n += o; o += p; p += q; q += i;
    }
    D_80145zz[0] = j + k + l + m + n + o + p + q;
}
