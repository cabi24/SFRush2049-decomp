# Codex Task: Process Ollama Decompilation Output

## Input Files
- `ollama_analysis/all_functions_classified.txt` - 762 function classifications
- `ollama_analysis/priority_decompiled.txt` - 9 core game functions
- `ollama_analysis/batch2_decompiled.txt` - 13 physics/collision functions
- `ollama_analysis/batch3_decompiled.txt` - 14 state handler/camera functions

## Tasks

### 1. Extract Symbols to symbol_addrs format
Run: `python3 tools/parse_ollama_output.py extract-symbols > ollama_analysis/extracted_symbols.txt`

Then review and clean up `extracted_symbols.txt`:
- Remove duplicate entries
- Fix any malformed function names (must be valid C identifiers)
- Keep format: `func_XXXXXXXX = 0xXXXXXXXX; // description`

### 2. Extract Clean C Code
Run: `python3 tools/parse_ollama_output.py clean-c > ollama_analysis/extracted_c_code.txt`

Review the C code output and:
- Remove markdown formatting artifacts (```, etc)
- Ensure each function has proper header comment with address
- Note any functions that look incomplete or errored

### 3. Generate Summary
Run: `python3 tools/parse_ollama_output.py summary > ollama_analysis/summary.txt`

## Output Location
Place all processed output in `ollama_analysis/` directory:
- `ollama_analysis/extracted_symbols.txt` - Ready to merge into symbol_addrs.us.txt
- `ollama_analysis/extracted_c_code.txt` - Cleaned C code snippets
- `ollama_analysis/summary.txt` - Analysis summary

## DO NOT
- Modify symbol_addrs.us.txt directly (human review needed)
- Commit changes (human will review and commit)
- Delete original ollama output files

## After Completion
Leave a note in `ollama_analysis/CODEX_DONE.txt` with:
- What was processed
- Any issues found
- Functions that need human review
