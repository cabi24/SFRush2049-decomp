"""Native/host-C differential fixtures, integer contract and negative controls."""

def require(condition, message):
    if not condition:
        raise AssertionError(message)
import ctypes
from pathlib import Path
import random
import subprocess

def expected(case):
    rate, duration, result, count, state, buffer = case
    if duration & 65535:
        samples = (rate * (duration & 65535) & 4294967295) // 1000
        rounded = (samples // 192 + 1) * 192
        return [rounded, 0, result, 1, rounded * 2, 0, rounded, 0, buffer]
    return [count, state, 0, 0, 0, 0, 0, 0, 0]

def cases():
    rows = []
    for rate in (0, 1, 999, 1000, 191999, 192000, 192001, 44100, 48000, 2147483647, 2147483648, 4294967295):
        for duration in (0, 1, 999, 1000, 65535, 65536, 3735879681, 4294967295):
            for result in (0, 2166571008):
                rows.append((rate, duration, result, 287454020, 2184468088, 2202363776))
    rng = random.Random(69080)
    for _ in range(128):
        rows.append(tuple((rng.getrandbits(32) for _ in range(6))))
    return rows

def host(source, directory, label, optimization="-O2", sanitize=False):
    here = Path(__file__).resolve().parent
    output = directory / (label + '.so')
    extra = ['-fsanitize=undefined,bounds', '-fno-sanitize-recover=all'] if sanitize else []
    subprocess.run(['gcc', '-std=c89', '-Wall', '-Wextra', '-Werror', optimization,
                    '-shared', '-fPIC', *extra, '-include', str(source),
                    str(here / 'host.c'), '-o', str(output)], check=True, capture_output=True)
    library = ctypes.CDLL(str(output))
    fn = library.run_case
    fn.argtypes = [ctypes.c_uint] * 6 + [ctypes.POINTER(ctypes.c_uint)]
    fn.restype = None

    def call(case):
        result = (ctypes.c_uint * 9)()
        fn(*case, result)
        return list(result)
    return call

def run(source, words, directory, native):
    call = host(source, directory, 'delivered')
    checked = host(source, directory, 'delivered_ubsan', '-O0', sanitize=True)
    coverage = set()
    fixture_cases = cases()
    for case in fixture_cases:
        wanted = expected(case)
        require(call(case) == wanted, ('host O2 semantics', case))
        require(checked(case) == wanted, ('host O0 UBSan/bounds semantics', case))
        require(native.execute(words, *case, coverage=coverage) == wanted, ('native semantics', case))
    require(coverage == set(range(0, 168, 4)) - {112}, 'verification requirement failed')
    mutation_rejections = {}
    text = source.read_text()
    for label, old, new in (('ceiling_instead_of_strict_next', '192 - count % 192 + count', '(count + 191) / 192 * 192'), ('wrong_allocation_width', 'D_80038018(count * 2, 0)', 'D_80038018(count, 0)'), ('lost_zero_duration_branch', 'if (duration != 0)', 'if (1)')):
        require(text.count(old) == 1, 'verification requirement failed')
        mutation = directory / (label + '.c')
        mutation.write_text(text.replace(old, new))
        wrong = host(mutation, directory, label)
        failed = sum((wrong(case) != expected(case) for case in fixture_cases))
        require(failed > 0, 'semantic mutation escaped: ' + label)
        mutation_rejections[label] = failed
    for label, edit in (('unknown_opcode', lambda ws: ws.__setitem__(7, 4294967295)), ('zero_divisor', lambda ws: ws.__setitem__(9, ws[9] & ~65535))):
        bad = list(words)
        edit(bad)
        try:
            native.execute(bad, 48000, 1000, 2166571008, 1, 2, 3)
        except AssertionError:
            mutation_rejections[label] = True
        else:
            raise AssertionError('native mutation escaped: ' + label)
    return {'cases': len(fixture_cases), 'native_and_host_equal': True, 'host_builds': ['O2', 'O0 + undefined,bounds sanitizer'], 'full_unsigned_product_wrap': True, 'narrow_duration_entry': True, 'strict_next_quantum': True, 'null_allocator_result': True, 'callback_observes_store_order': True, 'o32_callee_saves_and_stack': True, 'covered_instruction_offsets': sorted(coverage), 'unreachable_constant_divisor_break_offset': 112, 'negative_controls': mutation_rejections}
