#!/usr/bin/env python3
"""Reject loss of per-operation f32 rounding in an actual compiled C mutant."""
import argparse, hashlib, json, os, tempfile
from pathlib import Path
import verify_independent as review


def run(root):
    code,image=review.targets.load(root)
    source=(review.PACKET/'semantic.c').read_text()
    changed=source.replace('f32 offset[3], product, value, angle;',
                           'f32 offset[3], value, angle; double product;')
    changed=changed.replace('product = offset[','product = (double)offset[')
    assert changed != source
    with tempfile.TemporaryDirectory(prefix='aa8c-rounding-',dir=os.environ.get('TMPDIR')) as directory:
        host=review.compile_host(Path(directory),changed)
        try:review.compare(code,image,host,dict(mode=0,cached=9),dict(seed=0))
        except AssertionError as error:
            assert 'trace mismatch' in str(error)
        else:raise AssertionError('double product intermediate survived')
    digest=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    return dict(status='DOUBLE_PRODUCT_INTERMEDIATE_REJECTED',base_commit=review.targets.BASE,
       native_targets=review.targets.EXPECTED,
       semantic_source_sha256=digest(review.PACKET/'semantic.c'),
       host_bus_sha256=digest(review.PACKET/'host_bus.c'),
       host_header_sha256=digest(review.PACKET/'semantic_bus.h'),
       verifier_sha256=digest(Path(__file__)),
       independent_verifier_sha256=digest(Path(review.__file__)),
       mutant_source_sha256=hashlib.sha256(changed.encode()).hexdigest(),
       fixture=dict(mode=0,cached=9,seed=0),
       result='First created transform differs: retaining f64 products removes required single-precision rounding.')


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--packet',type=Path,required=True)
    parser.add_argument('--reference-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    args.output.write_text(json.dumps(run(args.reference_root.resolve()),indent=2)+'\n')
