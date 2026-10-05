#!/usr/bin/env python3
"""gen.py OUTDIR N [seed] : write N random variants of net_state_validate from choice points (variable sharing, loop forms).
Each file name encodes the choice vector. Used with batch.sh to search register-allocation neighbours."""
import sys, random, re, os
head = open(os.path.join(os.path.dirname(__file__), 'best.c')).read()
head = head[:head.index("void net_state_validate(void) {")]
CH = {
 'cmp1': ['!=', '<'],            # sec1 call loops comparator
 'rec': ['two', 'one', 'none'],  # sec1 track/cup pointer vars
 'k1': ['k', 'm'],               # sec1 loop var
 'sum2': ['sum', 'total'],       # sec2 accumulator
 'k2': ['k', 'm'],
 'd2': ['data', 'data2', 'none'],
 's3': ['E', 'C', 'L', 'P', 'Lne', 'B3E', 'B3L', 'W1', 'W2', 'W3', 'W4', 'W5', 'W6', 'X1', 'X2', 'X3', 'X4', 'X5', 'X6', 'X7', 'X8', 'Y1', 'Y2', 'Y3', 'Y4'],
 'k3': ['k', 'm'],
 'd3': ['data', 'data3'],
 't3': ['total', 'sum'],
 'd4': ['data', 'data4'],
 'b4': ['base', 'none'],
 'f4': ['flag', 'none'],
 'cont': ['cont', 'block'],
 'jty': ['s16'],
 'p1': ['p', 'none'],
}
def gen(c):
    D = []
    def decl(x):
        if x not in D: D.append(x)
    for x in ["s16 i;", "s16 j;", "s32 k;", "s32 n0;", "s32 n1;", "s32 n2;", "s32 n3;"]: decl(x)
    o = []
    A = o.append
    # ---- section 1
    A("    if (D_801164C0 == 1) {\n        for (i = 0; i < D_8014A108; i++) {\n            for (j = 0; j < 13; j++) {\n                D_80150E30[i][j] = 1;\n            }\n        }\n    } else {\n        for (i = 0; i < D_8014A108; i++) {")
    if c['p1'] == 'p':
        decl("Player76 *p;"); A("            p = &D_8014A118[i];"); P = "p->"
    else:
        P = "D_8014A118[i]."
    A("            n0 = 0;\n            n1 = 0;\n            n2 = 0;\n            n3 = 0;")
    A("            if (%sref->car->data == 0) {\n                continue;\n            }" % P)
    A("            for (j = 0; j < 13; j++) {\n                if (j < 6) {\n                    D_80150E30[i][j] = 1;\n                } else {\n                    D_80150E30[i][j] = 0;\n                }\n            }")
    k1 = c['k1']; decl("s32 %s;" % k1)
    if c['rec'] == 'two':
        decl("TrackRec *track;"); decl("CupRec *cup;")
        t1 = "                track = &%sref->car->data->base->tracks[%s];\n" % (P, k1); T = "track->unk5C"
        t2 = "                cup = &%sref->car->data->base->cups[%s];\n" % (P, k1); C = "cup->unk3C"
    elif c['rec'] == 'one':
        decl("u8 *rec;")
        t1 = "                rec = (u8 *)&%sref->car->data->base->tracks[%s];\n" % (P, k1); T = "((TrackRec *)rec)->unk5C"
        t2 = "                rec = (u8 *)&%sref->car->data->base->cups[%s];\n" % (P, k1); C = "((CupRec *)rec)->unk3C"
    else:
        t1 = ""; T = "%sref->car->data->base->tracks[%s].unk5C" % (P, k1)
        t2 = ""; C = "%sref->car->data->base->cups[%s].unk3C" % (P, k1)
    A("            for (%s = 0; %s %s 6; %s++) {\n%s                n0 += func_800B78A4(%s & 0xFF, 16);\n                n1 += func_800B78A4(%s & 0xFF00, 16);\n            }" % (k1, k1, c['cmp1'], k1, t1, T, T))
    A("            for (%s = 0; %s %s 4; %s++) {\n%s                n2 += func_800B78A4(%s & 0xFF, 16);\n                n3 += func_800B78A4(%s & 0xFF00, 16);\n            }" % (k1, k1, c['cmp1'], k1, t2, C, C))
    A("""            if (n0 >= 48) {
                D_80150E30[i][6] = 1;
            }
            if (n1 >= 24) {
                D_80150E30[i][7] = 1;
            }
            if (n1 >= 36) {
                D_80150E30[i][8] = 1;
            }
            if (n2 >= 32) {
                D_80150E30[i][9] = 1;
            }
            if (n3 >= 16) {
                D_80150E30[i][10] = 1;
            }
            if (n3 >= 24) {
                D_80150E30[i][11] = 1;
            }
            if (n0 >= 48 && n1 >= 48 && n2 >= 32 && n3 >= 32) {
                D_80150E30[i][12] = 1;
            }
        }
    }
""")
    # ---- section 2
    A("""    if (D_801164C4 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            if (D_8014A118[i].ref->car->data == 0) {
                continue;
            }
            for (j = 0; j < 8; j++) {
                if (j == 6 || j == 7) {
                    D_80150EB8[i][j] = 0;
                } else {
                    D_80150EB8[i][j] = 1;
                }
            }
            for (j = 0; j < 9; j++) {
                D_80150ED8[i][j] = 1;
            }
            for (j = 0; j < 5; j++) {
                D_80150F00[i][j] = 1;
            }
            for (j = 0; j < 6; j++) {
                D_80150F40[i][j] = 1;
            }
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {""")
    S = c['sum2']; decl("s32 %s;" % S); k2 = c['k2']; decl("s32 %s;" % k2)
    A("            %s = 0;" % S)
    if c['d2'] == 'none':
        A("            if (D_8014A118[i].ref->car->data == 0) {\n                continue;\n            }")
        d2 = "D_8014A118[i].ref->car->data"
    else:
        d2 = c['d2']; decl("Data *%s;" % d2)
        A("            %s = D_8014A118[i].ref->car->data;\n            if (%s == 0) {\n                continue;\n            }" % (d2, d2))
    A("            for (%s = 0; %s < 12; %s++) {\n                %s += %s->base->tracks[%s].unk58;\n            }\n            %s /= 10;" % (k2, k2, k2, S, d2, k2, S))
    for arr, vals in (("D_80150EB8", ["1", "1", ">= 200", ">= 200", ">= 500", ">= 500", "0", "0"]),
                      ("D_80150ED8", ["1", "1", "1", ">= 250", ">= 500", ">= 800", ">= 1200", ">= 1600", ">= 2000"]),
                      ("D_80150F00", ["1", ">= 300", ">= 1200", ">= 100", ">= 600"]),
                      ("D_80150F40", ["1", "1", "1", ">= 150", ">= 400", ">= 700"])):
        for n, v in enumerate(vals):
            A("            %s[i][%d] = %s;" % (arr, n, v if v in ("0", "1") else "%s %s" % (S, v)))
    A("        }\n    }\n")
    # ---- section 3
    T3 = c['t3']; decl("s32 %s;" % T3); decl("s32 total2;"); k3 = c['k3']; decl("s32 %s;" % k3); d3 = c['d3']; decl("Data *%s;" % d3)
    A("""    if (D_801164C2 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            for (j = 0; j < 19; j++) {
                D_80150DD8[i][j] = 1;
            }
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {
            total2 = 0;
            for (j = 0; j < 19; j++) {
                D_80150DD8[i][j] = 0;
            }
            D_80150DD8[i][0] = 1;
            D_80150DD8[i][1] = 1;
            D_80150DD8[i][2] = 1;
            D_80150DD8[i][3] = 1;
            D_80150DD8[i][14] = 1;
            D_80150DD8[i][6] = 1;
            D_80150DD8[i][7] = 1;
            D_80150DD8[i][8] = 1;
            D_80150DD8[i][9] = 1;""")
    A("            %s = D_8014A118[i].ref->car->data;\n            if (%s == 0) {\n                continue;\n            }" % (d3, d3))
    s3 = c['s3']
    if s3 in ('E', 'C', 'P'):
        decl("CupRec *cup2;"); decl("StuntRec *stunt;")
        A("            cup2 = %s->base->cups;" % d3)
        if s3 == 'C':
            A("            %s = cup2[0].unk0C + cup2[1].unk0C + cup2[2].unk0C + cup2[3].unk0C;" % T3)
        else:
            A("            %s = cup2[0].unk0C;\n            %s += cup2[1].unk0C;\n            %s += cup2[2].unk0C;\n            %s += cup2[3].unk0C;" % (T3, T3, T3, T3))
        if s3 == 'P':
            A("            for (%s = 0, stunt = %s->base->stunts; %s < 8; %s++, stunt++) {\n                total2 += stunt->unk8;\n            }" % (k3, d3, k3, k3))
        else:
            A("            stunt = %s->base->stunts;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt[%s].unk8;\n            }" % (d3, k3, k3, k3, k3))
    elif s3 in ('L', 'Lne'):
        op = '<' if s3 == 'L' else '!='
        A("            %s = 0;\n            for (%s = 0; %s %s 4; %s++) {\n                %s += %s->base->cups[%s].unk0C;\n            }\n            for (%s = 0; %s %s 8; %s++) {\n                total2 += %s->base->stunts[%s].unk8;\n            }" % (T3, k3, k3, op, k3, T3, d3, k3, k3, k3, op, k3, d3, k3))
    elif s3 in ('B3E', 'B3L'):
        decl("SaveBase *base3;")
        A("            base3 = %s->base;" % d3)
        if s3 == 'B3E':
            A("            %s = base3->cups[0].unk0C + base3->cups[1].unk0C + base3->cups[2].unk0C + base3->cups[3].unk0C;" % T3)
            A("            for (%s = 0; %s < 8; %s++) {\n                total2 += base3->stunts[%s].unk8;\n            }" % (k3, k3, k3, k3))
        else:
            A("            %s = 0;\n            for (%s = 0; %s != 4; %s++) {\n                %s += base3->cups[%s].unk0C;\n            }\n            for (%s = 0; %s != 8; %s++) {\n                total2 += base3->stunts[%s].unk8;\n            }" % (T3, k3, k3, k3, T3, k3, k3, k3, k3, k3))
    elif s3 in ('W1', 'W2', 'W3', 'W4', 'W5', 'W6'):
        decl("SaveBase *base3;"); decl("CupRec *cup2;"); decl("StuntRec *stunt;")
        A("            base3 = %s->base;" % d3)
        if s3 == 'W1':
            A("            %s = 0;\n            cup2 = base3->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2->unk0C;\n                cup2++;\n            }\n            stunt = (StuntRec *)cup2;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt->unk8;\n                stunt++;\n            }" % (T3, k3, k3, k3, T3, k3, k3, k3))
        elif s3 == 'W2':
            A("            %s = 0;\n            for (cup2 = base3->cups; cup2 != &base3->cups[4]; cup2++) {\n                %s += cup2->unk0C;\n            }\n            for (stunt = base3->stunts; stunt != &base3->stunts[8]; stunt++) {\n                total2 += stunt->unk8;\n            }" % (T3, T3))
        elif s3 == 'W3':
            A("            cup2 = base3->cups;\n            %s = cup2[0].unk0C;\n            %s += cup2[1].unk0C;\n            %s += cup2[2].unk0C;\n            %s += cup2[3].unk0C;\n            stunt = base3->stunts;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt[%s].unk8;\n            }" % (T3, T3, T3, T3, k3, k3, k3, k3))
        elif s3 == 'W4':
            A("            %s = 0;\n            cup2 = base3->cups;\n            for (%s = 0; %s < 4; %s++, cup2++) {\n                %s += cup2->unk0C;\n            }\n            stunt = base3->stunts;\n            for (%s = 0; %s < 8; %s++, stunt++) {\n                total2 += stunt->unk8;\n            }" % (T3, k3, k3, k3, T3, k3, k3, k3))
        elif s3 == 'W5':
            A("            %s = 0;\n            cup2 = base3->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2[%s].unk0C;\n            }\n            stunt = (StuntRec *)&cup2[4];\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt[%s].unk8;\n            }" % (T3, k3, k3, k3, T3, k3, k3, k3, k3, k3))
        elif s3 == 'W6':
            A("            cup2 = base3->cups;\n            %s = cup2[0].unk0C + cup2[1].unk0C + cup2[2].unk0C + cup2[3].unk0C;\n            stunt = base3->stunts;\n            for (%s = 0; %s < 8; %s++, stunt++) {\n                total2 += stunt->unk8;\n            }" % (T3, k3, k3, k3))
    elif s3[0] == 'Y':
        decl("CupRec *cup2;"); decl("StuntRec *stunt;"); decl("s32 q;")
        usebase = s3 in ('Y1', 'Y3')
        if usebase:
            decl("SaveBase *base3;"); A("            base3 = %s->base;" % d3); Bx = "base3"
        else:
            Bx = "%s->base" % d3
        A("            %s = 0;" % T3)
        if s3 in ('Y1', 'Y2'):
            A("            cup2 = %s->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2->unk0C;\n                cup2++;\n            }\n            stunt = (StuntRec *)cup2;\n            for (q = 0; q < 8; q++) {\n                total2 += stunt[q].unk8;\n            }" % (Bx, k3, k3, k3, T3))
        else:
            A("            cup2 = %s->cups;\n            stunt = %s->stunts;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2[%s].unk0C;\n            }\n            for (q = 0; q < 8; q++) {\n                total2 += stunt[q].unk8;\n            }" % (Bx, Bx, k3, k3, k3, T3, k3))
    elif s3[0] == 'X':
        decl("SaveBase *base3;"); decl("CupRec *cup2;"); decl("StuntRec *stunt;"); decl("s32 q;")
        kb = 'q'
        usebase = s3 in ('X1', 'X2', 'X3', 'X4')
        Bx = "base3" if usebase else "%s->base" % d3
        if usebase: A("            base3 = %s->base;" % d3)
        A("            %s = 0;" % T3)
        if s3 in ('X1', 'X5'):
            A("            cup2 = %s->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2->unk0C;\n                cup2++;\n            }\n            stunt = %s->stunts;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt[%s].unk8;\n            }" % (Bx, k3, k3, k3, T3, Bx, kb, kb, kb, kb))
        elif s3 in ('X2', 'X6'):
            A("            cup2 = %s->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2[%s].unk0C;\n            }\n            stunt = %s->stunts;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt[%s].unk8;\n            }" % (Bx, k3, k3, k3, T3, k3, Bx, kb, kb, kb, kb))
        elif s3 in ('X3', 'X7'):
            A("            for (%s = 0; %s < 4; %s++) {\n                %s += %s->cups[%s].unk0C;\n            }\n            for (%s = 0; %s < 8; %s++) {\n                total2 += %s->stunts[%s].unk8;\n            }" % (k3, k3, k3, T3, Bx, k3, kb, kb, kb, Bx, kb))
        elif s3 in ('X4', 'X8'):
            A("            cup2 = %s->cups;\n            for (%s = 0; %s < 4; %s++) {\n                %s += cup2->unk0C;\n                cup2++;\n            }\n            stunt = (StuntRec *)cup2;\n            for (%s = 0; %s < 8; %s++) {\n                total2 += stunt->unk8;\n                stunt++;\n            }" % (Bx, k3, k3, k3, T3, kb, kb, kb))
    for n, v in ((15, 100000), (16, 250000), (17, 500000), (18, 1000000)):
        A("            D_80150DD8[i][%d] = %s >= %d;" % (n, T3, v))
    for n, v in ((10, 100), (11, 250), (12, 500), (13, 1000)):
        A("            D_80150DD8[i][%d] = total2 >= %d;" % (n, v))
    A("        }\n    }\n")
    # ---- section 4
    d4 = c['d4']; decl("Data *%s;" % d4)
    A("""    if (D_801164C2 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            for (j = 0; j < 4; j++) {
                D_80150E88[i][j] = 1;
            }
        }
        if (D_80156994 == 0) {
            D_80150E88[i][2] = 0;
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {""")
    A("            %s = D_8014A118[i].ref->car->data;\n            if (%s == 0) {\n                continue;\n            }" % (d4, d4))
    if c['b4'] == 'base':
        decl("SaveBase *base;"); A("            base = %s->base;" % d4); B = "base"
    else:
        B = "%s->base" % d4
    if c['f4'] == 'flag':
        decl("s8 flag;"); A("            flag = D_80156994;"); F = "flag"
    else:
        F = "D_80156994"
    A("            for (j = 0; j < 4; j++) {\n                D_80150E88[i][j] = 0;\n            }\n            D_80150E88[i][0] = 1;")
    def cond(n, f): return "%s->modes[%d].unk2 != 0 || %s->modes[%d].unk4 != 0 || %s->modes[%d].unk6 != 0 || D_80150F7C[%d] != 0" % (B, n, B, n, B, n, f)
    A("            if (%s) {\n                D_80150E88[i][1] = 1;\n                D_80150DD8[i][4] = 1;\n            }" % cond(1, 1))
    A("            if (%s == 0) {\n                if (%s) {\n                    D_80150E88[i][3] = 1;\n                }\n            } else {\n                if (%s) {\n                    D_80150E88[i][2] = 1;\n                    D_80150DD8[i][5] = 1;\n                }\n                if (%s) {\n                    D_80150E88[i][3] = 1;\n                }\n            }" % (F, cond(2, 3), cond(2, 2), cond(3, 3)))
    A("        }\n    }\n}")
    return head + "void net_state_validate(void) {\n" + "".join("    %s\n" % d for d in D) + "\n" + "\n".join(o) + "\n"
if __name__ == '__main__':
    out, n = sys.argv[1], int(sys.argv[2]); random.seed(int(sys.argv[3]) if len(sys.argv) > 3 else 0)
    keys = sorted(CH); seen = set()
    os.makedirs(out, exist_ok=True)
    fixed = dict(a.split('=', 1) for a in sys.argv[4:])
    while len(seen) < n:
        c = {k: (fixed[k] if k in fixed else random.choice(CH[k])) for k in keys}
        name = "_".join("%s-%s" % (k, c[k].replace('!=', 'ne').replace('<', 'lt')) for k in keys)
        if name in seen: continue
        seen.add(name)
        open(os.path.join(out, name + '.c'), 'w').write(gen(c))
