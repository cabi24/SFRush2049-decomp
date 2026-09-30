from tools.conveyor.pipeline import cloud_worklist


def test_shim_include_is_expanded_in_place():
    text = '#include "conveyor_shim.h"\n\nint f(void) { return 1; }\n'
    out = cloud_worklist.expand_shim(text)
    assert "#include" not in out.splitlines()[0] and '#include "conveyor_shim.h"' not in out
    assert "compatibility shim" in out           # the shim text is inlined
    assert out.rstrip().endswith("int f(void) { return 1; }")


def test_text_without_the_include_is_unchanged():
    text = "int f(void) { return 1; }\n"
    assert cloud_worklist.expand_shim(text) == text
