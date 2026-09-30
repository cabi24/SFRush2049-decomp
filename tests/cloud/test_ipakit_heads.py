import sys
import unittest
import tempfile
from unittest import mock
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from ipakit import Corpus, Func, IMAGE_BASE, load_corpus, heads, unique_targets  # noqa: E402

JR_RA = 0x03E00008
ADDIU_SP_M16 = 0x27BDFFF0
ADDIU_SP_P16 = 0x27BD0010


class HeadTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.c = load_corpus(discover=False)
        cls.rep = heads.audit(cls.c)
        cls.rows = {int(r['addr'], 16): r for r in cls.rep['heads']}

    def test_extent_scan_reproduces_every_retail_function(self):
        bad = [f.name for f in self.c.funcs.values() if heads.scan_extent(self.c, f.addr) != len(f.words)]
        self.assertEqual(bad, [])

    def test_remaining_heads_match_current_audit(self):
        remaining = {0x8010221C, 0x80102F30, 0x80104704, 0x80104B14,
                     0x80105480, 0x8010D3C0, 0x8010D680}
        self.assertEqual(set(self.rows), remaining)
        self.assertTrue(all(not r['registered'] for r in self.rows.values()))

    def test_historical_markdown_heads_are_registered_or_still_audited(self):
        md = heads.parse_md_heads(ROOT / 'cloud/work/unregistered-heads.md')
        self.assertGreaterEqual(len(md), 30)
        opaque = set()
        for a, words in md.items():
            kind, f, _ = self.c.resolve(a)
            if kind == 'opaque':
                opaque.add(a)
                self.assertEqual(self.rows[a]['words'], words, hex(a))
            else:
                self.assertEqual(kind, 'func', hex(a))
                self.assertFalse(f.discovered)
                self.assertEqual((f.name, len(f.words)), ('func_%08X' % a, words))
        self.assertEqual(opaque, {0x8010221C, 0x80102F30, 0x80104704, 0x80104B14, 0x80105480})

    def test_func_80107EDC_is_registered_with_proved_extent(self):
        kind, f, _ = self.c.resolve(0x80107EDC)
        self.assertEqual(kind, 'func')
        self.assertEqual((f.name, len(f.words)), ('func_80107EDC', 158))
        self.assertEqual(heads.has_prologue(self.c, f.addr), 72)
        self.assertEqual(f.words.count(JR_RA), 1)
        self.assertNotIn(f.addr, self.rows)

    def test_other_named_heads(self):
        for a, words in ((0x80108154, 224), (0x801084D4, 375), (0x8010BC84, 232)):
            self.assertEqual(len(self.c.by_addr[a].words), words, hex(a))
            self.assertNotIn(a, self.rows)
        for a, words in ((0x80102F30, 890), (0x80104704, 260), (0x80104B14, 601)):
            self.assertEqual(self.rows[a]['words'], words, hex(a))

    def test_tail_labels_are_not_heads(self):
        for tail in (0x80108098, 0x801089CC, 0x8010BE7C):
            self.assertNotIn(tail, self.rows)
            self.assertIsNotNone(self.c.containing(tail), hex(tail))
        for tail in (0x80103A08, 0x80104A58):
            self.assertNotIn(tail, self.rows)
            owners = [a for a in self.rows if a < tail < a + 4 * self.rows[a]['words']]
            self.assertEqual(len(owners), 1, hex(tail))

    def test_prologue_does_not_cross_a_jr_ra(self):
        # 0x80103D20 is `jr ra; nop`, the next function's addiu sp is 3 words away: no prologue
        self.assertEqual(heads.has_prologue(self.c, 0x80103D20), 0)
        self.assertEqual(heads.has_prologue(self.c, 0x80103D28), 264)

    def test_no_head_overlaps_a_registered_function(self):
        for a, r in self.rows.items():
            self.assertIsNone(self.c.containing(a), hex(a))                   # the head itself is in an opaque run
        inside = {n for r in self.rows.values() for n in r['contains_registered']}
        self.assertEqual(inside, {'highscore_entry_anim'})                   # the only registered tail label
        self.assertEqual(self.rows[0x80104704]['contains_registered'], ['highscore_entry_anim'])
        c2 = load_corpus(discover=False, heads=True)
        self.assertEqual(c2.funcs['highscore_entry_anim'].tail_of, 'func_80104704')

    def test_opaque_runs_partition(self):
        runs = self.rep['opaque_runs']
        self.assertEqual(sum(r['words'] for r in runs) + sum(len(f.words) for f in self.c.funcs.values()), len(self.c.image))


class DuplicateSections(unittest.TestCase):
    def test_identical_sections_are_read_once_and_conflicts_refused(self):
        import score
        with tempfile.TemporaryDirectory() as tmp:
            directory = Path(tmp)
            section = '.section .text.F, "ax", @progbits\n.word 0x03E00008\n.word 0x00000000\n'
            (directory / 'a.s').write_text(section)
            (directory / 'b.s').write_text(section)
            with mock.patch.object(score, 'ASM_DIR', directory), \
                 mock.patch.object(score, 'targets', return_value={}) as gate, \
                 mock.patch.object(score, 'target_manifest', return_value={}), \
                 mock.patch.object(score, 'verified_bytes', side_effect=lambda path, _: path.read_bytes()):
                self.assertEqual(unique_targets(), {'F': [JR_RA, 0]})
                gate.assert_called_once()
                (directory / 'b.s').write_text(section.replace('0x00000000', '0x24020001'))
                with self.assertRaisesRegex(SystemExit, 'conflicting retail sections'):
                    unique_targets()


class SyntheticHeads(unittest.TestCase):
    def corpus(self, body_head, caller_words_fn):
        head = body_head
        known = [Func('known', IMAGE_BASE, [ADDIU_SP_M16, JR_RA, ADDIU_SP_P16])]
        img = list(known[0].words)
        head_addr = IMAGE_BASE + 4 * len(img)
        img += head
        img += [0] * 8
        known.append(Func('caller', IMAGE_BASE + 4 * len(img), caller_words_fn(head_addr)))
        img += known[1].words
        return Corpus(known, img, {}), head_addr

    def test_scan_extent_handles_forward_branch_past_a_ret(self):
        # beq at top jumps over an early `jr ra` to a later tail: the function ends at the LAST return
        body = [ADDIU_SP_M16,
                0x10000003,            # b +3 (over the early return)
                0,
                JR_RA, ADDIU_SP_P16,   # early return at index 3
                0x24020001,            # li v0,1  (branch target index 5)
                JR_RA, ADDIU_SP_P16]
        c, a = self.corpus(body, lambda h: [ADDIU_SP_M16, (3 << 26) | ((h >> 2) & 0x3FFFFFF), 0, JR_RA, ADDIU_SP_P16])
        self.assertEqual(heads.scan_extent(c, a), 8)

    def test_call_target_in_opaque_run_becomes_a_head(self):
        body = [ADDIU_SP_M16, 0x24020001, JR_RA, ADDIU_SP_P16]
        c, a = self.corpus(body, lambda h: [ADDIU_SP_M16, (3 << 26) | ((h >> 2) & 0x3FFFFFF), 0, JR_RA, ADDIU_SP_P16])
        self.assertEqual(c.resolve(a)[0], 'opaque')
        rep = heads.audit(c)
        rows = {int(r['addr'], 16): r for r in rep['heads']}
        self.assertIn(a, rows)
        self.assertIn('jal', rows[a]['evidence'])
        self.assertEqual(rows[a]['words'], 4)
        self.assertFalse(rows[a]['registered'])

    def test_alternate_entry_into_unregistered_head(self):
        first = 0x24020001
        body = [first, 0x24030002, JR_RA, 0]
        def caller(h):
            # jal head+4 with the head's first word copied into the delay slot
            return [ADDIU_SP_M16, (3 << 26) | (((h + 4) >> 2) & 0x3FFFFFF), first, JR_RA, ADDIU_SP_P16]
        c, a = self.corpus(body, caller)
        rep = heads.audit(c)
        rows = {int(r['addr'], 16): r for r in rep['heads']}
        self.assertIn(a, rows)
        self.assertTrue(any(e.get('unregistered_head') for e in rep['alt_entries']))

    def test_registered_alt_entry(self):
        c = Corpus([Func('F', IMAGE_BASE, [ADDIU_SP_M16, JR_RA, ADDIU_SP_P16]),
                    Func('G', IMAGE_BASE + 12, [ADDIU_SP_M16, (3 << 26) | (((IMAGE_BASE + 4) >> 2) & 0x3FFFFFF), ADDIU_SP_M16, JR_RA, ADDIU_SP_P16])],
                   [ADDIU_SP_M16, JR_RA, ADDIU_SP_P16, ADDIU_SP_M16, (3 << 26) | (((IMAGE_BASE + 4) >> 2) & 0x3FFFFFF), ADDIU_SP_M16, JR_RA, ADDIU_SP_P16], {})
        kind, f, alt = c.resolve(IMAGE_BASE + 4)
        self.assertEqual((kind, f.name, alt), ('alt', 'F', True))
        rep = heads.audit(c)
        self.assertEqual(rep['alt_entries'][0]['head'], 'F')
        self.assertTrue(rep['alt_entries'][0]['verified_slot'])


if __name__ == "__main__":
    unittest.main()
