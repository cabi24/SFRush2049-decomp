import glob, os, re, json, sys
from pathlib import Path
from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import blob_splice
lock = blob_splice.load_lock()
only = set(sys.argv[1:])
files = [p for p in sorted(glob.glob('cloud/matches/*.c')) if os.path.basename(p)[:-2] not in lock and (not only or os.path.basename(p)[:-2] in only)]
names = [os.path.basename(p)[:-2] for p in files]
flags = {}
for p, n in zip(files, names):
    flags[n] = re.match(r"/\* flags: (.*?) \*/", open(p).readline()).group(1).strip()
print('splicing', len(names), 'unlocked singles (skipping', len(glob.glob('cloud/matches/*.c')) - len(names), 'already locked)')
conn = dbmod.connect(Path(os.path.expanduser('~/.conveyor')) / 'conveyor.db')
res = blob_splice.splice(conn, names, lambda t: open(f'cloud/matches/{t}.c').read(), flagsets=flags)
print('spliced', len(res['spliced']))
print('refused', json.dumps(res['refused'], indent=1))
print('image_ok', res['image_ok'])
