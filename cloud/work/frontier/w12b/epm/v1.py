OLD="        poly->flags = ~(1 << i) & poly->flags;"
V={}
P="        poly->flags = "
for n,e in enumerate(["(u16)~(1 << i) & poly->flags","poly->flags & (u16)~(1 << i)","~(u16)(1 << i) & poly->flags","(s16)~(1 << i) & poly->flags","poly->flags & (s16)~(1 << i)","~(s16)(1 << i) & poly->flags","(u16)(~(1 << i) & poly->flags)","(u16)(poly->flags & ~(1 << i))"]):
    V['t%d'%n]=P+e+";"
