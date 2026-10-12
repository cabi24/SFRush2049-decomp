# w22c — instrumented IDO 7.1 trace toolkit (trace71) + 7.1 allocation residuals

Status: IN PROGRESS (stub written early per wave rules).

## Plan
1. Rebuild IDO 7.1 uopt/ugen/as1 from decompals/ido-static-recomp v1.2 (the release the pinned 7.1 binaries
   come from) in builder scratch `~/rush2049/scratch/frontier/w22c/isr71`; instrument the generated C
   (globalcolor CDX hooks ported from the workbench 5.3 profile, ugen free-list/emit hooks, as1 -R printf).
2. Gate: rebuilt-stock == release 7.1 and traced (off/on) == stock on locked boot-tail TUs.
3. Package `cloud/work/frontier/tools/trace71/`.
4. Residuals: func_80018448, func_80010E80, func_80018634, func_8001A270, func_80012D18.
