"""Synthetic insertion fixtures and an independent list-based reference."""
import struct
COUNT = {0x80015D68: 0x80038608, 0x8001671C: 0x8003DA20}
TABLE = {0x80015D68: 0x80038610, 0x8001671C: 0x8003E228}
RANGES = 0x8003DA28
PAYLOAD = 0x600000
NEW_PAYLOAD = PAYLOAD + 4 * 2048
PROFILES = [(0, []), (1, []), (2, []), (5, []), (17, []), (2047, []), (2048, []),
            (0, []), (0, [(0, 2)]), (0, [(0, 3), (2, 2), (511, 3)]),
            (0, [(3, 5), (8, 3), (2, 4)]),
            (0, [(0, 32)] + [(i, 63) for i in range(1, 33)])]


def initial(function, profile):
    rows = [[PAYLOAD + 4 * i, 0xA500 + i % 256, 0xCAFE] for i in range(2048)]
    ranges = [[0, 0] for _ in range(512)]
    n, groups = PROFILES[profile]
    if function == 0x80015D68:
        for i in range(n):
            rows[i][1:] = [2 * i + 2, 65535 if i % 3 == 0 else 100 + i]
    else:
        n = 0
        for group, length in groups:
            ranges[group] = [length, n]
            for i in range(length):
                step = 2 if profile == 11 and group == 0 else 1
                rows[n + i][1:] = [group * 64 + i * step, 65535 if i % 3 == 0 else 100 + i]
            n += length
    return {'count': n, 'rows': rows, 'ranges': ranges}


def encode(function, state):
    regions = {COUNT[function]: struct.pack('>I', state['count']),
               TABLE[function]: b''.join(struct.pack('>IHH', *row) for row in state['rows'])}
    if function == 0x8001671C:
        regions[RANGES] = b''.join(struct.pack('>HH', *row) for row in state['ranges'])
    return regions


def fingerprint(regions):
    h = 2166136261
    for base in sorted(regions):
        for byte in regions[base]:
            h = ((h ^ byte) * 16777619) & 0xFFFFFFFF
    return h


def hook(function, state, key, mode, entering):
    if entering and mode == 1:
        state['count'] = 0
        state['ranges'] = [[0, 0] for _ in range(512)]
    if not entering and mode == 2:
        for row in state['rows'][:state['count']]:
            if row[1] == key:
                row[2] = 500
                break


def model(function, profile, key, mode):
    state = initial(function, profile)
    hook(function, state, key, mode, True)
    rows, ranges = state['rows'], state['ranges']
    if function == 0x80015D68:
        pos = 0
        while pos < state['count'] and rows[pos][1] < key:
            pos += 1
        if pos < state['count'] and rows[pos][1] == key:
            hook(function, state, key, mode, False)
            rows[pos][2] = (rows[pos][2] + 1) & 65535
            return 0, state
        if state['count'] == 2048:
            hook(function, state, key, mode, False)
            return 0, state
    else:
        group = ranges[key >> 6]
        if not group[0]:
            group[1] = state['count'] & 65535
        first = group[1]
        offset = 0
        while offset < group[0] and rows[first + offset][1] < key:
            offset += 1
        pos = first + offset
        if offset < group[0] and rows[pos][1] == key:
            rows[pos][2] = (rows[pos][2] + 1) & 65535
            hook(function, state, key, mode, False)
            return 0, state
        if state['count'] == 2048:
            hook(function, state, key, mode, False)
            return 0, state
        for other in ranges:
            if first < other[1]:
                other[1] = (other[1] + 1) & 65535
        group[0] = (group[0] + 1) & 65535
    rows[pos + 1:state['count'] + 1] = [list(row) for row in rows[pos:state['count']]]
    rows[pos] = [NEW_PAYLOAD, key, 1]
    state['count'] += 1
    hook(function, state, key, mode, False)
    return 1, state


def cases(function):
    result = []
    for profile in (range(7) if function == 0x80015D68 else range(7, 12)):
        if function == 0x80015D68:
            n = PROFILES[profile][0]
            keys = sorted({0, 1, 2, 3, max(0, 2 * n - 1), 2 * n, 2 * n + 1, 65534, 65535})
        else:
            keys = [0, 1, 2, 3, 4, 62, 63, 64, 65, 126, 127, 128, 129, 130, 131, 192, 196, 197, 512, 515, 32704, 32706, 32766, 32767]
        for key in keys:
            for mode in range(3):
                result.append((profile, key, mode))
    return result


def native_hook(function, key, mode):
    def apply(destination, memory, calls):
        if destination == 0x80014594:
            assert calls == 0
            if mode == 1:
                memory(COUNT[function], 4, 0)
                if function == 0x8001671C:
                    for i in range(512):
                        memory(RANGES + 4 * i, 4, 0)
        else:
            assert destination == 0x800145DC and calls == 1
            if mode == 2:
                count = memory(COUNT[function], 4)
                for i in range(count):
                    if memory(TABLE[function] + 8 * i + 4, 2) == key:
                        memory(TABLE[function] + 8 * i + 6, 2, 500)
                        break
    return apply
