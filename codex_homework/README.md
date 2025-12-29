# Codex Homework Directory

This directory facilitates collaboration between Claude Code and Codex for the Rush 2049 decompilation project.

## Directory Structure

```
codex_homework/
├── README.md              # This file
├── homework_001.md        # Assignment 1 (pending)
├── homework_002.md        # Assignment 2 (pending)
├── ...
└── completed/
    ├── completed_001.md   # Results for homework_001
    ├── completed_002.md   # Results for homework_002
    └── ...
```

## Workflow

### For Claude Code (Assignment Creator)

1. **Create assignments** with sequential numbering:
   - `homework_001.md`, `homework_002.md`, etc.
   - Use 3-digit zero-padded index for sorting

2. **Assignment format**:
   ```markdown
   # Homework [INDEX]: [Title]

   ## Objective
   [Clear, actionable goal]

   ## Context
   [Relevant background, file locations, prior work]

   ## Tasks
   1. [Specific task 1]
   2. [Specific task 2]
   ...

   ## Deliverables
   - [Expected output file(s)]
   - [Format requirements]

   ## Reference Files
   - `path/to/relevant/file.c`
   - `reference/repos/rushtherock/game/file.c`

   ## Success Criteria
   - [How to know the task is complete]
   ```

3. **Track assignments** in the main `CLAUDE.md` or a tracking table

### For Codex (Assignment Worker)

1. **Check for pending homework**:
   ```bash
   ls codex_homework/homework_*.md
   ```

2. **Work on assignments** in order (lowest index first)

3. **Save results** to `completed/completed_XXX.md` where XXX matches the homework index:
   ```markdown
   # Completed: Homework [INDEX]

   ## Summary
   [Brief description of what was done]

   ## Findings
   [Key discoveries, mappings, analysis]

   ## Files Created/Modified
   - `docs/new_file.md`
   - `src/game/updated.c`

   ## Notes for Claude
   [Any follow-up items, questions, or recommendations]

   ## Time Spent
   [Approximate duration]
   ```

4. **Do NOT delete** the original homework file - Claude will archive/clean up

### For Claude Code (Reviewing Completed Work)

1. **Check for completed work**:
   ```bash
   ls codex_homework/completed/
   ```

2. **Review results** and integrate into project

3. **Archive or delete** processed homework files

4. **Update project documentation** (CLAUDE.md, PROGRESS_REPORT.md)

## Assignment Categories

| Category | Prefix Suggestion | Example |
|----------|-------------------|---------|
| Analysis | `homework_1XX` | Function mapping, struct analysis |
| Documentation | `homework_2XX` | Creating docs/*.md files |
| Code Review | `homework_3XX` | Reviewing decompiled code |
| Research | `homework_4XX` | Arcade source deep dives |
| Implementation | `homework_5XX` | Stub function guides |

## Current Status

| Index | Title | Status | Assigned | Completed |
|-------|-------|--------|----------|-----------|
| - | - | - | - | - |

(Claude: update this table as assignments are created/completed)

## Tips for Effective Assignments

1. **Be specific** - Include exact file paths and function names
2. **Provide context** - Link to relevant prior work
3. **Set clear scope** - Define what's in/out of scope
4. **Include examples** - Show expected output format
5. **Reference arcade source** - Point to `reference/repos/rushtherock/`

## Communication Protocol

- Claude creates assignments based on project needs
- Codex works independently on assignments
- Results go in `completed/` subdirectory
- Claude reviews and integrates work
- Questions/blockers noted in completed file for Claude to address

## Quick Reference

```bash
# Claude: Create new assignment
vim codex_homework/homework_001.md

# Codex: List pending assignments
ls codex_homework/homework_*.md | grep -v completed

# Codex: Save completed work
vim codex_homework/completed/completed_001.md

# Claude: Check for completed work
ls codex_homework/completed/

# Count pending vs completed
echo "Pending: $(ls codex_homework/homework_*.md 2>/dev/null | wc -l)"
echo "Completed: $(ls codex_homework/completed/*.md 2>/dev/null | wc -l)"
```
