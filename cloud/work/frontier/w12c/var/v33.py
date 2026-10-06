M='''#define kill_scrape_sound(p) \\
    scheduler_recv(D_80140AE0[p]); \\
    D_80140AE0[p] = -1; \\
    D_80140A08[p] = 0; \\
    D_80140B10[p] = 0.0f
'''
def body(ind, order):
    lines={'r':'scheduler_recv(D_80140AE0[p]);','h':'D_80140AE0[p] = -1;','s':'D_80140A08[p] = 0;','t':'D_80140B10[p] = 0.0f;'}
    return '\n'.join(ind+lines[c] for c in order)
def mk(order):
    return [(M,''),
      ('            if (scrape_side == 0) {\n                kill_scrape_sound(p);\n            }\n            else','            if (scrape_side == 0) {\n'+body('                ',order)+'\n            }\n            else'),
      ('            if (scrape_side == 0) {\n                kill_scrape_sound(p);\n            }\n            break;','            if (scrape_side == 0) {\n'+body('                ',order)+'\n            }\n            break;'),
      ('    } else {\n        kill_scrape_sound(p);\n    }','    } else {\n'+body('        ',order)+'\n    }')]
V={'out':mk('rhst')}
