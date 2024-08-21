#include git/libbse/makefile.setup
include git/libbse/makefile.setup.so

#OBJS_LIB_A: $(wildcard $(OBJ_DIR)/*.a)

OBJS_LIBS_SO = \
	$(OBJ_DIR)/EventMgr.a \
	$(OBJ_DIR)/WindowMgr.a

VPATH: $(OBJ_DIR)

TARGET_LIB = libpalm

TARGET_LIB_A = libpalm.a

TARGET_LIB_SO = $(TARGET_LIB).so

#$(OBJ_TARGET_LIB_SO): $(OBJ_TARGET_LIB_SO)($(OBJS_LIBS_SO))

all: $(TARGET_LIB_A) $(TARGET_LIB_SO)
	ranlib $(OBJ_TARGET_LIB_SO)

#$(OBJ_DIR)/%.a:
#	make -f makefile.$*

$(OBJ_DIR)/EventMgr.a:
	make -f makefile.EventMgr

$(OBJ_DIR)/WindowMgr.a:
	make -f makefile.WindowMgr

include git/libbse/makefile.build
