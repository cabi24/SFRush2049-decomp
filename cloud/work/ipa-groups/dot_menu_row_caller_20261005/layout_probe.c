#include "group.c"
#define OFF(T, f) ((unsigned int)&((T *)0)->f)
#define CHECK(n, e) typedef char n[(e) ? 1 : -1]
CHECK(color_size, sizeof(Color4) == 4);
CHECK(row_size, sizeof(OptionRow) == 64);
CHECK(row_angle, OFF(OptionRow, angle) == 8);
CHECK(row_position, OFF(OptionRow, position) == 48);
CHECK(row_alpha, OFF(OptionRow, alpha) == 60);
CHECK(header_offset, OFF(ResourceHeader, string_offset) == 32);
CHECK(asset_labels, OFF(MenuAssets, labels) == 4);
CHECK(asset_header, OFF(MenuAssets, header) == 12);
CHECK(asset_values, OFF(MenuAssets, values) == 16);
