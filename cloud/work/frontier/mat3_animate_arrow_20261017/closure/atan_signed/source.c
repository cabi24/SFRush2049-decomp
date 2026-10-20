typedef float f32;extern f32 func_8008C680(f32);
f32 func_8008C720(f32 arg0) {
    if (arg0 > 0.0f) {
        return func_8008C680(arg0);
    }
    return -func_8008C680(-arg0);
}
