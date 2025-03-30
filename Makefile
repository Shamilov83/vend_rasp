BIN:= vend
SOUR:= rasp_vend.c

CC:=gcc

vend: asp_vend.c
	$(CC) rasp_vend.c -o asp_vend

