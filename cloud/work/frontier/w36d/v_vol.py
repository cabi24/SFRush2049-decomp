H='parts/header.c'
VARIANTS = {
 'base': [],
 'vol_models': [(H, "extern u8 D_80140BDC;              /* gNumModels */", "extern volatile u8 D_80140BDC;     /* gNumModels */")],
}
