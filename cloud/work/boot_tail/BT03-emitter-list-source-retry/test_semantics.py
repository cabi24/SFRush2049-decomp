"""Source-bound C89/ASan/UBSan replay of the prior independently reviewed domains."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
PRIOR = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists/test_semantics.py'
SPEC = importlib.util.spec_from_file_location('emitter_prior_tests', PRIOR)
PRIOR_MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PRIOR_MODULE)


class EmitterSourceChecks(unittest.TestCase):
    def compile_run(self, source, body):
        with tempfile.TemporaryDirectory(prefix='emitter-semantic-') as temp:
            temp = Path(temp)
            harness = temp / 'test.c'
            harness.write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=address,undefined', '-no-pie', str(harness), '-o', str(temp / 'test')],
                           check=True, capture_output=True)
            subprocess.run([str(temp / 'test')], check=True, capture_output=True,
                           env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                                    UBSAN_OPTIONS='halt_on_error=1'))

    def run_c(self, address, body, match=False):
        self.assertFalse(match)
        sources = sorted((WORK / 'controls').glob('func_' + address + '_*.c'))
        self.assertEqual(len(sources), 2 if address == '8001D944' else 3)
        for source in sources:
            with self.subTest(source=source.name):
                self.compile_run(source, body)

    def test_small_reviewed_domain_all_controls(self):
        PRIOR_MODULE.Semantics.test_small_group_ordered_insertion(self)

    def test_large_reviewed_domain_all_controls(self):
        PRIOR_MODULE.Semantics.test_large_group_anchor_and_capacity(self)

    def test_exact_source_and_control_inventory(self):
        record = json.loads((WORK / 'verification.json').read_text())
        expected = {row['source_path']: row['source_sha256'] for row in record['results']}
        for path, digest in expected.items():
            self.assertEqual(hashlib.sha256((ROOT / path).read_bytes()).hexdigest(), digest)

    def test_large_shared_heads_untouched_bytes_and_all_float_inputs(self):
        source = WORK / 'controls/func_8001DA74_for_traversal.c'
        body = r'''
EmitGroup D_8004FDA0[32]; u8 D_8004FF20;
LargeNode D_8004FF28[32]; u8 D_800502A8;
SmallNode D_800502B0[32]; u8 D_80050430;
static EmitGroup groups[32];
static LargeNode nodes[32];
static SmallNode smalls[32];
static float val(int n) {
    u32 bits[7] = {0xBF800000U,0,0x3F800000U,0x40000000U,
                   0x7FC00000U,0x7F800000U,0xFF800000U};
    float f; memcpy(&f, &bits[n], 4); return f;
}
int main(void) {
    EmitObject object, before_object;
    int chain[4] = {17,0,6,5};
    int count, choice, full, length, fixture, i, group, previous, following;
    int expected_groups, expected_nodes, success, new_group, pos;
    float values[5];
    for (count=31; count<=32; ++count)
    for (choice=0; choice<3; ++choice)
    for (full=0; full<2; ++full)
    for (length=0; length<=4; ++length)
    for (fixture=0; fixture<7; ++fixture) {
        memset(&object,0xA5,sizeof(object));
        memset(D_8004FDA0,0,sizeof(D_8004FDA0));
        memset(D_8004FF28,0,sizeof(D_8004FF28));
        memset(D_800502B0,0,sizeof(D_800502B0));
        for (i=0;i<32;++i) {
            D_8004FDA0[i].identifier=100+(u32)i;
            D_8004FF28[i].distance=-77.0f;
        }
        new_group=choice==2;
        group=new_group?count:(choice?count-1:0);
        object.group38=new_group?999:D_8004FDA0[group].identifier;
        if (!new_group) {
            for (i=0;i<length;++i) {
                D_8004FF28[chain[i]].distance=(float)(3-i);
                D_8004FF28[chain[i]].next=i+1<length?D_8004FF28+chain[i+1]:0;
            }
            D_8004FDA0[group].large=length?D_8004FF28+chain[0]:0;
            /* Another group's pointer can refer to the same real list. */
            D_8004FDA0[group?0:1].large=D_8004FDA0[group].large;
        }
        D_8004FF20=(u8)count; D_800502A8=full?32:31; D_80050430=19;
        memcpy(groups,D_8004FDA0,sizeof(groups));
        memcpy(nodes,D_8004FF28,sizeof(nodes));
        memcpy(smalls,D_800502B0,sizeof(smalls));
        memcpy(&before_object,&object,sizeof(object));
        for(i=0;i<5;++i) values[i]=val((fixture+i)%7);
        expected_groups=count; expected_nodes=full?32:31;
        success=!(new_group&&count==32)&&!full;
        if (new_group&&count<32) {
            groups[group].large=0; groups[group].small=0;
            groups[group].identifier=object.group38; ++expected_groups;
        }
        if (success) {
            previous=-1; following=-1;
            if (!new_group&&length) {
                pos=0;
                while(pos+1<length&&!(nodes[chain[pos]].distance<values[0])) ++pos;
                previous=chain[pos];
                following=pos+1<length?chain[pos+1]:-1;
            }
            nodes[31].next=following<0?0:D_8004FF28+following;
            if(previous<0) groups[group].large=D_8004FF28+31;
            else nodes[previous].next=D_8004FF28+31;
            nodes[31].object=&object;
            memcpy(&nodes[31].distance,&values[0],4);
            memcpy(&nodes[31].first,&values[1],4);
            memcpy(&nodes[31].second,&values[2],4);
            memcpy(&nodes[31].third,&values[3],4);
            memcpy(&nodes[31].fourth,&values[4],4);
            ++expected_nodes;
        }
        assert(func_8001DA74(&object,values[0],values[1],values[2],values[3],values[4])==success);
        assert(D_8004FF20==expected_groups&&D_800502A8==expected_nodes&&D_80050430==19);
        assert(memcmp(groups,D_8004FDA0,sizeof(groups))==0);
        assert(memcmp(nodes,D_8004FF28,sizeof(nodes))==0);
        assert(memcmp(smalls,D_800502B0,sizeof(smalls))==0);
        assert(memcmp(&before_object,&object,sizeof(object))==0);
    }
    return 0;
}
'''
        self.compile_run(source, body)


if __name__ == '__main__':
    unittest.main()
