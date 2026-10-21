R='parts/func_8038CCF4.c'
src=open('groups/trackscr/parts/func_8038CCF4.c').read()
VARIANTS = {
 'base': [],
 'm16int': [(R, src, src.replace('* 16.0f', '* 16'))],
 'p01dot': [(R, src, src.replace('+= 0.1f', '+= .1f'))],
 'm001dot': [(R, src, src.replace('*= 0.01f', '*= .01f'))],
 'p01dbl': [(R, src, src.replace('+= 0.1f', '+= 0.1'))],
}
