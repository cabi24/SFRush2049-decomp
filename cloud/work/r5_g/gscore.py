import subprocess,re,sys,shutil,os,json
ROOT='/home/user/SFRush2049-decomp'
def run(gdir, fn, extra=()):
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',gdir,fn,'--as1=-r4300_mul','--hi','0',*extra],capture_output=True,text=True)
    m=re.search(r'aligned differing rows: (\d+) size (\d+) / (\d+)',r.stdout)
    return (int(m.group(1)),int(m.group(2))) if m else (9999,r.stdout[-200:]+r.stderr[-300:])
def zb(gdir, fn):
    r=subprocess.run(['python3',ROOT+'/cloud/work/tools/zbuild.py',gdir,'--as1=-r4300_mul'],capture_output=True,text=True).stdout
    for l in r.splitlines():
        if l.strip().startswith(fn+' '):
            m=re.search(r'size\s+(\d+)/(\d+)\s+(MATCH|(\d+)/(\d+) words)',l)
            return l.strip()
    return r[-300:]
def spoffs(gdir, fn):
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',gdir,fn,'--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
    ours=[l.split('|',1)[1] for l in r.splitlines() if '|' in l]
    offs=sorted(set(int(x) for l in ours for x in re.findall(r'(-?\d+)\(sp\)',l)))
    m=re.search(r'addiu sp,sp,-(\d+)',r)
    return offs
