#include "mdl_hdr.h"
#define T D_8012E700
void model_data_load(int idx, int mode, int f) {
    if (mode == 2) {
        T[idx].flags = (f << 8) | T[(short)idx].flags;
        if (T[idx].child != -1) {
            model_data_load(T[idx].child, 3, f);
        }
    } else if (mode == 0) {
        T[idx].flags = 0x80000000 | T[(short)idx].flags;
    } else if (mode == 1) {
        T[idx].flags = (f << 8) | T[(short)idx].flags;
    } else if (mode == 3) {
        do {
            T[idx].flags = (f << 8) | T[(short)idx].flags;
            if (T[idx].child != -1) {
                model_data_load(T[idx].child, 3, f);
            }
            idx = T[idx].sibling;
        } while (idx != -1);
    }
}
