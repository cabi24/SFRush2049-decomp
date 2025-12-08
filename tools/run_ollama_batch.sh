#!/bin/bash
# Batch process all naming prompts through Ollama
# Run with: nohup ./tools/run_ollama_batch.sh &

PROMPT_DIR="prompts/naming"
RESULTS_FILE="prompts/results/naming_results.txt"
LOG_FILE="prompts/results/batch.log"

mkdir -p prompts/results

echo "Starting batch processing at $(date)" | tee "$LOG_FILE"
echo "Total prompts: $(ls $PROMPT_DIR/*.txt | wc -l)" | tee -a "$LOG_FILE"

count=0
for prompt_file in $PROMPT_DIR/*.txt; do
    fname=$(basename "$prompt_file" _naming.txt)
    count=$((count + 1))
    
    echo "[$count] Processing $fname..." | tee -a "$LOG_FILE"
    
    # Send to Ollama, clean up output
    result=$(ssh cabi2@192.168.50.7 "ollama run qwen3-coder:30b" < "$prompt_file" 2>/dev/null | tr -d '\r' | grep -v '^\[' | head -3)
    
    echo "$fname: $result" >> "$RESULTS_FILE"
    echo "  -> $result" | tee -a "$LOG_FILE"
done

echo "Batch processing complete at $(date)" | tee -a "$LOG_FILE"
echo "Results saved to $RESULTS_FILE"
