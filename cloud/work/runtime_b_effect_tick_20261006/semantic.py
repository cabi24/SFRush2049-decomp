#!/usr/bin/env python3
import argparse,ctypes,hashlib,json,os,struct,subprocess,tempfile,zlib
from pathlib import Path
from native import verify_host
HERE=Path(__file__).resolve().parent
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
TARGETS={'func_80390B10':(0x80390B10,552,'656cfd510c2a08aa09c0dbba417f7e6bf84781681746380fddabce66c455fef8'),
'func_80390D38':(0x80390D38,552,'e90f5ca0395a6429e0137efb62e2a8077600aff474bf298d5e47d98aec520355'),
'func_80390F60':(0x80390F60,988,'6d91e9692637c1d96f48b3a1afcc63d3348212a42baf882e7e71034426a30bb3')}
def native(reference):
    asset=subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    dec=zlib.decompressobj(-15);data=dec.decompress(asset[0xB6FEC4-0x283D0:])
    assert dec.eof and len(data)==43888
    code={}
    for fn,(a,n,h) in TARGETS.items():
        raw=data[a-0x8038A400:a-0x8038A400+n]
        assert hashlib.sha256(raw).hexdigest()==h
        code.update({a+4*j:w[0] for j,w in enumerate(struct.iter_unpack('>I',raw))})
    return code,[(0x803942B8,data[0x803942B8-0x8038A400:0x803942C0-0x8038A400])]
def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path);args=p.parse_args()
    code,tables=native(args.reference_root)
    with tempfile.TemporaryDirectory(prefix='effect-host-') as td:
        so=Path(td)/'effect.so'
        subprocess.run(['cc','-std=c89','-Wall','-Wextra','-Werror','-O2','-fPIC','-shared','-ffp-contract=off',str(HERE/'host.c'),'-o',str(so)],check=True)
        report=verify_host(code,tables,ctypes.CDLL(str(so)))
    print(json.dumps(report,indent=2))
    if args.output:args.output.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
