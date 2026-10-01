"""Record coordinator-reviewed, independently executed whole-module strict proof.

This records evidence; it neither executes a scorer nor accepts an unaudited
zero declaration. Caller must pin the immutable proof SHA after reviewing its
independent execution. Current source/body/header/target identities are checked.
No pool protocol, scorer, source or ownership metadata is changed.
"""
import argparse, hashlib, json, sqlite3, sys, time
from pathlib import Path

def sha(data):
    return hashlib.sha256(data).hexdigest()

def validated_entries(repo, record, proof_path, proof_sha256, data):
    sys.path.insert(0, str(repo))
    from tools.conveyor.pipeline import lock
    from tools.conveyor.seeds.context import resolve_headers
    proof_bytes = Path(proof_path).read_bytes()
    if sha(proof_bytes) != proof_sha256:
        raise ValueError('reviewed independent proof hash differs')
    proof = json.loads(proof_bytes)
    source = repo / record['source']
    if sha(source.read_bytes()) != record['source_sha256'] or proof['source_sha256'] != record['source_sha256']:
        raise ValueError('complete proven source changed')
    if proof['flags'] != record['flags'] or source.read_text().splitlines()[0] != '/* flags: ' + record['flags'] + ' */':
        raise ValueError('literal flags differ')
    if not (proof['stack_differences'] is True and proof['function_scoped'] is True and proof['algorithm'] == 'difflib' and proof['ign_branch_targets'] is True):
        raise ValueError('proof is outside the canonical strict protocol')
    headers = resolve_headers(source, repo, ['include', 'include/PR', 'src/rom'])
    for item in record['context_files']:
        rel = Path(item['path']); snapshot = Path(item['source'])
        if rel.is_absolute() or snapshot.is_absolute() or '..' in rel.parts or '..' in snapshot.parts:
            raise ValueError('unsafe header context path')
        content = (repo / snapshot).read_bytes()
        if sha(content) != item['sha256'] or str(rel) not in headers:
            raise ValueError('pinned proposed header changed or absent from closure')
        headers[str(rel)] = content.decode()
    if proof['headers'] != {rel: sha(text.encode()) for rel, text in sorted(headers.items())}:
        raise ValueError('current resolved header closure differs; independently recompile')
    rows = proof['results']
    if len(rows) != len(record['members']) or {r['function'] for r in rows} != set(record['members']):
        raise ValueError('proof must cover each real member exactly once')
    linked_path = repo / record['proofs'][1]
    startup_path = repo / record['proofs'][2]
    linked = json.loads(linked_path.read_text())
    startup = json.loads(startup_path.read_text())
    if not (linked['source_sha256'] == record['source_sha256'] and linked['equal'] is True and linked['raw_word_diff'] == 0 and linked['bytes'] == 4 * sum(r['words'] for r in rows) and linked['linked_text_sha256'] == linked['retail_text_sha256']):
        raise ValueError('complete resolved module proof differs')
    start = int(record['vram_start'], 0); end = start + int(record['size'], 0)
    if not (startup['covered'] is True and int(startup['owned_start'], 0) == start and int(startup['owned_end_exclusive'], 0) == end and int(startup['clear_start'], 0) <= start and int(startup['clear_end_exclusive'], 0) >= end):
        raise ValueError('complete storage startup proof differs')
    result = {}
    with sqlite3.connect('file:' + str(Path(data) / 'conveyor.db') + '?mode=ro', uri=True) as conn:
        for row in rows:
            fn = row['function']; current = conn.execute('SELECT target_o_sha,population,insn_count FROM n64_target WHERE target_id=?', (fn,)).fetchone()
            if not current or current[1] == 'extracted' or current[0] != row['target_sha256']:
                raise ValueError('authoritative target changed or ineligible: ' + fn)
            if row['strict_score'] != 0 or row['words'] <= 0 or row['words'] != current[2] or row['inventory_insn_count'] != current[2]:
                raise ValueError('nonzero, empty or wrong-extent strict proof: ' + fn)
            body = lock.body_sha(source, fn)
            if not body or body != row['body_sha256']:
                raise ValueError('canonical body hash changed: ' + fn)
            result[record['source'] + ':' + fn] = dict(body_sha256=body, target_id=fn,
                flagset=record['flags'], verified='score0', toolkit_sha=None,
                verified_at=time.strftime('%Y-%m-%d'), verification_protocol='canonical-function-scoped-complete-storage-module',
                verification_proof_sha256=proof_sha256, source_sha256=record['source_sha256'],
                target_o_sha=row['target_sha256'], strict_words=row['words'], raw_word_diff=row['raw_word_diff'],
                context_sha256=sha(json.dumps(proof['headers'], sort_keys=True).encode()),
                linked_proof_sha256=sha(linked_path.read_bytes()), startup_proof_sha256=sha(startup_path.read_bytes()))
    return result

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=Path.cwd())
    parser.add_argument('--record', type=Path, required=True)
    parser.add_argument('--proof', type=Path, required=True)
    parser.add_argument('--proof-sha256', required=True, help='explicit coordinator-reviewed immutable proof hash')
    parser.add_argument('--data', type=Path, default=Path.home() / '.conveyor')
    parser.add_argument('--output', type=Path, required=True, help='explicit lock file path; dry-run scratch path recommended first')
    args = parser.parse_args()
    updates = validated_entries(args.repo.resolve(), json.loads(args.record.read_text()), args.proof, args.proof_sha256, args.data)
    entries = json.loads(args.output.read_text()) if args.output.exists() else {}
    for key, value in updates.items():
        if key in entries and entries[key] != value:
            raise ValueError('refusing to overwrite existing lock: ' + key)
        entries[key] = value
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(entries, indent=2, sort_keys=True) + '\n')
    print('Recorded ' + str(len(updates)) + ' independently strict-verified owner body locks; raw counts retained.')

if __name__ == '__main__':
    main()
