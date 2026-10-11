# w18a — runtime-image (overlay A/B) layout + splice + lock path

Status: IN PROGRESS (written early; updated as deliverables land).

## Commands the coordinator runs (Pi, repo root)

```bash
python3 -m tools.conveyor.pipeline.ovl_image extract                 # build/ovl/{a,b}/image.bin from baserom
python3 -m tools.conveyor.pipeline.ovl_layout build --passthrough    # image gate, no C, both images
python3 -m tools.conveyor.pipeline.ovl_layout build                  # image gate with every locked body
```
