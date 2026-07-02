"""Content-addressed blob store: sha256-named files under <data>/blobs/."""
import contextlib
import hashlib
import os
import tempfile
from pathlib import Path

_CHUNK = 1024 * 1024


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(_CHUNK), b""):
            h.update(chunk)
    return h.hexdigest()


class BlobStore:
    def __init__(self, root):
        self.root = Path(root)
        self.root.mkdir(parents=True, exist_ok=True)

    def _path(self, sha256):
        return self.root / sha256

    def has(self, sha256):
        return self._path(sha256).is_file()

    def get(self, sha256):
        """Return the file path for a blob, or None if absent."""
        p = self._path(sha256)
        return p if p.is_file() else None

    def put_bytes(self, data):
        sha = hashlib.sha256(data).hexdigest()
        if not self.has(sha):
            self._write_atomic(sha, data)
        return sha

    def put_file(self, path):
        sha = sha256_file(path)
        if not self.has(sha):
            # Copy via temp file + rename so a crash never leaves a partial
            # blob under its final content-addressed name.
            with open(path, "rb") as src:
                self._write_atomic(sha, None, reader=src)
        return sha

    def put_stream(self, reader, length=None):
        """Ingest from a file-like object (e.g. an HTTP request body)."""
        fd, tmp = tempfile.mkstemp(dir=str(self.root), prefix=".ingest-")
        h = hashlib.sha256()
        try:
            with os.fdopen(fd, "wb") as out:
                remaining = length
                while True:
                    n = _CHUNK if remaining is None else min(_CHUNK, remaining)
                    if n == 0:
                        break
                    chunk = reader.read(n)
                    if not chunk:
                        break
                    h.update(chunk)
                    out.write(chunk)
                    if remaining is not None:
                        remaining -= len(chunk)
            sha = h.hexdigest()
            final = self._path(sha)
            if final.exists():
                os.unlink(tmp)
            else:
                os.replace(tmp, final)
            return sha
        except BaseException:
            with contextlib.suppress(OSError):
                os.unlink(tmp)
            raise

    def size(self, sha256):
        p = self._path(sha256)
        return p.stat().st_size if p.is_file() else None

    def _write_atomic(self, sha, data, reader=None):
        fd, tmp = tempfile.mkstemp(dir=str(self.root), prefix=".ingest-")
        try:
            with os.fdopen(fd, "wb") as out:
                if reader is not None:
                    for chunk in iter(lambda: reader.read(_CHUNK), b""):
                        out.write(chunk)
                else:
                    out.write(data)
            os.replace(tmp, self._path(sha))
        except BaseException:
            with contextlib.suppress(OSError):
                os.unlink(tmp)
            raise
