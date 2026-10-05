#include "menu_context.h"
#define OFFSET(T, f) ((unsigned int)&((T *)0)->f)
#define CHECK(n, test) typedef char n[(test) ? 1 : -1]
CHECK(color_size, sizeof(Color4) == 4);
CHECK(asset_size, sizeof(MenuAssets) == 20);
CHECK(asset_labels, OFFSET(MenuAssets, labels) == 4);
CHECK(asset_header, OFFSET(MenuAssets, header) == 12);
CHECK(asset_values, OFFSET(MenuAssets, values) == 16);
CHECK(header_offset, OFFSET(ResourceHeader, string_offset) == 32);
