/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native controller helper; see cloud/work/boot_tail/C13-tail/README.md. */
unsigned char func_80021548(unsigned char controller)
{
    switch (controller) {
    case 128: controller = 128; break;
    case 129: controller = 130; break;
    case 130: controller = 160; break;
    case 131: controller = 161; break;
    case 132: controller = 131; break;
    case 133: controller = 132; break;
    }
    return controller;
}
