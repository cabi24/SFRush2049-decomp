#!/bin/sh
# usage: lst.sh file.c keep1,keep2 -> listing to file.s ; prints the tag-compare lines
./o3s.sh "$1" "$2" > "${1%.c}.s" 2>&1
grep -n -A2 "if (tag == item" "${1%.c}.s" | grep "lw\|lhu" | head -3
