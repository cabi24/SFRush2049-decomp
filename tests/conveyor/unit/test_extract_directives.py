from tools.conveyor.seeds.extract_candidates import extract_functions


def test_macro_semicolons_do_not_hide_static_body():
    source = """extern int dependency(void);
#define ERRCK(fn) ret=fn; if(ret!=0) return ret
#define ARRLEN(a) ((int)(sizeof(a)/sizeof((a)[0])))
int free_blocks(void) { int ret; ERRCK(dependency()); return 0; }
"""
    functions = list(extract_functions(source))
    assert [name for name, _, _ in functions] == ["free_blocks"]
    _, start, end = functions[0]
    assert source[start:end] == "int free_blocks(void) { int ret; ERRCK(dependency()); return 0; }"


def test_continued_macro_braces_do_not_become_functions():
    source = '#define FAKE(x) \\\n void fake(void) { x; }\nint real(void) { return 3; }\n'
    functions = list(extract_functions(source))
    assert [name for name, _, _ in functions] == ["real"]
    _, start, end = functions[0]
    assert source[start:end] == "int real(void) { return 3; }"
