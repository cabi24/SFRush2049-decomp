# Project Wiki (DokuWiki on unraid)

The project documentation wiki lives in a DokuWiki container on the unraid
server. This doc covers how to reach it and how to read/write pages from this
machine (the Pi, `nvmepi3`).

## Where it is

| | |
|---|---|
| Web UI | `http://192.168.50.30:8089` |
| Project namespace | `rush2049:` → `http://192.168.50.30:8089/doku.php?id=rush2049:start` |
| Host | unraid server, `ssh root@192.168.50.30` (this Pi's ed25519 key is authorized) |
| Container | `DocuWiki` (image `dokuwiki/dokuwiki:stable`, host port 8089 → container 8080) |
| Data on host | `/mnt/user/appdata/docuwiki/storage/` (bind-mounted as `/storage`) |

## Current pages (rush2049 namespace)

| Page id | File | Content |
|---|---|---|
| `rush2049:start` | `start.txt` | project overview, key facts, machine roster, page index |
| `rush2049:conveyor` | `conveyor.txt` | matching-pipeline architecture and stages |
| `rush2049:runbook` | `runbook.txt` | bring-up commands, node join, steady-state ops |
| `rush2049:status` | `status.txt` | dated status snapshot (update at milestones) |
| `rush2049:improvements` | `improvements.txt` | big ideas + easy wins backlog |

## Reading pages

Pages are plain text files. Read them directly over SSH:

```bash
ssh root@192.168.50.30 'cat /mnt/user/appdata/docuwiki/storage/data/pages/rush2049/status.txt'
```

Or fetch rendered HTML (also confirms the wiki is serving):

```bash
curl -s "http://192.168.50.30:8089/doku.php?id=rush2049:status"
```

## Writing pages

DokuWiki pages are just `.txt` files in [DokuWiki syntax](https://www.dokuwiki.org/wiki:syntax)
under `storage/data/pages/<namespace>/<page>.txt`. The reliable workflow
(learned the hard way — see caveats):

1. **Write/edit the page locally** on the Pi, then `scp` it over. Do not try
   to edit in place on unraid: the unraid host has **no `python3`**, and
   quoting `sed` expressions through ssh is error-prone.
2. **Fix ownership and mode after every copy** — files must be readable by
   the container's web user (uid/gid `33:33`, shows as `sshd:sshd` on the
   unraid host):

```bash
scp mypage.txt root@192.168.50.30:/mnt/user/appdata/docuwiki/storage/data/pages/rush2049/
ssh root@192.168.50.30 'chown 33:33 /mnt/user/appdata/docuwiki/storage/data/pages/rush2049/mypage.txt \
                        && chmod 644 /mnt/user/appdata/docuwiki/storage/data/pages/rush2049/mypage.txt'
```

3. **Verify it renders**:

```bash
curl -s -o /dev/null -w "%{http_code}\n" "http://192.168.50.30:8089/doku.php?id=rush2049:mypage"
```

New namespaces are just new directories (`mkdir` + same chown). A page named
`foo/bar.txt` under `pages/` is page id `foo:bar`.

## Caveats and conventions

- **Search index is lazy.** Writing files bypasses DokuWiki's indexer; the
  index updates when a page is first visited. If a new page doesn't show in
  search, `curl` it once (as in step 3) or just open it in a browser.
- **Page history is not recorded** for direct file writes (DokuWiki keeps
  revisions in `data/attic/` only for edits made through the web UI). For
  anything where history matters, keep the source of truth in this repo and
  treat the wiki as a rendered copy.
- **DokuWiki syntax gotchas:** `--` becomes an en-dash — escape literal
  double hyphens in shell examples as `%%--%%`. Headings are `====== H1 ======`
  down to `== H5 ==`; tables use `^ header ^` / `| cell |`; code blocks are
  `<code bash> ... </code>`; internal links are `[[rush2049:page|label]]`.
- **Keep `rush2049:status` honest:** it's a dated snapshot and drifts. The
  live truth is `python3 -m tools.conveyor.cli report` on the Pi. Update the
  wiki page at milestones (smoke pass, first matrix run, first promotion) —
  or automate it (see the "easy wins" list on `rush2049:improvements`).
- **Backups:** the wiki lives under unraid's `appdata`, so it's covered by
  the server's appdata backup; nothing extra needed from this side.

## First-time setup on a new machine

If a different machine needs write access, authorize its SSH key on unraid:

```bash
ssh-copy-id root@192.168.50.30       # or paste the pubkey into unraid's web UI
ssh-keyscan 192.168.50.30 >> ~/.ssh/known_hosts   # if host key verification fails
```
