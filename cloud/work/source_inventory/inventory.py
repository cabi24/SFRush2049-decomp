#!/usr/bin/env python3
"""Conservative C definition leads across explicit Git revisions, not C parsing."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[3]

def mask_noncode(source):
    """Keep positions/newlines while masking comments, strings and directives."""
    def blank(match):
        return re.sub(r'[^\n]', ' ', match.group())
    # Match literals before comment markers inside them; preserve line positions.
    text = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//(?:\\\r?\n|[^\n])*',
                  blank, source, flags=re.S)
    lines = text.splitlines(keepends=True)
    continuation = False
    for i, line in enumerate(lines):
        if continuation or line.lstrip().startswith('#'):
            continuation = line.rstrip('\r\n').rstrip().endswith('\\')
            lines[i] = re.sub(r'[^\n]', ' ', line)
    return ''.join(lines)

def close_delimiter(text, start, opening='(', closing=')'):
    depth = 0
    for i in range(start, len(text)):
        if text[i] == opening:
            depth += 1
        elif text[i] == closing:
            depth -= 1
            if depth == 0:
                return i
    return None

def definitions(source):
    text = mask_noncode(source)
    # Anchor at declaration line. Never let a preceding #define eat the header.
    pattern = re.compile(r'^[ \t]*(?:[A-Za-z_]\w*[ \t\n]+|\*[ \t]*)+([A-Za-z_]\w*)\s*\(', re.M)
    result, consumed = [], -1
    for match in pattern.finditer(text):
        if match.start() < consumed:
            continue
        name = match.group(1)
        if name in {'if', 'while', 'for', 'switch', 'return'}:
            continue
        end_args = close_delimiter(text, match.end()-1)
        if end_args is None:
            continue
        start_body = end_args + 1
        while start_body < len(text) and text[start_body].isspace():
            start_body += 1
        if start_body >= len(text) or text[start_body] != '{':
            continue
        end_body = close_delimiter(text, start_body, '{', '}')
        if end_body is None:
            continue
        consumed = end_body+1
        body = text[start_body+1:end_body]
        original_body = source[start_body+1:end_body]
        classification = 'empty_stub' if not body.strip() else ('todo_body' if re.search(r'\bTODO\b|M2C_ERROR', original_body) else 'nonempty_body_unreviewed')
        result.append(dict(name=name, line=source.count('\n',0,match.start())+1,
                           classification=classification))
    return result

def git(root, *args):
    return subprocess.check_output(['git','-C',str(root),*args])

def inventory(root, refs):
    blobs, revisions = {}, []
    for ref in refs:
        commit = git(root,'rev-parse',ref+'^{commit}').decode().strip()
        tree = git(root,'rev-parse',commit+'^{tree}').decode().strip()
        revisions.append(dict(ref=ref,commit=commit,tree=tree))
        for entry in git(root,'ls-tree','-rz',commit).split(b'\0'):
            if not entry:
                continue
            meta, raw_path = entry.split(b'\t',1)
            mode, kind, sha = meta.decode().split()
            path = raw_path.decode()
            if kind == 'blob' and path.endswith('.c') and path.startswith(('cloud/','src/','work/')):
                blobs.setdefault(sha,{}).setdefault(path,[]).append(ref)
    rows=[]
    for sha, paths in sorted(blobs.items()):
        data=git(root,'cat-file','blob',sha)
        for definition in definitions(data.decode('utf-8',errors='replace')):
            for path, source_refs in sorted(paths.items()):
                rows.append(dict(**definition,path=path,blob=sha,source_sha256=hashlib.sha256(data).hexdigest(),refs=source_refs))
    return dict(schema=1,classification='SOURCE_LEADS_NOT_COMPLETENESS_PROOF',
                revisions=revisions,unique_c_blobs=len(blobs),definitions=rows)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,default=ROOT)
    parser.add_argument('--ref',action='append',required=True)
    args=parser.parse_args()
    print(json.dumps(inventory(args.root,args.ref),indent=2))
