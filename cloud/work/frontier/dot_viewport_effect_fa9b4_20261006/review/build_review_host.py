from pathlib import Path
import re,subprocess,os,hashlib,tempfile
HERE=Path(__file__).resolve().parent
WORK=Path(os.environ.get('RUSH_VIEWPORT_REVIEW_WORK',Path(tempfile.gettempdir())/'viewport-source-review')).resolve()
WORK.mkdir(parents=True,exist_ok=True)
PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(HERE.parent))).resolve()
source=(PACKET/'candidate.c').read_bytes()
assert hashlib.sha256(source).hexdigest()=='e843f3500c6a952dd067949a7d5f894febfb8d635d77baa480778360dc95f4ee'
assert (HERE/'frozen_candidate.c').read_bytes()==source
host=(PACKET/'host.c').read_text().replace('#include "candidate.c"','#include CANDIDATE_FILE')
host=host.replace('static void mutate(void)\n{','static void (*review_hook)(int);\nvoid register_hook(void (*hook)(int)) { review_hook=hook; }\nstatic void mutate(void)\n{\n    if (review_hook) review_hook(event_count);')
host+='''\nvoid review_event(unsigned *out) { memcpy(out,events[event_count-1],44); }\nvoid review_ref(int i, int enabled) { D_80152698[i] = enabled ? &refs[i] : 0; }\nint review_ref_state(int i) { return D_80152698[i] != 0; }\nvoid review_header(int i, int value) { headers[i].flags = (s8)value; }\nint review_header_state(int i) { return headers[i].flags; }\n'''
assert (HERE/'host_review.c').read_text()==host
def build(name,src):
 subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(HERE/'host_review.c'),'-o',str(WORK/name)],check=True,env=dict(os.environ,TMPDIR=str(WORK)))
build('review.so','frozen_candidate.c')
print('Literal frozen source host review build passed')
