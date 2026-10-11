// Registers N64Recomp's section table (four non-relocatable sections: static,
// game, ovl_a, ovl_b) with librecomp. The static section is loaded by
// librecomp's init (ROM 0x1000 -> 0x80000400); the game image and the two
// overlay images are loaded by the inflate hook (hooks.cpp).
#include "librecomp/overlays.hpp"
#include "RecompiledFuncs/recomp_overlays.inl"
#include "rush_runtime.h"

void rush::register_overlays() {
    recomp::overlays::overlay_section_table_data_t sections{
        .code_sections = section_table,
        .num_code_sections = ARRLEN(section_table),
        .total_num_sections = num_sections,
    };
    recomp::overlays::overlays_by_index_t overlays{
        .table = overlay_sections_by_index,
        .len = ARRLEN(overlay_sections_by_index),
    };
    recomp::overlays::register_overlays(sections, overlays);
}
