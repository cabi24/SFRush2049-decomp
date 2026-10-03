import importlib.util
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
spec=importlib.util.spec_from_file_location('source_inventory',ROOT/'cloud/work/source_inventory/inventory.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

def test_macro_does_not_consume_function():
    source='#define FIELD(x) (*(float *)(base + (x)))\nvoid target(void)\n{ use(FIELD(4)); }'
    assert m.definitions(source)==[dict(name='target',line=2,classification='nonempty_body_unreviewed')]

def test_multiline_macro_and_comments():
    source='#define BODY(x) \\\nvoid fake(void) { x; }\n/* void bogus(void) {} */\nint real(int x) { return x; }'
    assert [d['name'] for d in m.definitions(source)]==['real']

def test_empty_and_todo_are_not_complete():
    result=m.definitions('void empty(void) { /* TODO */ }\nvoid partial(void) { M2C_ERROR(); }\nvoid incomplete(void) { /* TODO later */ foo(); }')
    assert [d['classification'] for d in result]==['empty_stub','todo_body','todo_body']

def test_prototypes_calls_strings_callback():
    source='void proto(int);\nvoid outer(int (*cb)(int)) { const char *s="void fake(void) { }"; if (cb) { cb(1); } }\n'
    assert [d['name'] for d in m.definitions(source)]==['outer']

def test_pointer_return_and_split_signature():
    source='static const Thing *fetch(\nint n)\n{ return 0; }\nvoid\nsplit(void)\n{ call(); }'
    assert [d['name'] for d in m.definitions(source)]==['fetch','split']

def test_truncated_body_not_recorded():
    assert m.definitions('void bad(void) { call();')==[]

def test_git_inventory_keeps_provenance(tmp_path):
    def git(*a):return subprocess.check_output(['git','-C',str(tmp_path),*a])
    git('init','-q');git('config','user.name','Test');git('config','user.email','test@example.com')
    (tmp_path/'cloud').mkdir();(tmp_path/'cloud/a.c').write_text('void f(void) {}\n')
    git('add','.');git('commit','-qm','fixture');git('branch','second')
    result=m.inventory(tmp_path,['HEAD','second'])
    assert result['unique_c_blobs']==1
    assert len(result['revisions'])==2
    assert result['definitions'][0]['refs']==['HEAD','second']
    assert result['definitions'][0]['classification']=='empty_stub'
    assert len(result['definitions'][0]['source_sha256'])==64


def test_inactive_preprocessor_branches_remain_unreviewed_leads():
    result=m.definitions('#if 0\nvoid disabled(void) { call(); }\n#endif\n')
    assert result==[dict(name='disabled',line=2,classification='nonempty_body_unreviewed')]

def test_continued_line_comment_masks_fake_definition():
    source='// continued '+chr(92)+'\nvoid fake(void) {}\nvoid real(void) {}\n'
    assert [d['name'] for d in m.definitions(source)]==['real']
