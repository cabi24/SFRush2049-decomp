#!/usr/bin/env python3
"""Host C89 behavior tests with call mocks; native pointer width/packing is checked separately."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

PACKET = Path(__file__).resolve().parent

def run():
    rows = []
    cases = {}
    for address, keybase, keyfield, count, table, cache in [
        ('80016E68', '80042670', '80042674', '80038608', '80038610', '80042678'),
        ('80016EE0', '80042680', '80042684', '8003C610', '8003C618', '80042688')]:
        cases[address] = (8, '''
unsigned int D_%s;
unsigned short D_%s;
int D_%s;
LookupEntry D_%s[4];
LookupEntry *D_%s;
static LookupEntry selected;
static void *reply;
static int calls;
static unsigned short expected_key;
int func_80016E40(const void *a, const void *b) { (void)a; (void)b; return 0; }
void *func_8001E864(const void *key, const void *base, int count, int width,
                   int (*compare)(const void *, const void *))
{
    assert(key == &D_%s && base == D_%s);
    assert(count == 4 && width == 8 && compare == func_80016E40);
    assert(D_%s == expected_key);
    calls++;
    return reply;
}
int main(void)
{
    unsigned short keys[4] = {0, 63, 64, 65535};
    int i, hit;
    static int payload;
    selected.value = &payload;
    D_%s = 4;
    for (i = 0; i < 4; i++) for (hit = 0; hit < 2; hit++) {
        expected_key = keys[i]; calls = 0;
        reply = hit ? &selected : 0;
        D_%s = &selected;
        assert(func_%s(keys[i]) == (hit ? &payload : 0));
        assert(calls == 1 && D_%s == reply);
    }
    return 0;
}
''' % (keybase, keyfield, count, table, cache, keybase, table, keyfield,
       count, cache, address, cache))
    cases['80016C20'] = (12, '''
LookupGroup D_8003DA28[1024];
LookupEntry D_8003E228[8];
int D_80042630, D_80042634;
unsigned int D_80042638;
unsigned short D_8004263C;
LookupEntry *D_80042640;
static LookupEntry selected;
static void *reply;
static int calls;
static unsigned short expected_key;
int func_80016BF8(const void *a, const void *b) { (void)a; (void)b; return 0; }
void *func_8001E864(const void *key, const void *base, int count, int width,
                   int (*compare)(const void *, const void *))
{
    assert(key == &D_80042638 && base == &D_8003E228[3]);
    assert(count == 2 && width == 8 && compare == func_80016BF8);
    assert(D_8004263C == expected_key && D_80042630 == 3);
    calls++;
    return reply;
}
int main(void)
{
    unsigned short keys[4] = {0, 63, 64, 65535};
    int i, mode, group;
    static int payload;
    selected.value = &payload;
    for (i = 0; i < 4; i++) for (mode = 0; mode < 3; mode++) {
        expected_key = keys[i]; group = keys[i] >> 6; calls = 0;
        D_8003DA28[group].count = mode ? 2 : 0;
        D_8003DA28[group].first = 3;
        D_80042630 = -1; D_80042634 = -1; D_8004263C = 1234;
        D_80042640 = &selected; reply = mode == 2 ? &selected : 0;
        assert(func_80016C20(keys[i]) == (mode == 2 ? &payload : 0));
        assert(D_80042634 == group);
        if (mode) assert(calls == 1 && D_80042640 == reply);
        else assert(calls == 0 && D_80042630 == -1 && D_8004263C == 1234 && D_80042640 == &selected);
    }
    return 0;
}
''')
    cases['80016F80'] = (6, '''
unsigned int D_80042690;
unsigned short D_80042694;
int D_8003CE18;
unsigned char D_8003CE20[48];
LookupEntry *D_8004269C;
static LookupEntry selected;
static void *reply;
static int calls;
static unsigned short expected_key;
int func_80016F58(const void *a, const void *b) { (void)a; (void)b; return 0; }
void *func_8001E864(const void *key, const void *base, int count, int width,
                   int (*compare)(const void *, const void *))
{
    assert(key == &D_80042690 && base == D_8003CE20);
    assert(count == 4 && width == 12 && compare == func_80016F58);
    assert(D_80042694 == expected_key);
    calls++;
    return reply;
}
int main(void)
{
    unsigned short keys[3] = {0, 32768, 65535};
    unsigned short output;
    int i, hit;
    static int payload;
    selected.value = &payload; selected.auxiliary = 65530; D_8003CE18 = 4;
    for (i = 0; i < 3; i++) for (hit = 0; hit < 2; hit++) {
        expected_key = keys[i]; calls = 0; output = 1234;
        reply = hit ? &selected : 0; D_8004269C = &selected;
        assert(func_80016F80(keys[i], &output) == (hit ? &payload : 0));
        assert(calls == 1 && D_8004269C == reply);
        assert(output == (hit ? 65530 : 1234));
    }
    return 0;
}
''')
    cases['80017040'] = (9, '''
int D_80042228;
LookupBank D_80042230[3];
unsigned short D_800426A0;
static char bases[3][12], payload;
static int calls, hit_index, shorten;
int func_80017018(const void *a, const void *b) { (void)a; (void)b; return 0; }
void *func_8001E864(const void *key, const void *base, int count, int width,
                   int (*compare)(const void *, const void *))
{
    int current;
    current = calls++;
    assert(key == &D_800426A0 && D_800426A0 == 65535);
    assert(base == bases[current] && count == current + 1);
    assert(width == 12 && compare == func_80017018);
    if (shorten) D_80042228 = 1;
    return current == hit_index ? &payload : 0;
}
int main(void)
{
    int i, n, hit, expected;
    for (i = 0; i < 3; i++) {
        D_80042230[i].entries = bases[i]; D_80042230[i].count = i + 1;
    }
    for (n = -1; n <= 0; n++) {
        calls = 0; D_80042228 = n;
        assert(func_80017040(65535) == 0 && calls == 0 && D_800426A0 == 65535);
    }
    for (n = 1; n <= 3; n += 2) for (hit = -1; hit < n; hit++) {
        calls = 0; D_80042228 = n; hit_index = hit;
        assert(func_80017040(65535) == (hit < 0 ? 0 : &payload));
        expected = hit < 0 ? n : hit + 1;
        assert(calls == expected);
    }
    calls = 0; D_80042228 = 3; hit_index = -1; shorten = 1;
    assert(func_80017040(65535) == 0 && calls == 1);
    return 0;
}
''')
    with tempfile.TemporaryDirectory(prefix='bt03-lookup-host-') as directory:
        tmp = Path(directory)
        for address, (vectors, harness) in cases.items():
            source = PACKET / 'nonmatch' / ('func_' + address + '.c')
            translation = tmp / (address + '.c')
            translation.write_text('#include <assert.h>\n' + source.read_text() + '\n' + harness)
            executable = tmp / address
            flags = ['-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2',
                     '-fsanitize=undefined', '-fno-sanitize-recover=undefined']
            subprocess.run(['cc', *flags, str(translation), '-o', str(executable)], check=True, capture_output=True, text=True)
            subprocess.run([str(executable)], check=True, capture_output=True, text=True)
            rows.append({'function': 'func_' + address, 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                         'cases': vectors, 'result': 'PASS'})
    return {'schema_version': 1, 'result': 'PASS', 'compiler_flags': ' '.join(flags),
            'scope': 'Host-width logical behavior with a mock search; does not prove native packing, adjacency of separate address-spelled globals, or the real search implementation.',
            'cases': sum(r['cases'] for r in rows), 'results': rows}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')
