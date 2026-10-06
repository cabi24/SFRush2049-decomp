#!/bin/sh
# usage: sum.sh TRACE  (or - for stdin) -- one line per colouring decision, in decision order:
#   phase web save nocs totalsave bestcost numintf decision -> final register [(forced)] type/raw10/bb [DECLINED(reg)]
# final register from p1color/p2color (a web decided twice is listed once, at its first position); FP colours (bestreg=?) are named from the table in README.md.
awk '
function reg(c, r) { if (r != "?" && r != "") return r; return (c in fp) ? fp[c] : "c" c }
BEGIN { fp[24]="$f0"; fp[25]="$f2"; fp[26]="$f12"; fp[27]="$f14"; fp[28]="$f16"; fp[29]="$f18"; fp[30]="$f20"; fp[31]="$f22" }
/p[12]dec /{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} w=v["web"]; ph=v["phase"]; line[ph w]=sprintf("%s w%-4s save=%-9s nocs=%-3s tot=%-9s best=%-8s numintf=%-3s %-8s", ph, w, v["save"], v["nocs"], v["totalsave"], v["bestcost"], v["numintf"], v["decision"]); if (!((ph w) in cnt)) order[++n]=ph w; cnt[ph w]++; delete v}
/webdetail/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} det[v["phase"] v["web"]]=sprintf("type=%s raw10=%s bb=%s", v["type"], v["raw10"], v["bb"]); delete v}
/p[12]color/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} col[v["phase"] v["web"]]=reg(v["color"], v["reg"]) (v["forced"]!="-2" ? "(forced)" : ""); delete v}
/force_declined/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} dec[v["phase"] v["web"]]=" DECLINED(" reg(v["color"], v["reg"]) ")"; delete v}
END{for(i=1;i<=n;i++){k=order[i]; printf "%s -> %-12s %s%s%s\n", line[k], col[k], det[k], dec[k], (cnt[k] > 1 ? " (decided " cnt[k] "x, last shown)" : "")}}' "${1:--}"
