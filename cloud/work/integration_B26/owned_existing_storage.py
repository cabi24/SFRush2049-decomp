"""Reviewed atomic source/data/BSS activation for an already converted real TU.

This proposal does not commit or mutate a coordinator database. The caller must
provide current authoritative target identities, independently review the strict
proof digest, and wrap successful publication in the standard DB/git transaction.
The mandatory gate must force regeneration/rebuild and original ROM SHA checks.
"""
from pathlib import Path
import hashlib,json

REGISTRY='rom_owned_data.json'
FLAGS='-g0 -O1 -mips2 -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul'

# Explicit reviewed profiles, never a caller-supplied compiler/extent whitelist.
PROFILES={
    'timer_services':dict(tu='src/rom/lib_cc50.c',flags=FLAGS,
        source_sha256='8ea97f1232f2b872f37530130da9d8ccca45c26ba87a1ec9f03b317c61a22bd3',
        text_bytes=1120,data_bytes=16,bss_start=0x80037c30,bss_bytes=64,
        data_offset=0x1cff0,
        members=('dll_remove','dll_init','dll_update','dll_reschedule','dll_insert','dll_get_priority'),
        new_members=('dll_init',)),
    'sdk_initialize':dict(tu='src/rom/lib_8a80.c',
        flags='-g0 -O1 -mips2 -G 0 -non_shared -Xcpluscomm',
        source_sha256='7606f20fc4e2459c6de43e5be046a9389030c5c7a93db6548bf9729003e5fe80',
        text_bytes=848,data_bytes=32,bss_start=0x800367d0,bss_bytes=16,
        data_offset=0x1cf60,members=('__osInitialize_common','__osPiReadDeviceType'),
        new_members=('__osInitialize_common',)),
}


def profile(row):
    value=PROFILES.get(row.get('owner'))
    if (value is None or row.get('activation_mode')!='existing_tu' or
        row.get('tu')!=value['tu'] or row.get('flags')!=value['flags'] or
        row.get('source_sha256')!=value['source_sha256'] or
        tuple(row.get('members',()))!=value['members'] or
        tuple(row.get('new_members',()))!=value['new_members'] or
        int(row.get('vram_start','0'),0)!=value['bss_start'] or
        int(row.get('size','0'),0)!=value['bss_bytes'] or
        int(row.get('data_slot',{}).get('offset','0'),0)!=value['data_offset'] or
        int(row.get('data_slot',{}).get('size','0'),0)!=value['data_bytes']):
        raise ValueError('unsupported compiler recipe or extent for reviewed existing-TU profile')
    return value



def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _row(repo,owner):
    from .owned_data import load_registry
    rows=[r for r in load_registry(repo)['storage_blocks'] if r['owner']==owner]
    if len(rows)!=1 or rows[0].get('activation_mode')!='existing_tu':
        raise ValueError('missing reviewed existing-TU storage owner')
    return rows[0]


def package_paths(repo,row):
    repo=Path(repo)
    # No recursion into owned_data.promotion_paths / owned_storage.package_paths.
    paths={REGISTRY,row['data_slot']['source'],'rom_owned_storage.ld','src/rom/storage_overrides.mk','Makefile',
           'rush2049.us.ld','splat.us.yaml','matched.lock.json',row['tu'],row['source'],
           row['tu_before_source'],row['strict_proof'],row['context_proof'],row['linked_proof'],
           row['startup_proof'],'asm/us/1000.s','baserom.us.z64',
           'tools/conveyor/pipeline/owned_existing_storage.py','tools/conveyor/pipeline/owned_data.py',
           'tools/conveyor/pipeline/owned_storage.py','tools/conveyor/pipeline/owned_text.py',
           'tools/conveyor/seeds/extract_candidates.py','tools/asm-processor/asm_processor.py'}
    paths.update(json.loads((repo/row['context_proof']).read_text()))
    return sorted((repo/p for p in paths),key=str)



def assert_existing_context(repo,row,check_rom=False):
    """Standalone active-owner guard, including immutable revert prerequisites."""
    repo=Path(repo)
    if row['flags']!=profile(row)['flags']:
        raise ValueError('active existing-TU compiler recipe changed')
    for field in ('source','tu_before_source','strict_proof','context_proof','linked_proof','startup_proof'):
        if sha(repo/row[field])!=row['tu_before_sha256' if field=='tu_before_source' else field+'_sha256']:
            raise ValueError('owned source/proof/revert prerequisite changed: '+field)
    for rel,digest in json.loads((repo/row['context_proof']).read_text()).items():
        if sha(repo/rel)!=digest:
            raise ValueError('active owned compiler context changed: '+rel)
    startup=json.loads((repo/row['startup_proof']).read_text())
    if sha(repo/'asm/us/1000.s')!=startup['entry_asm_sha256']:
        raise ValueError('original startup assembly authority changed')
    if check_rom and hashlib.sha256((repo/'baserom.us.z64').read_bytes()[0x1000:0x1038]).hexdigest()!=startup['entry_code_sha256']:
        raise ValueError('original startup authority changed')

def validate(repo,row,reviewed_digest,trusted_targets):
    from . import lock
    from .owned_data import rom_slots,load_registry
    from .owned_storage import storage_blocks
    repo=Path(repo);approved=profile(row)
    if row['flags']!=approved['flags'] or sha(repo/row['source'])!=row['source_sha256'] or sha(repo/row['tu_before_source'])!=row['tu_before_sha256']:
        raise ValueError('complete source/baseline/flags changed')
    if sha(repo/row['strict_proof'])!=reviewed_digest or reviewed_digest!=row['strict_proof_sha256']:
        raise ValueError('independent canonical proof is not the reviewed immutable record')
    contexts=json.loads((repo/row['context_proof']).read_text())
    if sha(repo/row['context_proof'])!=row['context_proof_sha256'] or any(sha(repo/p)!=digest for p,digest in contexts.items()):
        raise ValueError('current complete compiler context changed; rescore')
    proof=json.loads((repo/row['strict_proof']).read_text())
    if len(proof)!=len(row['members']) or {r['function'] for r in proof}!=set(row['members']):
        raise ValueError('proof must cover every real module member')
    if set(row['new_members']) & set(row['prior_locks']) or set(row['new_members'])|set(row['prior_locks'])!=set(row['members']):
        raise ValueError('prior/new member partition differs')
    entries=lock.load_lock(repo/'matched.lock.json')
    for result in proof:
        fn=result['function']
        if (result['strict_score']!=0 or result['words']<=0 or result['flags']!=approved['flags'] or
            result['source_sha256']!=row['source_sha256'] or
            result['protocol']!='unchanged canonical scorer settings, function-scoped objdump; raw unlinked object slices, no masks or linked substitutions' or
            trusted_targets.get(fn)!=(result['target_sha256'],result['words'])):
            raise ValueError('strict protocol/source/current target differs: '+fn)
        if fn in row['prior_locks']:
            prior=row['prior_locks'][fn]
            if entries.get(row['tu']+':'+fn)!=prior or prior['body_sha256']!=lock.body_sha(repo/row['source'],fn):
                raise ValueError('accepted body or historical lock changed: '+fn)
    linked=json.loads((repo/row['linked_proof']).read_text())
    if sha(repo/row['linked_proof'])!=row['linked_proof_sha256'] or {(r['section'],r['bytes'],r['differences']) for r in linked}!={('.text',approved['text_bytes'],0),('.data',approved['data_bytes'],0)}:
        raise ValueError('complete module linked proof differs')
    startup=json.loads((repo/row['startup_proof']).read_text())
    start=int(row['vram_start'],0);end=start+int(row['size'],0)
    if (sha(repo/row['startup_proof'])!=row['startup_proof_sha256'] or
        int(startup['owned_start'],0)!=start or int(startup['owned_end_exclusive'],0)!=end or
        not int(startup['clear_start'],0)<=start<end<=int(startup['clear_end_exclusive'],0) or
        sha(repo/'asm/us/1000.s')!=startup['entry_asm_sha256'] or
        hashlib.sha256((repo/'baserom.us.z64').read_bytes()[0x1000:0x1038]).hexdigest()!=startup['entry_code_sha256']):
        raise ValueError('original startup clearing does not cover complete storage')
    storage_blocks(repo);rom_slots(repo)
    registry=load_registry(repo)
    slot=row['data_slot']
    if slot.get('storage_owner')!=row['owner'] or slot['tu']!=row['tu'] or slot['owner_section']!='.data':
        raise ValueError('data slot is not the genuine coupled pointer owner')
    data=(repo/slot['source']).read_bytes();off=int(slot['offset'],0);size=int(slot['size'],0)
    if hashlib.sha256(data[off:off+size]).hexdigest()!=slot['sha256']:
        raise ValueError('original pointer slot changed')
    if not row.get('active') and any(r['owner']==slot['owner'] for r in registry['rom_slots']):
        raise ValueError('inactive source must retain original container bytes')


def register(repo,row,reviewed_digest,trusted_targets):
    from .owned_data import load_registry
    from . import owned_storage
    repo=Path(repo);registry=load_registry(repo)
    if row.get('active') or row.get('activation_mode')!='existing_tu':
        raise ValueError('registration must preserve inactive existing C baseline')
    if any(r['owner']==row['owner'] or r['tu']==row['tu'] for r in registry['storage_blocks']):
        raise ValueError('existing storage owner/TU')
    if sha(repo/row['tu'])!=row['tu_before_sha256']:
        raise ValueError('current baseline TU differs')
    old=(repo/REGISTRY).read_bytes()
    registry['storage_blocks'].append(row);(repo/REGISTRY).write_text(json.dumps(registry,indent=2)+'\n')
    try:
        validate(repo,row,reviewed_digest,trusted_targets)
        owned_storage.generate(repo)
    except BaseException:
        (repo/REGISTRY).write_bytes(old);owned_storage.generate(repo);raise


def transition(repo,owner,activate,*,reviewed_digest,trusted_targets,gate,publish=None):
    """Gate-aware package transaction; retain every prior C body/lock on revert.

    `gate(repo, paths)` must return True ONLY after original-ROM forced build
    validation. If activation fails, the same gate must verify the restored
    baseline. Missing file snapshots remain real rollback deletions.
    """
    from . import lock,owned_storage,owned_data
    repo=Path(repo);row=_row(repo,owner)
    if bool(row.get('active'))==bool(activate):raise ValueError('owner already in requested state')
    validate(repo,row,reviewed_digest,trusted_targets)
    expected=row['tu_before_sha256'] if activate else row['source_sha256']
    if sha(repo/row['tu'])!=expected:raise ValueError('live owned/baseline TU changed')
    if not callable(gate):raise ValueError('mandatory forced ROM gate missing')
    paths=package_paths(repo,row);saved=owned_storage._snapshot(paths)
    registry=owned_data.load_registry(repo);target_row=next(r for r in registry['storage_blocks'] if r['owner']==owner)
    entries=lock.load_lock(repo/'matched.lock.json');prior_entries=dict(entries)
    yaml_before=(repo/'splat.us.yaml').read_bytes()
    try:
        (repo/row['tu']).write_bytes((repo/(row['source'] if activate else row['tu_before_source'])).read_bytes())
        target_row['active']=bool(activate)
        if activate:registry['rom_slots'].append(row['data_slot'])
        else:registry['rom_slots']=[r for r in registry['rom_slots'] if r['owner']!=row['data_slot']['owner']]
        (repo/REGISTRY).write_text(json.dumps(registry,indent=2)+'\n');owned_storage.generate(repo)
        ld=repo/'rush2049.us.ld';ld.write_text(owned_data.rewrite_linker(ld.read_text(),repo))
        if not gate(repo,paths):raise ValueError('joint storage/data original-ROM gate failed')
        if (repo/'splat.us.yaml').read_bytes()!=yaml_before:raise ValueError('existing C SPLAT owner changed')
        for fn in row['new_members']:
            key=row['tu']+':'+fn
            if activate:
                result=next(r for r in json.loads((repo/row['strict_proof']).read_text()) if r['function']==fn)
                entries[key]=dict(body_sha256=lock.body_sha(repo/row['tu'],fn),target_id=fn,
                    flagset=row['flags'],verified='rom-sha1',toolkit_sha=None,storage_owner=owner,
                    strict_proof_sha256=reviewed_digest,raw_word_diff=result['raw_word_diff'],
                    target_o_sha=result['target_sha256'],strict_words=result['words'])
            else:entries.pop(key,None)
        for fn,entry in row['prior_locks'].items():
            if entries.get(row['tu']+':'+fn)!=entry:raise ValueError('historical body lock not preserved')
        lock.save_lock(entries,repo/'matched.lock.json')
        # A standard coordinator callback can check all locks and publish DB/git
        # within this rollback scope; a callback exception restores the package.
        if publish is not None:
            publish(repo,row,activate,paths)
        return dict(owner=owner,active=activate,new_members=list(row['new_members']),prior_locks_preserved=True)
    except BaseException:
        owned_storage._restore(saved)
        if not gate(repo,paths):raise ValueError('package restored locally but restored original-ROM gate failed')
        raise


if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command',choices=['assert'])
    parser.add_argument('owner')
    args=parser.parse_args()
    from . import owned_storage
    repo=Path.cwd();row=_row(repo,args.owner)
    if not row.get('active'):raise ValueError('build assertion requires active complete owner')
    owned_storage.assert_context(repo,row)
