#!/bin/sh
# usage: sum71.sh TRACE|- [VARS]  -- one line per 7.1 colouring decision, in decision order (VARS: vars71.sh output,
# `OFFSET NAME KIND` lines; names the var@OFF webs):
#   phase web save nocs totalsave bestcost numintf decision -> final register [(forced)]  what  bb [DECLINED(reg)]
# `what` decodes the web's ichain (7.1 layout, w22c): type 3 = variable `var@OFF/SIZE` (OFF = cfe's virtual-frame offset,
# raw10; SIZE = top byte of raw18 -- locals are laid out downward in declaration order, so the first int local is
# var@-4/4, then each next one below it; parameters are positive), type 4 = expression web (`expr op=0xNN`, top
# byte of raw10), type 1/2 = constant / address webs (`const`, `addr`). bb = first block of the web's expression.
# Same record format as the 5.3 CDX profile; colour names: integer c1 v0 .. c22 s8 (checked on 7.1 objects), FP as A71.
awk -v vars="${2:-}" '
function reg(c, r) { if (r != "?" && r != "") return r; return (c in fp) ? fp[c] : "c" c }
function hx(s,  i,c,n) { s=tolower(s); sub(/^0x/,"",s); n=0; for(i=1;i<=length(s);i++){c=index("0123456789abcdef",substr(s,i,1))-1; n=n*16+c} return n }
BEGIN { if (vars != "") while ((getline l < vars) > 0) { split(l, f, " "); vn[f[1]] = f[2] }
        fp[24]="$f0"; fp[25]="$f2"; fp[26]="$f12"; fp[27]="$f14"; fp[28]="$f16"; fp[29]="$f18"; fp[30]="$f20"; fp[31]="$f22" }
/p[12]dec /{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} w=v["web"]; ph=v["phase"]; line[ph w]=sprintf("%s w%-4s save=%-10s nocs=%-3s tot=%-10s best=%-8s numintf=%-3s %-8s", ph, w, v["save"], v["nocs"], v["totalsave"], v["bestcost"], v["numintf"], v["decision"]); if (!((ph w) in cnt)) order[++n]=ph w; cnt[ph w]++; delete v}
/webdetail/ && / role=target /{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} t=v["type"]; r10=hx(v["raw10"]);
  if (t==3) { o=r10; if (o>=2147483648) o-=4294967296; d=sprintf("var@%d/%d", o, int(hx(v["raw18"])/16777216)); if ((o "") in vn) d = d " " vn[o ""] }
  else if (t==4) d=sprintf("expr op=0x%02x", int(r10/16777216)); else if (t==1) d="addr"; else if (t==2) d=sprintf("const %d", hx(v["raw10"])); else d="type" t;
  det[v["phase"] v["web"]]=sprintf("%-22s bb=%s", d, v["bb"]); delete v}
/p[12]color/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} col[v["phase"] v["web"]]=reg(v["color"], v["reg"]) (v["forced"]!="-2" ? "(forced)" : ""); delete v}
/force_declined/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} dec[v["phase"] v["web"]]=" DECLINED(" reg(v["color"], v["reg"]) ")"; delete v}
END{for(i=1;i<=n;i++){k=order[i]; printf "%s -> %-12s %s%s%s\n", line[k], col[k], det[k], dec[k], (cnt[k] > 1 ? " (decided " cnt[k] "x, last shown)" : "")}}' "${1:--}"
