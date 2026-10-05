import itertools
head=open('v0.c').read().split('void func_800E8F10')[0]
def body(loop, ratio_local, end_local, path_local, arr_first):
    d=[]
    if arr_first: d.append("    Vec3 delta,result;")
    d.append("    s16 previous,track;")
    d.append("    s32 next,coordinate;")
    if path_local: d.append("    Path8 *path;")
    d.append("    Point *start%s;" % (",*end" if end_local else ""))
    d.append("    f32 amount,length%s;" % (",ratio" if ratio_local else ""))
    if not arr_first: d.append("    Vec3 delta,result;")
    P = "path->points" if path_local else "D_8012E5E8[track].points"
    E = "end" if end_local else "(&%s[next])" % P
    core = """            next=func_800B9338(previous,track);
            start=&%s[previous];
%s            delta[0]=%s->x-start->x;
            delta[1]=%s->y-start->y;
            delta[2]=%s->z-start->z;
            length=sqrtf(delta[2]*delta[2]+(delta[0]*delta[0]+delta[1]*delta[1]));
""" % (P, ("            end=&%s[next];\n" % P) if end_local else "", E,E,E)
    adv = """                D_80139308[slot]+=length;
                previous=next;
                D_801391E8[slot]=next;
                amount-=length;
"""
    if loop==0:
        L="        for(;;) {\n"+core+"            if(length<=amount) {\n"+adv+"            } else break;\n        }\n"
    elif loop==1:
        L="        while(1) {\n"+core+"            if(length>amount) break;\n"+adv.replace("    ","",1).replace("\n                ","\n            ")+"        }\n"
    else:
        L="    loop:\n"+core.replace("            ","        ")+"        if(length<=amount) {\n"+adv.replace("                ","            ")+"            goto loop;\n        }\n"
    R = "ratio" if ratio_local else "(amount/length)"
    tail = ("        ratio=amount/length;\n" if ratio_local else "") + """        result[0]=start->x+%s*delta[0];
        result[1]=start->y+%s*delta[1];
        result[2]=start->z+%s*delta[2];
        result[1]+=10.0f;
        result[0]-=position[0];
        result[1]-=position[1];
        result[2]-=position[2];
        D_80152720[slot]=0.0f;
        func_800E8D50(object,position,matrix,result);
""" % (R,R,R)
    s = "void func_800E8F10(s16 update,Object *object,f32 *position,Mat3 *matrix)\n{\n    s8 slot=object->slot;\n" + "\n".join(d) + """
    if(!update) {
        coordinate=(s32)position[0];
        D_801391E8[slot]=coordinate;
        D_80138668[slot]=coordinate;
        D_80138878[slot]=object->other828;
        D_801392B8[slot]=D_801543CC;
        D_80139308[slot]=0.0f;
    } else {
        if(D_801392B8[slot]>D_801543CC) D_801392B8[slot]=0.0f;
        amount=(D_801543CC-D_801392B8[slot])*(100.0f+(object->speed>>2)*D_801244C4)-D_80139308[slot];
        previous=D_801391E8[slot];
        track=D_80138878[slot];
""" + ("        path=&D_8012E5E8[track];\n" if path_local else "") + L + tail + "    }\n}\n"
    return head+s
for c in itertools.product(range(3),[0,1],[0,1],[0,1],[0,1]):
    open('s1/v_%d%d%d%d%d.c'%c,'w').write(body(*c))
