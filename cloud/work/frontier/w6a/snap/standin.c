
void zz_caller(s32 n)
{
    s32 i;
    for (i = 0; i < n; i++) {
        func_8009F058(i, 0, 0, &D_80150B70[i]);
    }
}
void zz_caller2(s32 n)
{
    func_8009F058(n, 0, 0, &D_80150B70[n]);
}
f32 camera_update_c(f32 input) { return func_8009C3F8(input, 0); }
f32 select_screen_update(f32 input) { return func_8009C3F8(input, 1); }
