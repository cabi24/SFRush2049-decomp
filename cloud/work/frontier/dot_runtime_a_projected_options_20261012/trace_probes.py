#!/usr/bin/env python3
"""Optional finite source/stage probes. Publish metadata only, not compiler listings."""
import importlib.util
import json
from pathlib import Path
import re
import tempfile

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('projected_options_verify', HERE/'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def inspect(group, work):
    ordinary, traced = work/'ordinary.o', work/'traced.o'
    v.score.compile_group(group, ordinary)
    original = v.score._run
    found = {}

    def run(command, **kwargs):
        command = list(command)
        stage = Path(command[0]).name
        if stage == 'ugen':
            command += ['-e', 'trees', '-l', 'listing']
        result = original(command, **kwargs)
        if stage == 'as1' and result.returncode == 0:
            cwd = Path(kwargs['cwd'])
            names = re.findall(r'^\s*\.ent\s+(\w+)', (cwd/'listing').read_text(), re.M)
            blocks = (cwd/'trees').read_text().split('Tree dump after Build')[1:]
            v.require(len(names) == len(blocks), 'stage procedure counts disagree')
            block = blocks[names.index(v.NAME)].split('Tree dump', 1)[0]
            found['memory_areas'] = {k: int(n) for k, n in re.findall(r'\bdef mtype=(\w+) length=(\d+)', block)}
            found['external'] = int(re.search(r'\bent .* external=(\d+)', block)[1])
        return result

    try:
        v.score._run = run
        v.score.compile_group(group, traced)
    finally:
        v.score._run = original
    v.require(ordinary.read_bytes() == traced.read_bytes(), 'diagnostics changed complete object')
    start = v.score.symbols(ordinary)[v.NAME]
    found['frame_bytes'] = 65536 - (v.score.text_words(ordinary)[start//4] & 65535)
    comparison = v.score.compare(ordinary, v.NAME, show=0)
    found['differing_words'] = comparison.differing
    found['total_words'] = comparison.total
    found['complete_object_diagnostic_identical'] = True
    return found


def main():
    source = (v.GROUP/'renderer.c').read_text()
    title = 'title = countdown_state.indexed[countdown_state.indices[22] + i];\n            state_utility(x, y, title);'
    nested = source.replace(title, 'state_utility(x, y, countdown_state.indexed[countdown_state.indices[22] + i]);')
    v.require(nested != source, 'named-title control unavailable')
    arrays = '    char text[50];\n    s16 point[2];\n'
    previous = nested.replace(arrays, '').replace('    s16 x, y;', arrays + '    s16 x, y;')
    scoped = source.replace('    char text[50];\n', '').replace(
        '    if (D_8014A108 == 1) {', '    { char text[50];\n    if (D_8014A108 == 1) {').replace(
        '    dispatch_handler(10);', '    }\n    dispatch_handler(10);')
    data, base, _ = v.authenticate()
    output = {}
    with tempfile.TemporaryDirectory(prefix='options-trace-') as temp, v.targets(v.owndata.ImageData.from_image(data, base)):
        for name, body in [('previous_group', previous), ('early_locals', nested),
                           ('named_title', source), ('scoped_text', scoped)]:
            work = Path(temp)/name
            work.mkdir()
            (work/'renderer.c').write_text(body)
            for filename in ('group.json', 'eligibility.c'):
                (work/filename).write_bytes((v.GROUP/filename).read_bytes())
            output[name] = inspect(work, work)
        standalone = Path(temp)/'standalone.c'
        standalone.write_text(previous)
        obj = Path(temp)/'standalone.o'
        v.score.compile_single(standalone, '-g0 -O3 -mips2 -G 0 -non_shared', obj)
        result = v.score.compare(obj, v.NAME, show=0)
        output['previous_standalone'] = {'differing_words': result.differing,
                                         'total_words': result.total}
    print(json.dumps(output, indent=2))


if __name__ == '__main__':
    main()
