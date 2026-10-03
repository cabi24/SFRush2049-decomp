#!/usr/bin/env python3
"""Host behavior checks with separate translation units and callback doubles."""
import json
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
CALLBACKS = ('1144C', '11848', '11D24', '121BC', '1261C', '140F8', '14140')


def main():
    with tempfile.TemporaryDirectory(prefix='bt02-medium-host-') as tmp:
        binary = Path(tmp) / 'callbacks'
        subprocess.run(['cc', '-std=c89', '-O2', '-Wall', '-Wextra', '-Werror',
                        str(HERE / 'host_callbacks.c'),
                        *[str(ROOT / 'cloud/matches/boot_tail' / ('func_800' + s + '.c'))
                          for s in CALLBACKS], '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True, timeout=5)
        blocked = []
        for mode in ('busy8', 'busy16'):
            try:
                subprocess.run([str(binary), mode], check=True, timeout=0.2)
            except subprocess.TimeoutExpired:
                blocked.append(mode)
            else:
                raise AssertionError(mode + ' failed to wait while volatile flag remained set')
        selection = Path(tmp) / 'selection'
        subprocess.run(['cc', '-std=c89', '-O2', '-Wall', '-Wextra', '-Werror',
                        '-Wno-pointer-to-int-cast', str(HERE / 'host_buffer_selection.c'),
                        str(ROOT / 'cloud/matches/boot_tail/func_80013964.c'),
                        '-o', str(selection)], check=True)
        subprocess.run([str(selection)], check=True, timeout=5)
    print(json.dumps({'result': 'PASS', 'matching_sources_linked_separately': 8,
                      'callbacks_and_forwarding': 'PASS',
                      'buffer_selection_and_neighbor_preservation': 'PASS',
                      'blocking_control_paths': blocked,
                      'limit': 'Host behavior only; no native pointer-layout, concurrency, hardware or ROM proof.'}, indent=2))


if __name__ == '__main__':
    main()
