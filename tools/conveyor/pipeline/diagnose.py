"""Read-only workbench diagnosis; reports never authorize a splice.

python3 -m tools.conveyor.pipeline.diagnose one FUNCTION [--source FILE.c]
python3 -m tools.conveyor.pipeline.diagnose triage [--limit N]
"""
import argparse
from collections import Counter
from datetime import date
import hashlib
import io
import json
from pathlib import Path
import re
import shlex
import sqlite3
import subprocess
import sys
import tarfile

from . import cloud_worklist, ipa

REPO = Path(__file__).resolve().parents[3]
TOOLKIT = '796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
DEFAULT_FLAGS = cloud_worklist.DEFAULT_FLAGS
NAME = re.compile(r'^[A-Za-z_][A-Za-z_0-9]*$')


class ReadStore:
    """The source selector needs get(), never a writable BlobStore."""
    def __init__(self, root):
        self.root = Path(root)

    def get(self, sha):
        if not sha or not re.fullmatch(r'[0-9a-f]{64}', sha):
            return None
        path = self.root / sha
        if not path.is_file():
            return None
        if hashlib.sha256(path.read_bytes()).hexdigest() != sha:
            raise ValueError('blob SHA-256 mismatch: ' + sha)
        return path


def summary(document):
    """Use positional word counts, retaining raw and relocation-aware counts.

    Neither count is the project's strict linked-image scorer. Ownership is
    heuristic unless the workbench explicitly reports trace evidence.
    """
    comparison, view = document['comparison'], document['view']
    target_frame, candidate_frame = view.get('target_frame_size'), view.get('candidate_frame_size')
    lever = document.get('lever') or {}
    return {
        'function': view.get('symbol') or comparison.get('symbol'),
        'words_differing': comparison['word_mismatches'],
        'strict_words_differing': comparison['raw_word_mismatches'],
        'target_words': comparison['target_instructions'],
        'candidate_words': comparison['candidate_instructions'],
        'verdict': view['verdict'],
        'frame_delta': None if target_frame is None or candidate_frame is None else candidate_frame - target_frame,
        'lanes': [{k: lane.get(k) for k in ('class', 'slot', 'rotation')}
                  for lane in view.get('lanes', [])],
        'owning_pass': document.get('owning_pass', view.get('owning_pass', 'unknown')),
        'ownership_basis': document.get('ownership_basis', view.get('ownership_basis', 'unknown')),
        'lever': lever.get('edit_family') or lever.get('lever_class') or 'none',
    }


def run_workbench(target, candidate, function, objdump):
    proc = subprocess.run([sys.executable, str(REPO / 'tools/workbench.py'),
                           'diagnose', str(target), str(candidate), '--function', function,
                           '--objdump', objdump, '--json'], capture_output=True, text=True)
    # A non-exact diagnosis may return nonzero: the JSON document is authoritative.
    try:
        document = json.loads(proc.stdout)
        summary(document)
    except (ValueError, KeyError, TypeError) as exc:
        raise RuntimeError('workbench failed: ' + (proc.stderr or proc.stdout)[-2000:]) from exc
    return document


# A private directory per request; no fixed shared builder staging, no renamed
# sections, and only compiler stdout/stderr captured inside the returned tar.
REMOTE = r'''
import io,json,os,pathlib,subprocess,sys,tarfile,tempfile
with tempfile.TemporaryDirectory(prefix='rush-diagnose-') as work:
    root=pathlib.Path(work)
    with tarfile.open(fileobj=io.BytesIO(sys.stdin.buffer.read()),mode='r:') as archive:
        for member in archive:
            if not member.isfile() or '/' in member.name or member.name in ('.','..'):
                raise ValueError('invalid input member')
            (root/member.name).write_bytes(archive.extractfile(member).read())
    manifest=json.loads((root/'manifest.json').read_text())
    toolkit=pathlib.Path.home()/'rush2049/cache/toolkits'/manifest['toolkit']
    failures={}
    for job in manifest['jobs']:
        name=job['function']
        result=subprocess.run([str(toolkit/'ido/cc'),'-c',*job['flags'],
            '-I',str(toolkit/'shim'),str(root/(name+'.c')),'-o',str(root/(name+'.o'))],
            stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        if result.returncode or not (root/(name+'.o')).is_file():
            failures[name]=(result.stdout+result.stderr).decode(errors='replace')[-4000:] or 'compiler produced no object'
            (root/(name+'.o')).unlink(missing_ok=True)
    (root/'failures.json').write_text(json.dumps(failures))
    with tarfile.open(fileobj=sys.stdout.buffer,mode='w|') as archive:
        archive.add(root/'failures.json',arcname='failures.json')
        for job in manifest['jobs']:
            obj=root/(job['function']+'.o')
            if obj.is_file(): archive.add(obj,arcname=obj.name)
'''


def compile_batch(jobs, output, builder='watchman2', toolkit=TOOLKIT, runner=subprocess.run):
    """Compile mixed flagsets as normal objects with one SSH round trip."""
    if not jobs:
        return {}, {}
    manifest = {'toolkit': toolkit, 'jobs': []}
    payload = io.BytesIO()
    with tarfile.open(fileobj=payload, mode='w') as archive:
        def add(name, content):
            entry = tarfile.TarInfo(name)
            entry.size = len(content)
            archive.addfile(entry, io.BytesIO(content))
        for job in jobs:
            name = job['function']
            if not NAME.fullmatch(name):
                raise ValueError('invalid function name: ' + name)
            manifest['jobs'].append({'function': name, 'flags': shlex.split(job['flags'])})
            add(name + '.c', job['source'].encode())
        add('manifest.json', json.dumps(manifest).encode())
    command = 'python3 -c ' + shlex.quote(REMOTE)
    proc = runner(['ssh', '-o', 'BatchMode=yes', '-o', 'ConnectTimeout=15', builder, command],
                  input=payload.getvalue(), capture_output=True, timeout=600)
    if proc.returncode:
        raise RuntimeError('builder failed: ' + proc.stderr.decode(errors='replace')[-2000:])
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    objects = {}
    with tarfile.open(fileobj=io.BytesIO(proc.stdout), mode='r:') as archive:
        failures = json.load(archive.extractfile('failures.json'))
        for job in jobs:
            name = job['function']
            if name in failures:
                continue
            try:
                data = archive.extractfile(name + '.o').read()
            except KeyError:
                failures[name] = 'builder returned no object'
                continue
            path = output / (name + '.o')
            path.write_bytes(data)
            objects[name] = path
    return objects, failures


def batches(items, size):
    if size < 1:
        raise ValueError('batch size must be positive')
    for offset in range(0, len(items), size):
        yield items[offset:offset + size]


def select_jobs(conn, store, function=None, source=None, flags=None, near_miss=None):
    near_miss = Path(near_miss or REPO / 'cloud/work/near-miss')
    if function:
        rows = conn.execute('SELECT t.*, f.flagset FROM n64_target t LEFT JOIN function_status f USING(target_id) WHERE t.target_id=?', (function,)).fetchall()
        if not rows:
            raise ValueError('unknown target: ' + function)
    else:
        if not ipa.MEMBERS_JSON.is_file():
            raise ValueError('ABI selection requires build/ipa_members.json; run ipa scan first')
        rows = conn.execute("SELECT t.*, f.flagset FROM n64_target t JOIN function_status f USING(target_id) WHERE t.population='extracted' AND f.status IN ('unmatched','seeded','in_search','candidate_identified')").fetchall()
        member_document = json.loads(ipa.MEMBERS_JSON.read_text())
        if not isinstance(member_document.get('members'), list):
            raise ValueError('invalid IPA scan; run ipa scan first')
        members = set(member_document['members'])
        lock = json.loads((REPO / 'blob_matched.lock.json').read_text())
        rows = [row for row in rows if row['target_id'] not in members and row['target_id'] not in lock]
    jobs = []
    for row in rows:
        name = row['target_id']
        path = Path(source) if source else near_miss / name / 'base.c'
        if path.is_file():
            text, origin = path.read_text(), str(path.relative_to(REPO)) if path.is_relative_to(REPO) else str(path)
        else:
            if source:
                raise ValueError('source does not exist: ' + str(path))
            text, origin = cloud_worklist._best_source(conn, store, name)
        if not text:
            continue
        target = store.get(row['target_o_sha'])
        if target is None:
            raise ValueError('missing target object: ' + name)
        text = cloud_worklist.expand_shim(text)
        first = re.fullmatch(r'/\* flags: (.*?) \*/', text.splitlines()[0]) if text.splitlines() else None
        job_flags = flags or (first.group(1) if first else row['flagset']) or DEFAULT_FLAGS
        jobs.append({'function': name, 'source': text, 'origin': origin, 'flags': job_flags,
                     'target': str(target), 'target_sha256': row['target_o_sha'],
                     'source_sha256': hashlib.sha256(text.encode()).hexdigest()})
    return sorted(jobs, key=lambda job: (not (near_miss / job['function'] / 'base.c').is_file(), job['function']))


def render_verdicts(rows, commit, date):
    def cell(value):
        return str(value if value is not None else '-').replace('|', '\\|').replace('\n', ' ')
    ordered = sorted(rows, key=lambda row: (row.get('words_differing') is None,
                      row.get('words_differing') or 0, row['function']))
    lines = ['# Near-miss diagnoses', '', 'Generated by `tools.conveyor.pipeline.diagnose` at ' + commit + ' (' + date + ').', '',
             'Diagnostic evidence only. Word counts are positional object comparisons, with known relocations masked; the linked image and ROM gates still decide acceptance.', '',
             '| Function | Words differing | Raw words differing | Verdict | Owning pass | Lever |',
             '|---|---:|---:|---|---|---|']
    for row in ordered:
        lines.append('| ' + ' | '.join(cell(row.get(key)) for key in
                     ('function', 'words_differing', 'strict_words_differing', 'verdict', 'owning_pass', 'lever')) + ' |')
    lines += ['', 'Processed: ' + str(len(rows)) + '; diagnosis failures: ' + str(sum(row['verdict'] == 'diagnosis-failed' for row in rows)) + '.', '', 'Counts:', '']
    lines += ['- ' + cell(verdict) + ': ' + str(count) for verdict, count in sorted(Counter(row['verdict'] for row in rows).items())]
    return '\n'.join(lines) + '\n'


def diagnose_jobs(jobs, output, builder, objdump, batch_size):
    results = []
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    for group in batches(jobs, batch_size):
        try:
            objects, failures = compile_batch(group, output, builder)
        except (OSError, RuntimeError, subprocess.SubprocessError, tarfile.TarError) as exc:
            objects, failures = {}, {job['function']: str(exc) for job in group}
        for job in group:
            name = job['function']
            report = {'function': name, 'source_origin': job['origin'], 'flags': job['flags'],
                      'source_sha256': job['source_sha256'], 'target_sha256': job['target_sha256']}
            try:
                if name in failures:
                    raise RuntimeError(failures[name])
                document = run_workbench(job['target'], objects[name], name, objdump)
                report.update(summary(document), diagnosis=document,
                              candidate_sha256=hashlib.sha256(objects[name].read_bytes()).hexdigest())
            except (OSError, RuntimeError, ValueError, KeyError) as exc:
                report.update(verdict='diagnosis-failed', error=str(exc))
            (output / (name + '.json')).write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')
            print(json.dumps({key: value for key, value in report.items() if key != 'diagnosis'}, sort_keys=True), flush=True)
            results.append(report)
    return results


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data', type=Path, default=Path.home() / '.conveyor')
    parser.add_argument('--builder', default='watchman2')
    parser.add_argument('--objdump', default='mips-linux-gnu-objdump')
    parser.add_argument('--output', type=Path, default=REPO / 'build/diagnose')
    sub = parser.add_subparsers(dest='command', required=True)
    one = sub.add_parser('one')
    one.add_argument('function')
    one.add_argument('--source', type=Path)
    one.add_argument('--flags')
    triage = sub.add_parser('triage')
    triage.add_argument('--limit', type=int)
    triage.add_argument('--batch-size', type=int, default=20)
    args = parser.parse_args(argv)
    if args.command == 'triage' and (args.batch_size < 1 or (args.limit is not None and args.limit < 1)):
        parser.error('limit and batch size must be positive')
    try:
        with sqlite3.connect((args.data / 'conveyor.db').resolve().as_uri() + '?mode=ro', uri=True) as conn:
            conn.row_factory = sqlite3.Row
            jobs = select_jobs(conn, ReadStore(args.data / 'blobs'),
                               getattr(args, 'function', None), getattr(args, 'source', None), getattr(args, 'flags', None))
        if args.command == 'triage' and args.limit is not None:
            jobs = jobs[:args.limit]
        if not jobs:
            raise ValueError('no candidate sources found')
        results = diagnose_jobs(jobs, args.output, args.builder, args.objdump, getattr(args, 'batch_size', 1))
        if args.command == 'triage':
            provenance = subprocess.check_output(['git', 'log', '-1', '--format=%h %cs', '--', 'tools/conveyor/pipeline/diagnose.py'], cwd=REPO, text=True).strip()
            commit, generated_date = provenance.split() if provenance else ('uncommitted', date.today().isoformat())
            (REPO / 'cloud/work/near-miss/VERDICTS.md').write_text(render_verdicts(results, commit, generated_date))
        return int(any(row['verdict'] == 'diagnosis-failed' for row in results))
    except (ValueError, OSError, sqlite3.Error) as exc:
        print(str(exc), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
