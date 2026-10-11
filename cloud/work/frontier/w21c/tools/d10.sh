#!/bin/sh
# print D10 source path for FN
awk -F, -v f="$1" '$2==f{print $11}' cloud/work/boot_tail/STATUS.csv
