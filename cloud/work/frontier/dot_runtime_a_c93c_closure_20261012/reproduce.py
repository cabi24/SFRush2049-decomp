#!/usr/bin/env python3
"""Run the unchanged canonical all-member scorer with authenticated image data."""
import sys
import verify

with verify.target_scope():
    sys.argv = ['score.py', 'group', str(verify.HERE), '--targets', str(verify.ROOT / 'asm/us/ovl_a')]
    verify.score.main()
