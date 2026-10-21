E='parts/func_8038E5DC.c'; H='parts/header.c'
VARIANTS = {
 'alias_diag': [(E, "                if (D_80146108[29] < 0) D_80146108[29] = 3;", "                if (D_80146108[29] < 0) D_8014611D[8] = 3;"), (H, "extern s8 D_80146108[];            /* gOptions */", "extern s8 D_80146108[];            /* gOptions */\nextern s8 D_8014611D[];")],
 'ptr_cast': [(E, "                if (D_80146108[29] < 0) D_80146108[29] = 3;", "                if (D_80146108[29] < 0) ((u8 *)D_80146108)[29] = 3;")],
}
