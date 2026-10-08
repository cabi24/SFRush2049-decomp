"""Focused guards for the image-A resource cache research packet."""
import shutil
import copy,hashlib,importlib.util,json,os,struct,subprocess,sys,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_resource_cache_20261006'
spec=importlib.util.spec_from_file_location('runtime_cache_native',PACKET/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
sys.path.insert(0,str(PACKET))
spec=importlib.util.spec_from_file_location('runtime_cache_verify',PACKET/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)

class RuntimeCachePacket(unittest.TestCase):
    def test_frozen_artifact_hashes_and_honest_claim(self):
        r=json.loads((PACKET/'verification.json').read_text())
        self.assertEqual(r['status'],'NONMATCH')
        self.assertEqual((r['comparison']['differing'],r['comparison']['total']),(5,91))
        self.assertEqual(r['function_bytes'],364)
        self.assertEqual(r['zero_alignment_bytes'],4)
        self.assertEqual(r['owned_data_bytes'],0)
        self.assertEqual(r['instruction_coverage'],[91,91,91])
        self.assertEqual(r['status'], 'NONMATCH')  # This packet makes no matching claim.
        for path,want in r['files'].items():
            self.assertEqual(hashlib.sha256((PACKET/path).read_bytes()).hexdigest(),want,path)

    def test_lookup_prefers_loaded_match_and_unloaded_reservation(self):
        rows=[(i,0,30+i) for i in range(16)];rows[0]=(1,0,-5);rows[3]=(2,0,7);rows[9]=(3,128,7)
        m=native.state(rows,99);out,ret,calls,route=native.oracle(m,7,2,-1,0)
        self.assertEqual((ret,route,len(calls)),(1,'hit',0));self.assertEqual(native.read(out,native.OUT,4),3)
        rows[9]=(3,0,50);m=native.state(rows,99);out,ret,calls,route=native.oracle(m,7,2,-1,0)
        self.assertEqual((ret,route,len(calls)),(1,'reload',2))
        self.assertEqual(native.read(out,native.RECORDS+36,4),0xffffffff)
        self.assertEqual(out[native.RECORDS+4],0)
        self.assertEqual(out[native.RECORDS+36+4],1)

    def test_decoder_and_extent_fail_closed(self):
        m=native.state([(0,0,30+i) for i in range(16)],0)
        for words in ([0xffffffff]*91,[0]*90,[0]*92):
            with self.assertRaises(AssertionError):native.run(words,m,0,0,0,0)
        p=subprocess.run([sys.executable,'-O',str(PACKET/'verify.py')],capture_output=True,text=True)
        self.assertNotEqual(p.returncode,0)
        self.assertIn('optimization is unsupported',p.stderr)
        p=subprocess.run([sys.executable,'-O',str(PACKET/'independent/mutants.py'),str(PACKET/'candidate.c')],capture_output=True,text=True)
        self.assertNotEqual(p.returncode,0)
        self.assertIn('optimization is unsupported',p.stderr)

    def test_path_only_metadata_is_portable(self):
        a=synthetic_elf();b=synthetic_elf(debug=b'/different/long/path.c\0')
        self.assertNotEqual(a,b);self.assertEqual(verify.portable_elf(a),verify.portable_elf(b))
        r=json.loads((PACKET/'verification.json').read_text())
        self.assertEqual(len(r['portability_controls']['source_paths']),8)
        for c in r['portability_controls']['source_paths'].values():self.assertTrue(all(c.values()))

    def test_nondebug_elf_sections_and_metadata_remain_bound(self):
        base=synthetic_elf();fingerprint=verify.portable_elf(base)
        for name in ('.text','.rel.text','.symtab','.strtab','.reginfo','.options','.data','.rodata','.bss'):
            self.assertNotEqual(fingerprint,verify.portable_elf(synthetic_elf({name:b'changed'})),name)
        for offset in (7,24,36,39):
            bad=bytearray(base);bad[offset]^=1;self.assertNotEqual(fingerprint,verify.portable_elf(bad))
        shoff=struct.unpack_from('>I',base,32)[0]
        for field in (1,2,3,6,7,8,9):
            bad=bytearray(base);at=shoff+2*40+4*field;value=struct.unpack_from('>I',bad,at)[0];struct.pack_into('>I',bad,at,value^1)
            self.assertNotEqual(fingerprint,verify.portable_elf(bad))
        with self.assertRaises(AssertionError):verify.portable_elf(synthetic_elf(debug_flags=2))

    def test_portable_receipt_excludes_historical_and_proven_debug_hashes(self):
        r=json.loads((PACKET/'verification.json').read_text());changed=copy.deepcopy(r)
        for field in ('object_sha256','mdebug_sha256'):changed[field]='path only'
        for c in changed['controls'].values():
            for field in ('object_sha256','mdebug_sha256'):c[field]='path only'
        self.assertEqual(verify.portable(r),verify.portable(changed))
        for field in r.keys()-{'object_sha256','mdebug_sha256','game_manifest','protected_manifest','scorer_sha256'}:
            altered=copy.deepcopy(r)
            if field in ('portable_elf','portability_controls','controls','files'):
                altered[field]['unexpected']='changed'
            else:altered[field]='changed'
            try:normal=verify.portable(altered)
            except (AssertionError,TypeError,KeyError):continue
            self.assertNotEqual(verify.portable(r),normal,field)
        for label,c in r['controls'].items():
            for field in c.keys()-{'object_sha256','mdebug_sha256'}:
                altered=copy.deepcopy(r);altered['controls'][label][field]='changed'
                self.assertNotEqual(verify.portable(r),verify.portable(altered),(label,field))

    def test_complete_frozen_replay(self):
        ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
        if not (ido/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
            self.skipTest('pinned IDO and MIPS GNU linker required')
        repo=Path(os.environ.get('RUSH_CACHE_INPUT_REPO',ROOT))
        output=ROOT/'build/runtime_a_cache/test-replay.json'
        p=subprocess.run([sys.executable,str(PACKET/'verify.py'),'--repo',str(repo),'--output',str(output)],cwd='/tmp',capture_output=True,text=True)
        self.assertEqual(p.returncode,0,p.stdout+p.stderr)
        self.assertEqual(verify.portable(json.loads(output.read_text())),verify.portable(json.loads((PACKET/'verification.json').read_text())))

def synthetic_elf(payloads=None,debug=b'/source.c\0',debug_flags=0):
    payloads=payloads or {}
    rows=[('',0,0,b''),('.shstrtab',3,0,b''),('.text',1,6,b'code'),('.rel.text',9,0,b'reloc'),('.symtab',2,0,b'symbol'),('.strtab',3,0,b'names'),('.reginfo',0x70000006,2,b'registers'),('.options',0x7000000d,0,b'options'),('.data',1,3,b'data'),('.rodata',1,2,b'constants'),('.bss',8,3,b'zero'),('.mdebug',0x70000005,debug_flags,debug)]
    names=b'\0'.join(n.encode() for n,_,_,_ in rows)+b'\0';rows[1]=('.shstrtab',3,0,names)
    data=bytearray(52);sections=[]
    for name,kind,flags,payload in rows:
        payload=payloads.get(name,payload);offset=len(data)
        if kind!=8:data.extend(payload)
        sections.append((names.index(name.encode()+b'\0'),kind,flags,0,offset,len(payload),0,0,1,0))
    at=len(data)
    for row in sections:data.extend(struct.pack('>10I',*row))
    data[:16]=b'\x7fELF\x01\x02\x01'+b'\0'*9
    struct.pack_into('>HHIIIIIHHHHHH',data,16,1,8,1,0,0,at,0x10000000,52,0,0,40,len(sections),1)
    return data

if __name__=='__main__':unittest.main()
