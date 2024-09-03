#!/usr/bin/bash

#FROM_ENCODING="ascii"
#TO_ENCODING="utf-8"
#iconv  -f   $FROM_ENCODING  -t   $TO_ENCODING "log.txt" -o "log_utf8.txt"
#iconv  -t   $TO_ENCODING "log.txt" -o "log_utf8.txt"
strings ~/Desktop/log.txt > log_ascii_$1.txt
rm ~/Desktop/log.txt
#FROM_ENCODING="ascii"
#TO_ENCODING="UTF-8"
#iconv  -f   $FROM_ENCODING  -t   $TO_ENCODING "log_ascii.txt" -o "log_utf8.txt"
#sed "/P2P: r /d" log_utf8.txt > log_postprocess_$1.txt
