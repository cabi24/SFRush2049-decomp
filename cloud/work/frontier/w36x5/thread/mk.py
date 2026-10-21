import sys, re
src=open('v/t0.c').read()
V={}
def v(name, *pairs, base=None):
    s=V[base] if base else src
    for a,b in pairs:
        assert a in s, (name, a[:60])
        s=s.replace(a,b)
    V[name]=s
loop_old='''    for (;;) {
        osRecvMesg(&D_801427A8, &msg, 1);
        type = ((tAudioMsg *)msg)->hdr.type;
        if (type == 1) {
            func_80098FB8();
        } else {
            _ProcessCommand(msg);
        }
    }'''
v('V1_cont',(loop_old,'''    for (;;) {
        osRecvMesg(&D_801427A8, &msg, 1);
        type = ((tAudioMsg *)msg)->hdr.type;
        if (type == 1) {
            func_80098FB8();
            continue;
        }
        _ProcessCommand(msg);
    }'''))
v('V2_notype',(loop_old,'''    for (;;) {
        osRecvMesg(&D_801427A8, &msg, 1);
        if (((tAudioMsg *)msg)->hdr.type == 1) {
            func_80098FB8();
        } else {
            _ProcessCommand(msg);
        }
    }'''))
v('V3_while',(loop_old,'''    while (1) {
        osRecvMesg(&D_801427A8, &msg, 1);
        type = ((tAudioMsg *)msg)->hdr.type;
        if (type == 1) {
            func_80098FB8();
        } else {
            _ProcessCommand(msg);
        }
    }'''))
v('V4_nolock',('    _LockAudio();\n    switch','    osRecvMesg(&D_80142728, 0, 1);\n    switch'),('    _FreeCmd(cmd);\n    _UnlockAudio();\n}','    cmd->hdr.used = 0;\n    osSendMesg(&D_80142728, 0, 0);\n}'))
for k in sys.argv[1:] or V:
    open('v/%s.c'%k,'w').write(V[k]); print(k)
