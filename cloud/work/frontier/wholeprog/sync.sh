#!/bin/sh
# Push the scripts to the isolated builder copy (never the production repo).
rsync -a --exclude runs --exclude cache "$(dirname "$0")/" watchman2:rush2049/scratch/frontier/wholeprog/wp/
