#!/usr/bin/env python3
"""Pinned IDO O32 layout checks, without changing the scored C."""
import json
from pathlib import Path
import tempfile
from verify import SOURCE, WORK, FLAGS, digest, score
CHECKS=[('pointer','sizeof(void *) == 4'),('word','sizeof(u32) == 4'),
        ('signed_word','sizeof(s32) == 4'),('halfword','sizeof(s16) == 2'),
        ('byte','sizeof(u8) == 1'),('command','sizeof(MacroCommand) == 8'),
        ('word1','(unsigned int)&((MacroCommand *)0)->word1 == 4')]


def run():
    with tempfile.TemporaryDirectory(prefix='bt05-macro-layout-') as tmp:
        path=Path(tmp)/'layout.c';obj=Path(tmp)/'layout.o';original=Path(tmp)/'source.o'
        path.write_text(SOURCE.read_text()+'\n'+''.join(
            'typedef char check_%s[(%s) ? 1 : -1];\n'%x for x in CHECKS))
        score.compile_single(path,FLAGS,obj);score.compile_single(SOURCE,FLAGS,original)
        assert score.text_words(obj)==score.text_words(original)
    return dict(result='PASS',source_sha256=digest(SOURCE),checks=[x[1] for x in CHECKS],
                assertions_leave_candidate_text_unchanged=True,voice_state_layout='opaque, never accessed')


if __name__=='__main__':print(json.dumps(run(),indent=2))
