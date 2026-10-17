"""Finite placement contract; real root and position setter, captured services."""
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
FLAGS = ['-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
         '-Wno-misleading-indentation', '-fsanitize=undefined',
         '-fno-sanitize-recover=undefined']
CONTROLS = {
    'wrong_horizontal_shift': ('root', '>>1', '>>2', 197),
    'wrong_vertical_division': ('root', 'object_bytes23_sum()/2', 'object_bytes23_sum()/3', 34),
    'wrong_draw_bound': ('root', 'menu_input_process(text,-1)', 'menu_input_process(text,0)', 1),
    'wrong_position_sentinel': ('position', 'x!=-32768', 'x!=-32767', 1),
}


def run(position_source):
    original = {'root': (HERE/'tb_draw.c').read_text(), 'position': position_source}
    results = {}
    for name in ['positive'] + list(CONTROLS):
        sources = dict(original)
        if name != 'positive':
            unit, old, new, fixture = CONTROLS[name]
            if sources[unit].count(old) != 1:
                raise RuntimeError('ambiguous semantic mutation')
            sources[unit] = sources[unit].replace(old, new)
        with tempfile.TemporaryDirectory(prefix='tb-place-host-') as tmp:
            path = Path(tmp)
            for unit, source in sources.items():
                (path/(unit+'.c')).write_text(source)
            (path/'semantics.c').write_bytes((HERE/'semantics.c').read_bytes())
            subprocess.run(['cc', *FLAGS, str(path/'semantics.c'), '-o', str(path/'run')],
                           check=True, capture_output=True, text=True)
            result = subprocess.run([str(path/'run')], capture_output=True, text=True)
        row = dict(returncode=result.returncode, stdout=result.stdout, stderr=result.stderr)
        wanted = dict(returncode=0, stdout='37748736 bounded text placement fixtures passed\n', stderr='') if name == 'positive' else dict(returncode=3, stdout='', stderr=f'text placement differential failure at {CONTROLS[name][3]}\n')
        if row != wanted:
            raise RuntimeError('unexpected semantic outcome '+name+': '+str(row))
        results[name] = row
    return {'flags': FLAGS, 'results': results, 'fixtures': 37748736}
