"""romcmp.py <obj> name:off:romaddr:n ... : compare object .text slices with ROM words, masking reloc fields."""
import subprocess,struct,re,sys,tempfile,os
o=sys.argv[1]
img=open('build/game_code.bin','rb').read(); base=0x80086A50
t=tempfile.mktemp(); subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',o,t],check=True); txt=open(t,'rb').read(); os.unlink(t)
rel=subprocess.run(['mips-linux-gnu-objdump','-r','-j','.text',o],capture_output=True,text=True).stdout
masks={}
for l in rel.splitlines():
    m=re.match(r'([0-9a-f]{8}) (R_MIPS_\w+)',l)
    if m: masks[int(m.group(1),16)]={'R_MIPS_26':0xFC000000,'R_MIPS_HI16':0xFFFF0000,'R_MIPS_LO16':0xFFFF0000}.get(m.group(2),0xFFFFFFFF)
for spec in sys.argv[2:]:
    name,off,rom,n=spec.split(':'); off=int(off,16); rom=int(rom,16); n=int(n)
    bad=[]
    for i in range(n):
        a=struct.unpack('>I',txt[off+4*i:off+4*i+4])[0]; b=struct.unpack('>I',img[rom-base+4*i:rom-base+4*i+4])[0]
        mk=masks.get(off+4*i,0xFFFFFFFF)
        if (a&mk)!=(b&mk): bad.append((i,hex(a),hex(b)))
    print(name,'MATCH' if not bad else f'{len(bad)} words differ {bad[:4]}')
