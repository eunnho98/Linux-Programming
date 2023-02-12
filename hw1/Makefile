# SHELL의 절대 경로지정, bash shell 이라는 것을 알려줌
SHELL = /bin/sh 

#DISTRO: 리눅스 시스템에서 사용되는 소프트웨어 패키지, clanton은 intel-Galileo에서 사용되는 이미지
DISTRO = clanton-tiny 

# Intel-Galileo에서 사용되는 IoT Dev Kit, 그것을 TARGET_NAME이라고 명명
TARGET_NAME = i586-poky-linux-uclibc

# Yocto가 리눅스 배포판을 원활하게 개발할 수 있는 툴이고, 그것의 레퍼런스 시스템이 Poky
# Poky의 소프트웨어 개발 킷을 NATIVE_NAME이라고 명명
NATIVE_NAME = x86_64-pokysdk-

# GCC의 버전
GCC_VERSION = 4.7.2 

# .bin 파일: Binary format으로 정보를 저장하기 위해 디자인된 형식
# Binary format은 디스크 스토리지와 호환되어 실제 디스크에 저장 가능
# TARGET_BIN: target system을 위한 bin의 output
TARGET_BIN    = helloworld

# Arch Linux: x86-64용 리눅스 배포판
# TARGET_ARCH은 이 arch을 파싱할 때 타겟팅하는 CPU 제품군
TARGET_ARCH   = -m32 -march=i586 --sysroot=$(SYSROOT_TARGET)

# include 파일 경로 지정
INCLUDE_DIRS  = -I $(SYSROOT_TARGET)/usr/include \
				-I $(SYSROOT_TARGET)/usr/include/c++ \
				-I $(SYSROOT_NATIVE)/usr/lib/$(TARGET_NAME)/gcc/$(TARGET_NAME)/$(GCC_VERSION)/include

# library 파일 경로 지정
LIBRARY_DIRS  = -L $(SYSROOT_TARGET)/lib \
				-L $(SYSROOT_TARGET)/usr/lib \
				-L $(SYSROOT_TARGET)/usr/lib/$(TARGET_NAME)/$(GCC_VERSION)

# -O2: 만들어지는 코드가 가능한 빠르게 수행되도록하고 코드의 크기가 커지지 않는 한에서 범위 최적화 하는 컴파일 옵션
# -pipe: 멀티코어 액세스 가능한 병렬 실행을 약간 추가
# -g: gdb에게 제공하는 정보를 바이너리에 삽입. g 옵션을 주지 않고 디버깅하면 C 레벨이 아닌 어셈블리 레벨 디버깅이 됨
# -feliminate-unused-debug-types: 컴파일 장치에 선언된 유형에 대해 방출되는 디버그 정보 제어
# -fpermissive: 부적합 코드에 대한 일부 진단을 오류에서 경고로 다운그레이드함 -> 일부 부적합 코드 컴파일 가능
# -Wall: 모든 컴파일러의 경고 메시지를 활성화
# INCLUDE_DIRS에 있는 include 파일에 대해 COMPILE_OPTS를 적용
COMPILE_OPTS  = -O2 -pipe -g -feliminate-unused-debug-types -fpermissive -Wall $(INCLUDE_DIRS) 

# Intel-Galileo에서 사용되는 IoT Dev Kit의 시스템 경로 지정
SYSROOT_TARGET = /opt/$(DISTRO)/1.4.2/sysroots/$(TARGET_NAME)/

# Poky의 소프트웨어 개발 킷의 경로 지정
SYSROOT_NATIVE = /opt/$(DISTRO)/1.4.2/sysroots/$(NATIVE_NAME)/

# 개발 킷 툴의 경로 지정
TOOLDIR = $(SYSROOT_NATIVE)/usr/bin/$(TARGET_NAME)

#----------------- c, cpp 파일 등의 파일들을 처리하기 위한 명령어 ---------------
# CC = gcc
CC      = $(TOOLDIR)/i586-poky-linux-uclibc-g++

# CXX = g++
CXX     = $(CC)

# AS = assembler
AS      = $(CC)

# LD: 링킹 과정에서 각 오브젝트를 같은 종류의 섹션 별로 묶어 하나의 바이너리로 만듦
LD      = $(CC)

# AR = Archive
AR      = $(TOOLDIR)/i586-poky-linux-uclibc-ar

# CFLAGS: C 컴파일러의 플래그
CFLAGS  = $(COMPILE_OPTS)

# C++ 컴파일러의 플래그
CXXFLAGS= $(COMPILE_OPTS)

# ld 링커의 플래그
LDFLAGS = -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed $(LIBRARY_DIRS) -lstdc++

# make가 완전한 빌드를 만드는 데 필요한 모든 것을 빌드할  나타냄
all: target것임을

# $(patsubst %.c,%.o,$(wildcard *.c)): 특정 패턴을 대체, .c 패턴을 .o 패턴으로 바꿈.
# wildcard는 여러 파일을 한꺼번에 지정할 목적으로 사용 -> 모든 c파일을 .o로 대체
# cpp 파일도 o 파일로 전부 대체
# 모든 object 파일을 빌드
target: $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# $(LD) == gcc, $(LDFLAGS) == ld 링커의 플래그
# &^: the list of all the current prerequisites
# $(TARGET_BIN) 파일을 o 옵션으로 컴파일
	$(LD) $(LDFLAGS) $(TARGET_ARCH) $^ -o $(TARGET_BIN)

# vs에서 다시 빌드를할 때 초기화하는 작업과 비슷
clean:
	rm -f $(TARGET_BIN) *.o
