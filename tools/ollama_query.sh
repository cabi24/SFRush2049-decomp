#!/bin/bash
# Query Ollama API on watchman24 directly (no SSH needed)
# Usage: ./ollama_query.sh "your prompt here" [model]
# Default model: qwen2.5-coder:7b (fits in 8GB VRAM)

MODEL="${2:-qwen2.5-coder:7b}"
PROMPT="$1"

if [ -z "$PROMPT" ]; then
    echo "Usage: $0 \"prompt\" [model]"
    exit 1
fi

# Create JSON payload and call API directly
python3 -c "
import json, urllib.request, sys

data = json.dumps({
    'model': '$MODEL',
    'prompt': '''$PROMPT''',
    'stream': False
}).encode()

req = urllib.request.Request('http://192.168.50.7:11434/api/generate', data=data)
req.add_header('Content-Type', 'application/json')

try:
    with urllib.request.urlopen(req, timeout=300) as resp:
        result = json.loads(resp.read())
        print(result.get('response', 'ERROR: ' + str(result)))
except Exception as e:
    print(f'ERROR: {e}', file=sys.stderr)
    sys.exit(1)
"
