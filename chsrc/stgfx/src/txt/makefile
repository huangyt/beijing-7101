# vi:noexpandtab
######################
# MODULE : sttxt.lib #
######################


DVD_MAKE_VERSION :=2

ifdef IN_OBJECT_DIR

include $(DVD_MAKE)/generic.mak

HEADER_IMPORT += stblit stgxobj

SUB_COMPONENT := $(notdir $(DVD_BUILD_DIR))
    
ST20_TARGETS := $(SUB_COMPONENT).lib
ST20_OBJS    := sttxt.tco sttxt_metrics.tco sttxt_effects.tco

ST40_IMPORTS := os20emu
ST40_TARGETS := $(call ST20LIB_TO_ST40LIB,$(ST20_TARGETS))
ST40_OBJS = $(call ST20OBJ_TO_ST40OBJ,$(ST20_OBJS))

PATH_SRC_DIR := $(dir $(DVD_BUILD_DIR))
   
INCLUDE_PATH := $(PATH_SRC_DIR)/gfx $(INCLUDE_PATH)

# Pick up standard targets
include $(DVD_MAKE)/defrules.mak

$(ST20_TARGETS): $(ST20_OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

$(ST40_TARGETS): $(ST40_OBJS)
	 @$(ECHO)  Building $@
	 $(BUILD_LIBRARY)

clean:
	@echo Cleaning $($(ARCHITECTURE)_TARGETS)
	-$(RM) $($(ARCHITECTURE)_OBJS)
	-$(RM) $($(ARCHITECTURE)_TARGETS)

# Local Dependancies


else

include $(DVD_MAKE)/builddir.mak

endif

