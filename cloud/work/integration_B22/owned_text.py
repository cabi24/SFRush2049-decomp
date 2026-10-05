"""Explicit original TU text boundaries and proven non-code zero fill.

A boundary belongs to a real SPLAT segment, survives C/asm conversion, and
never manufactures a C function or credits linker fill as C body bytes.
"""
import hashlib,re
from pathlib import Path

class TextBoundaryError(ValueError):
    pass

def _sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def _path(value):
    if not isinstance(value,str) or not re.fullmatch(r'[A-Za-z0-9_./-]+',value):
        raise TextBoundaryError('invalid boundary path')
    p=Path(value)
    if p.is_absolute() or '..' in p.parts:
        raise TextBoundaryError('boundary path must be repository-relative')
    return p.as_posix()

def _int(value):
    return int(value,0) if isinstance(value,str) else int(value)

def _symbols(repo):
    result={}
    for line in (Path(repo)/'symbol_addrs.us.txt').read_text().splitlines():
        m=re.match(r'\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x([0-9a-fA-F]+)\s*;',line)
        if m:result[m[1]]=int(m[2],16)
    return result

def _checked(repo,row,check_rom=True):
    from . import layout,lock
    repo=Path(repo);r=dict(row)
    for key in ('owner','function'):
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*',r[key]):
            raise TextBoundaryError('invalid boundary identifier')
    for key in ('tu','source','asm_authority'):
        r[key]=_path(r[key])
    for key in ('rom_start','vram_start','extent_bytes','function_offset','logical_body_bytes','padding_bytes','next_rom_start'):
        r[key]=_int(r[key])
    start=r['rom_start'];size=r['extent_bytes'];body_end=r['function_offset']+r['logical_body_bytes']
    if (min(start,r['function_offset'])<0 or r['logical_body_bytes']<=0 or r['padding_bytes']<=0 or
        any(r[k]%4 for k in ('extent_bytes','function_offset','logical_body_bytes','padding_bytes')) or
        body_end+r['padding_bytes']!=size or r['next_rom_start']!=start+size or
        r['vram_start']!=start+layout.VRAM_DELTA):
        raise TextBoundaryError('invalid logical body / zero-fill / original extent')
    subsegments,_=layout.parse_subsegments(repo/'splat.us.yaml')
    indices=[i for i,s in enumerate(subsegments) if s['off']==start]
    if len(indices)!=1 or indices[0]+1>=len(subsegments):
        raise TextBoundaryError('original boundary has no unique following SPLAT segment')
    i=indices[0];sub=subsegments[i]
    if subsegments[i+1]['off']!=r['next_rom_start']:
        raise TextBoundaryError('following SPLAT boundary changed')
    if sub['type']=='c':
        if 'src/'+sub['name']+'.c'!=r['tu']:
            raise TextBoundaryError('converted boundary belongs to another TU')
        object_rel='build/us/'+str(Path(r['tu']).with_suffix('.o'))
        r['converted']=True
    elif sub['type']=='asm':
        if sub['name']:
            raise TextBoundaryError('named assembly boundary requires separate proof')
        object_rel='build/us/asm/us/'+format(start,'X')+'.o'
        r['converted']=False
    else:
        raise TextBoundaryError('boundary is not a real code segment')
    r['input']=object_rel+'(.text)'
    symbols=_symbols(repo)
    aliases=r['aliases']
    if not aliases or len({a['symbol'] for a in aliases})!=len(aliases):
        raise TextBoundaryError('missing or duplicate authoritative boundary aliases')
    for a in aliases:
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*',a['symbol']) or not 0<=_int(a['offset'])<=size:
            raise TextBoundaryError('invalid boundary alias')
        if symbols.get(a['symbol'])!=r['vram_start']+_int(a['offset']):
            raise TextBoundaryError('authoritative boundary address changed: '+a['symbol'])
    if not any(a['symbol']==r['function'] and _int(a['offset'])==r['function_offset'] for a in aliases):
        raise TextBoundaryError('logical function lacks its authoritative entry alias')
    if not any(_int(a['offset'])==0 for a in aliases) or not any(_int(a['offset'])==size for a in aliases):
        raise TextBoundaryError('both original TU endpoints need authoritative aliases')
    if check_rom:
        rom=(repo/'baserom.us.z64').read_bytes();original=rom[start:start+size];padding=original[body_end:]
        if len(original)!=size or hashlib.sha256(original).hexdigest()!=r['retail_extent_sha256']:
            raise TextBoundaryError('original full text extent changed')
        if len(padding)!=r['padding_bytes'] or any(padding) or hashlib.sha256(padding).hexdigest()!=r['padding_sha256']:
            raise TextBoundaryError('original trailing bytes are not the proven zero fill')
    asm=repo/r['asm_authority']
    if _sha(asm)!=r['asm_authority_sha256']:
        raise TextBoundaryError('original endlabel authority changed')
    text=asm.read_text();marker='endlabel '+r['function']
    if text.count(marker)!=1:
        raise TextBoundaryError('logical endpoint is not unique')
    body,tail=text.split(marker)
    body_words=re.findall(r'/\*\s*[0-9a-fA-F]+\s+([0-9a-fA-F]{8})\s+([0-9a-fA-F]{8})\s*\*/',body)
    tail_words=re.findall(r'/\*\s*[0-9a-fA-F]+\s+([0-9a-fA-F]{8})\s+([0-9a-fA-F]{8})\s*\*/',tail)
    address=r['vram_start']+r['function_offset']
    expected_addresses=list(range(address,address+r['logical_body_bytes'],4))
    if [int(v,16) for v,w in body_words]!=expected_addresses or len(body_words)*4!=r['logical_body_bytes']:
        raise TextBoundaryError('original logical body endpoint/extent changed')
    if [int(v,16) for v,w in tail_words]!=list(range(address+r['logical_body_bytes'],r['vram_start']+size,4)) or any(int(w,16) for v,w in tail_words):
        raise TextBoundaryError('original assembly tail is not exactly the proven fill')
    if check_rom and b''.join(int(w,16).to_bytes(4,'big') for v,w in body_words)!=original[r['function_offset']:body_end]:
        raise TextBoundaryError('original logical body differs from retail')
    source=repo/r['source']
    if _sha(source)!=r['source_sha256'] or lock.body_sha(source,r['function'])!=r['body_sha256']:
        raise TextBoundaryError('proven canonical body/source changed')
    if source.read_text().splitlines()[0]!='/* flags: '+r['flags']+' */':
        raise TextBoundaryError('literal canonical flags changed')
    if r['converted']:
        from ..seeds.extract_candidates import extract_functions
        allowed = {a['symbol'] for a in aliases if _int(a['offset']) < size}
        live_text = (repo/r['tu']).read_text()
        if re.search(r'\b(?:asm|__asm__)\s*\(', live_text):
            raise TextBoundaryError('inline assembly cannot replace proven linker fill')
        if any(name not in allowed for name, _, _ in extract_functions((repo/r['tu']).read_text())):
            raise TextBoundaryError('live TU contains an unproven additional function')
        actual=lock.body_sha(repo/r['tu'],r['function'])
        if actual is not None and actual!=r['body_sha256']:
            raise TextBoundaryError('live boundary function differs from proven canonical body')
    return r

def boundaries(repo,check_rom=True):
    from .owned_data import load_registry
    registry=load_registry(repo);rows=registry.get('text_boundaries',[])
    if not isinstance(rows,list):raise TextBoundaryError('text_boundaries must be a list')
    result=[];owners=set();ranges=[];tus=set()
    for row in rows:
        r=_checked(repo,row,check_rom=check_rom)
        if r['owner'] in owners or r['tu'] in tus or any(r['rom_start']<end and start<r['next_rom_start'] for start,end in ranges):
            raise TextBoundaryError('duplicate/overlapping real text boundaries')
        owners.add(r['owner']);tus.add(r['tu']);ranges.append((r['rom_start'],r['next_rom_start']));result.append(r)
    return result

def rewrite_linker(text,repo):
    # Removing metadata restores the genuine original input, never stale fill.
    pattern=r'/\* ROM_TEXT_BOUNDARY_BEGIN ([A-Za-z0-9_./-]+\(\.text\)) \*/.*?/\* ROM_TEXT_BOUNDARY_END \1 \*/'
    text=re.sub(pattern,lambda m:m.group(1)+';',text,flags=re.S)
    for r in boundaries(repo):
        original=r['input']+';'
        if text.count(original)!=1:raise TextBoundaryError('boundary input missing or ambiguous')
        indent=re.search(r'(?m)^([ \t]*)'+re.escape(original),text).group(1)
        prefix='rom_text_'+r['owner'];logical=r['function_offset']+r['logical_body_bytes']
        lines=['/* ROM_TEXT_BOUNDARY_BEGIN '+r['input']+' */',prefix+'_START = .;',original,prefix+'_INPUT_END = .;',
               'ASSERT(ABSOLUTE('+prefix+'_START) == '+hex(r['vram_start'])+', "original text owner moved");',
               'ASSERT('+prefix+'_INPUT_END == '+prefix+'_START + '+hex(logical)+' || '+prefix+'_INPUT_END == '+prefix+'_START + '+hex(r['extent_bytes'])+', "unproven text owner extent");',
               *['ASSERT(ABSOLUTE('+a['symbol']+') == '+hex(r['vram_start']+_int(a['offset']))+', "original text alias moved");' for a in r['aliases']],
               'FILL(0x00000000);','. = '+prefix+'_START + '+hex(r['extent_bytes'])+';',prefix+'_END = .;',
               '/* ROM_TEXT_BOUNDARY_END '+r['input']+' */']
        text=text.replace(original,('\n'+indent).join(lines),1)
    return text

def package_paths(repo,tu):
    repo=Path(repo);rel=str(Path(tu).relative_to(repo)) if Path(tu).is_absolute() else str(tu)
    rows=[r for r in boundaries(repo) if r['tu']==rel]
    if not rows:return []
    paths={'rom_owned_data.json','splat.us.yaml','symbol_addrs.us.txt','Makefile','rush2049.us.ld','tools/conveyor/pipeline/owned_text.py','tools/conveyor/pipeline/owned_data.py','tools/conveyor/pipeline/layout.py'}
    for r in rows:paths.update((r['source'],r['asm_authority']))
    return [repo/p for p in sorted(paths)]

def accounting(repo):
    # Reporting needs the committed ASM/source/alias proofs, not a ROM upload.
    # Linker generation and promotion packaging always retain full ROM checks.
    return {(r['tu'],r['function']):r for r in boundaries(repo,check_rom=False)}


def dependency_paths(repo):
    """List inputs before validation, so changed invalid bodies force a refusal.

    Validation belongs to linker generation. Running it in make's shell path
    expansion would turn an invalid body into an empty dependency list.
    """
    from .owned_data import load_registry
    repo=Path(repo);rows=load_registry(repo).get('text_boundaries',[])
    if not isinstance(rows,list):raise TextBoundaryError('text_boundaries must be a list')
    paths=set()
    if rows:paths.update(('splat.us.yaml','symbol_addrs.us.txt','baserom.us.z64'))
    for r in rows:
        paths.update(_path(r[key]) for key in ('source','asm_authority'))
        tu=_path(r['tu']);whole='asm/us/'+format(_int(r['rom_start']),'X')+'.s'
        paths.update(p for p in (tu,whole) if (repo/p).is_file())
    return sorted(paths)


if __name__=='__main__':
    import argparse
    from ..seeds.extract_candidates import REPO
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command',choices=('inputs',))
    args=parser.parse_args()
    print(' '.join(dependency_paths(REPO)))
