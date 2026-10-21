BASE = 'work/usc_padded'
P = "    u8 padB[52];"
VARIANTS = {}
for n in range(40, 61, 2):
    VARIANTS['pad%d' % n] = [(P, "    u8 padB[%d];" % n)]
