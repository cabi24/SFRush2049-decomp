"""w25a: static_rename for data symbols (timerintr.c's __osCurrentTime etc.):
symbol_addrs keeps a data definition untyped, and `owned` renames a storage
block's linker aliases."""
from tools.conveyor.pipeline import static_rename as S


def test_rewrite_symbols_keeps_data_untyped_and_funcs_typed():
    text = ("gViLastCount = 0x80037C58; // VI last osGetCount value\n"
            "osPfsReadWriteFile_pages = 0x8000F2D0; // type:func read/write pages\n")
    rows = [(0x80037C58, "gViLastCount", "__osBaseCounter"),
            (0x8000F2D0, "osPfsReadWriteFile_pages", "__osCheckId")]
    out, dropped = S.rewrite_symbols(text, rows)
    lines = out.splitlines()
    assert lines[0].startswith("__osBaseCounter = 0x80037C58; // libultra __osBaseCounter (renamed from gViLastCount")
    assert "type:func" not in lines[0]
    assert lines[1].startswith("__osCheckId = 0x8000F2D0; // type:func libultra __osCheckId")
    assert dropped == []


def test_rebind_owned_renames_storage_aliases(tmp_path):
    doc = {"storage_blocks": [{"owner": "timer_services", "tu": "src/rom/x.c", "members": [],
                               "source_sha256": "0", "aliases": [{"symbol": "gViTimeAccumHi", "offset": 32},
                                                                 {"symbol": "__osBaseTimer", "offset": 0}]}]}
    (tmp_path / "src/rom").mkdir(parents=True)
    (tmp_path / "src/rom/x.c").write_text("")
    doc["storage_blocks"][0]["source_sha256"] = S._file_sha(tmp_path / "src/rom/x.c")
    out, _log = S.rebind_owned(doc, {"gViTimeAccumHi": "__osCurrentTime"}, "HEAD", tmp_path, repo=tmp_path)
    assert [a["symbol"] for a in out["storage_blocks"][0]["aliases"]] == ["__osCurrentTime", "__osBaseTimer"]


def test_w25a_table_is_part_of_the_union():
    names = S.libultra_names()
    assert names["osPfsReadWriteFile_pages"] == "__osCheckId"
    assert names["__osCheckId"] == "__osCheckPackId"
    assert names["gViTimeAccumHi"] == "__osCurrentTime"
