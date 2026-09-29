/* zlib's tables at their game addresses. Derived from the relocation sites of
 * the compiled members against the target words; the .data ones sit in
 * exactly zlib 1.0.4's source order (extra_lbits, extra_dbits, extra_blbits,
 * bl_order, static_l/d/bl_desc, then tr_static_init's static_init_done). */
#define extra_lbits          D_8011E888
#define extra_dbits          D_8011E8FC
#define extra_blbits         D_8011E974
#define bl_order             D_8011E9C0
#define static_l_desc        D_8011E9D4
#define static_d_desc        D_8011E9E8
#define static_bl_desc       D_8011E9FC
#define configuration_table  D_8011E838
#define static_ltree         D_801249F0
#define static_dtree         D_80124E70
#define dist_code            D_80152260
#define length_code          D_80152468
#define base_length          D_80152578
#define base_dist            D_80152600
extern int extra_lbits[LENGTH_CODES];
extern int extra_dbits[D_CODES];
extern int extra_blbits[BL_CODES];
extern uch bl_order[BL_CODES];
extern ct_data static_ltree[L_CODES+2];
extern ct_data static_dtree[D_CODES];
extern uch dist_code[512];
extern uch length_code[MAX_MATCH-MIN_MATCH+1];
extern int base_length[LENGTH_CODES];
extern int base_dist[D_CODES];
struct static_tree_desc_s {
    ct_data *static_tree;
    intf    *extra_bits;
    int     extra_base;
    int     elems;
    int     max_length;
};
extern static_tree_desc static_l_desc;
extern static_tree_desc static_d_desc;
extern static_tree_desc static_bl_desc;
extern config configuration_table[10];
