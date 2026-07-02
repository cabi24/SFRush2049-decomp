"""Shared bearer-token auth for the trusted-LAN coordinator (research R7)."""
import hmac
import secrets
from pathlib import Path


def load_or_create_token(data_dir):
    token_path = Path(data_dir) / "token"
    if token_path.is_file():
        return token_path.read_text().strip()
    token_path.parent.mkdir(parents=True, exist_ok=True)
    token = secrets.token_hex(24)
    token_path.write_text(token + "\n")
    token_path.chmod(0o600)
    return token


def check_bearer(header_value, token):
    """Constant-time check of an `Authorization: Bearer <token>` header."""
    if not header_value or not header_value.startswith("Bearer "):
        return False
    presented = header_value[len("Bearer "):].strip()
    return hmac.compare_digest(presented, token)
