# func_800C8918 round 1: busy-wait / END / START address forms and loop shapes.
import itertools, os, sys
out, srcf = sys.argv[1], sys.argv[2]
os.makedirs(out, exist_ok=True)
src = open(srcf).read()
head = src[:src.index('void func_800C8918')]
head = head.replace('Residual: the busy-wait START/END address webs.', 'Residual: the busy-wait START/END address webs (lane w14za round 1 variants).')
END = ['D_801439D8', '&D_80142DD8[128]', 'D_80142DD8 + 128', '(ShutdownMsg *)((u8 *)D_80142DD8 + 3072)']
START = ['D_80142DD8', '&D_80142DD8[0]']
LOOP = [
 # nested do/while, break (current)
 ('do {{ m = {S}; do {{ if (m->used) break; m++; }} while (m != end); }} while (m < end);'),
 # inner while with guard
 ('do {{ m = {S}; while (m != end) {{ if (m->used) break; m++; }} }} while (m < end);'),
 # inner for, outer while
 ('m = {S}; while (m < end) {{ for (; m != end; m++) {{ if (m->used) break; }} if (m == end) break; m = {S}; }}'),
 # inner do, outer for
 ('for (; ;) {{ m = {S}; do {{ if (m->used) break; m++; }} while (m != end); if (m == end) break; }}'),
]
VOL = ['volatile ShutdownMsg *m;', 'ShutdownMsg *m;']
n = 0
for e, s, l, v in itertools.product(END, START, LOOP, VOL):
    body = '''void func_800C8918(void) {
    %s
    ShutdownMsg *end;

    if (D_8011023C) {
        if (D_8011025C) {
            audio_effect_process(D_8011025C);
            D_8011025C = 0;
        }
        if (D_80110260) {
            audio_effect_process(D_80110260);
            D_80110260 = 0;
        }
        object_type1_create();
        object_type7_create();
        end = %s;
        %s
        D_8011023C = 0;
        init_wait_completion();
        audio_effect_process(D_80110244);
        audio_effect_process(D_80110248);
        audio_effect_process(D_80110270);
    }
}
''' % (v, e, l.format(S=s).replace('end', 'end'))
    open(os.path.join(out, 'v%04d.c' % n), 'w').write(head + body)
    n += 1
print(n)
