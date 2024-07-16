CFLAGS = -O1
CFLAGS += -D__LIB_PALM_SDK__
CFLAGS += -DEMULATION_LEVEL=EMULATION_UNIX
#CFLAGS += -nostdinc
#CFLAGS += -Wno-char-subscripts
#CFLAGS += -Wno-misleading-indentation
#CFLAGS += -Wno-multichar
#CFLAGS += -Wno-pointer-sign
#CFLAGS += -Wno-unused-but-set-variable
#CFLAGS += -Wno-unused-parameter
#CFLAGS += -Wno-unused-variable
#CFLAGS += -Wno-switch

INCLUDE = -IRsc
INCLUDE += -I$(PALM-SDK-INCLUDE)
INCLUDE += -I$(PALM-SDK-INCLUDE)/Core
INCLUDE += -I$(PALM-SDK-INCLUDE)/Core/Hardware
INCLUDE += -I$(PALM-SDK-INCLUDE)/Core/System
INCLUDE += -I$(PALM-SDK-INCLUDE)/Core/UI
INCLUDE += -I$(PALM-SDK-INCLUDE)/Dynamic
INCLUDE += -I$(PALM-SDK-INCLUDE)/Libraries

#LDLIBS = -lSDL2
#LDLIBS = -lSDL

PALM-SDK = git/clone/palm-sdk-5r3
PALM-SDK-INCLUDE = $(PALM-SDK)/include

SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET_LIB = libpalm.a

VPATH = source

all: $(TARGET_LIB)

include git/libbse/makefile.setup
include git/libbse/makefile.build
