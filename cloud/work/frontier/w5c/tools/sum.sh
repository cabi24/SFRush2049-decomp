#!/bin/sh
# sum.sh TRACE: one line per colouring decision (final colour from p1color/p2color; forced/declined marked)
awk '
/p[12]dec /{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} w=v["web"]; ph=v["phase"]; line[ph w]=sprintf("%s w%-3s save=%-9s nocs=%-3s tot=%-8s numintf=%-3s %s", ph, w, v["save"], v["nocs"], v["totalsave"], v["numintf"], v["decision"]); order[++n]=ph w; delete v}
/webdetail/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} det[v["phase"] v["web"]]=sprintf("type=%s raw10=%s bb=%s", v["type"], v["raw10"], v["bb"]); delete v}
/p[12]color/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} col[v["phase"] v["web"]]=v["reg"] (v["forced"]!="-2" ? "(forced)" : ""); delete v}
/force_declined/{for(i=1;i<=NF;i++){split($i,a,"="); v[a[1]]=a[2]} dec[v["phase"] v["web"]]=" DECLINED(" v["reg"] ")"; delete v}
END{for(i=1;i<=n;i++){k=order[i]; printf "%s -> %-4s %s%s\n", line[k], col[k], det[k], dec[k]}}' "$1"
