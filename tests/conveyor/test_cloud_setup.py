"""F5: an untrusted compiler archive must not reach extraction."""
import io
import os
from pathlib import Path
import shutil
import subprocess
import tarfile

import pytest

SETUP = Path(__file__).resolve().parents[2] / "tools/cloud/setup.sh"
pytestmark = pytest.mark.skipif(
    not shutil.which("bash") or not shutil.which("sha256sum"),
    reason="needs bash and sha256sum")


@pytest.mark.parametrize("valid_tar", [False, True])
def test_wrong_checksum_aborts_before_extraction(tmp_path, valid_tar):
    # Include spaces in the installation path to exercise checksum file lookup.
    cloud = tmp_path / "cloud tools"
    cloud.mkdir()
    shutil.copyfile(SETUP, cloud / "setup.sh")
    archive = tmp_path / "download.tgz"
    if valid_tar:
        with tarfile.open(archive, "w:gz") as tar:
            data = b"#!/bin/sh\nexit 0\n"
            entry = tarfile.TarInfo("cc")
            entry.size, entry.mode = len(data), 0o755
            tar.addfile(entry, io.BytesIO(data))
    else:
        archive.write_bytes(b"corrupted download")
    binaries = tmp_path / "bin"
    binaries.mkdir()
    curl = binaries / "curl"
    curl.write_text('''#!/bin/bash
while [ "$#" -gt 0 ]; do
  if [ "$1" = "-o" ]; then
    cp "$F5_DOWNLOAD_SOURCE" "$2"
    exit 0
  fi
  shift
done
exit 2
''')
    tar = binaries / "tar"
    tar.write_text('#!/bin/sh\ntouch "$F5_TAR_CALLED"\n')
    curl.chmod(0o755)
    tar.chmod(0o755)
    marker = tmp_path / "tar-called"
    env = dict(os.environ, PATH=str(binaries) + os.pathsep + os.environ["PATH"],
               F5_DOWNLOAD_SOURCE=str(archive), F5_TAR_CALLED=str(marker))
    proc = subprocess.run(["bash", str(cloud / "setup.sh")], cwd=tmp_path,
                          env=env, capture_output=True, text=True)
    assert proc.returncode != 0
    assert "ido.tgz: FAILED" in proc.stdout
    assert "checksum did NOT match" in proc.stderr
    assert not marker.exists()
    assert not (cloud / "ido/cc").exists()
    assert "IDO 5.3 ready" not in proc.stdout
