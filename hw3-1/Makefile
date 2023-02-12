CC = gcc
CFLAGS = # 컴파일러에 추가적인 옵션 설정
LDFLAGS = # Linker flag

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $< # gcc -c 현재Prerequisite

ALL = mycp hole list tcp1 tcp2 mycp2 split merge conv access tab

all: $(ALL)

mycp: mycp.o
	$(CC) -o $@ $< $(LDFLAGS) # 현재 Target을 현재 Prerequisite로 만들어라

hole: hole.o
	$(CC) -o $@ $< $(LDFLAGS)

list: list.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp1: tcp1.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp2: tcp2.o
	$(CC) -o $@ $< $(LDFLAGS)

mycp2: mycp2.o
	$(CC) -o $@ $< $(LDFLAGS)

split: split.o
	$(CC) -o $@ $< $(LDFLAGS)

merge: merge.o
	$(CC) -o $@ $< $(LDFLAGS)

conv: conv.o
	$(CC) -o $@ $< $(LDFLAGS)

access: access.o
	$(CC) -o $@ $< $(LDFLAGS)

tab: tab.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL) file.hole
