#include "mdlv_hdr.h"
void model_data_load(short idx, int mode, int f) {
    if (mode == 2) {
        D_8012E700[idx].flags = D_8012E700[idx].flags | (f << 8);
        if (D_8012E700[idx].child != -1) {
            model_data_load(D_8012E700[idx].child, 3, f);
        }
    } else if (mode == 0) {
        D_8012E700[idx].flags = D_8012E700[idx].flags | 0x80000000;
    } else if (mode == 1) {
        D_8012E700[idx].flags = D_8012E700[idx].flags | (f << 8);
    } else if (mode == 3) {
        do {
            D_8012E700[idx].flags = D_8012E700[idx].flags | (f << 8);
            if (D_8012E700[idx].child != -1) {
                model_data_load(D_8012E700[idx].child, 3, f);
            }
            idx = D_8012E700[idx].sibling;
        } while (idx != -1);
    }
}
