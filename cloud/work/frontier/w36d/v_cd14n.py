R='parts/func_8038CCE4.c'
OLD="                for (i = 0; i < 8; i++) {\n                    sectVerts[i][0] *= 0.01f;\n                    sectVerts[i][1] *= 0.01f;\n                    sectVerts[i][2] *= 0.01f;\n                }\n"
EXP=''.join(f"                sectVerts[{i}][0] *= 0.01f; sectVerts[{i}][1] *= 0.01f; sectVerts[{i}][2] *= 0.01f;\n" for i in range(8))
VARIANTS = {
 'base': [],
 'explicit': [(R, OLD, EXP)],
 'nested': [(R, OLD, "                for (i = 0; i < 8; i++)\n                    for (j = 0; j < 3; j++)\n                        sectVerts[i][j] *= 0.01f;\n"), (R, "    s32 i;\n", "    s32 i, j;\n")],
}
