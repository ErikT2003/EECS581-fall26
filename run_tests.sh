#!/bin/bash
while IFS= read -r line; do
  [ "$line" = "END" ] && break
  printf '%-32s -> ' "[$line]"
  printf '%s\nEND\n' "$line" | ./ipv4 | head -1 | sed "s/^Enter a string (or 'END' to quit): //"
done < tests.txt
