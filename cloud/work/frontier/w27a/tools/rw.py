import sys,struct
d=open('/home/cburnes/projects/rush2049-decomp/build/game_code.bin','rb').read()
for a in sys.argv[1:]:
    a=int(a,16); w=d[a-0x80086A50:a-0x80086A50+4]
    print(hex(a), w.hex(), struct.unpack('>f',w)[0])
