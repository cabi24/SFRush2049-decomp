#include "mdl_hdr.h"
#define T D_8012E700
void model_data_load(int idx, int mode, int f) {
    if (mode == 2) {
        *(unsigned *)&T[idx].flags = T[(short)idx].flags | (f << 8);
        if (T[idx].child != -1) {
            model_data_load(T[idx].child, 3, f);
        }
    } else if (mode == 0) {
        *(unsigned *)&T[idx].flags = T[(short)idx].flags | 0x80000000;
    } else if (mode == 1) {
        *(unsigned *)&T[idx].flags = T[(short)idx].flags | (f << 8);
    } else if (mode == 3) {
        do {
            *(unsigned *)&T[idx].flags = T[(short)idx].flags | (f << 8);
            if (T[idx].child != -1) {
                model_data_load(T[idx].child, 3, f);
            }
            idx = T[idx].sibling;
        } while (idx != -1);
    }
}
