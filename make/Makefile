PROJECT                 := flappy
PROJECT_DIR             := $(shell pwd)
PROJECT_LABEL           := flappy

# # # Only for FlopyBuilder
# PROJECT_DSK           := $(PROJECT).dsk

# # # Autorun main project file
PROJECT_AUTORUN         := 1

# # # Leave DOS
PROJECT_QUITDOS         := 1

# # # 6502, 65816
CPU                     := 6502

# # # Toolchain OSDK or CC65
TOOLCHAIN               := OSDK

# # #
START_ADDRESS           := $(shell echo $$((0x1000)))

ATAPS                   :=
BTAPS                   :=
CTAPS                   := $(PROJECT)
OTAPS                   :=

TAPS                    := $(addsuffix .tap,$(ATAPS) $(BTAPS) $(CTAPS) $(OTAPS))

#

$(PROJECT)_SRC          := main.c
$(PROJECT)_AUTORUN      := 1
$(PROJECT)_ADDRESS      := $(START_ADDRESS)
$(PROJECT)_ACPP         := 1

#
include                 ${HOME}/.oric/Makefile.local

OSDK                    := $(OSDK_DIR)
OSDK_OPT                := 0
CC65                    := $(CC65_DIR)
CC65_ALIGNED            := 0

EMU                     := ./oricutron
EMUDIR                  := $(EMUL_DIR)
EMUARG                  := -ma
EMUARG                  += --serial none
EMUARG                  += --vsynchack off
EMUARG                  += -s $(PROJECT_DIR)/$(PROJECT).sym
EMUARG                  += -r :$(PROJECT_DIR)/$(PROJECT).brk
EMUARG                  += #-r $(START_ADDRESS)

COMMON                  := $(COMMON_DIR)

VPATH                   := $(VPATH) ..

PREPARE                 := prepare
FINALIZE                := finalize

#
include $($(TOOLCHAIN))/atmos.make

AFLAGS                  += $(addprefix -I,$(VPATH))
AFLAGS                  += -DASSEMBLER
AFLAGS                  += -DSTART_ADDRESS=$(START_ADDRESS)
AFLAGS                  += -DUSE_ROMCALLS

# AFLAGS                += -DUSE_JOYSTICK
# AFLAGS                += -DUSE_JOYSTICK_IJK
#
# AFLAGS                += -DUSE_VSYNC
# AFLAGS                += -DUSE_VSYNC_50HZ
# AFLAGS                += -DUSE_VSYNC_60HZ
# AFLAGS                += -DUSE_VSYNC_SOFT
# AFLAGS                += -DUSE_VSYNC_HARD
# AFLAGS                += -DUSE_VSYNC_NEGEDGE
# AFLAGS                += -DUSE_VSYNC_AUTO_TEXT

CFLAGS                  += $(addprefix -I,$(VPATH))
CFLAGS                  += -DSTART_ADDRESS=$(START_ADDRESS)

# CFLAGS                += -DUSE_JOYSTICK
# CFLAGS                += -DUSE_JOYSTICK_IJK

# CFLAGS                += -DUSE_VSYNC_50HZ
# CFLAGS                += -DUSE_VSYNC_60HZ


# $(PROJECT)_AFLAGS       +=
# $(PROJECT)_CFLAGS       +=
# $(PROJECT)_LFLAGS       +=

TEMP_FILES              += $(PROJECT)_dsk.hfe DSKA0000.HFE

#
# BIN2TXT := $(OSDK)/bin/bin2txt
# 	@$(BIN2TXT) -s1 -f2 -h1 -n16 infile outfile label >/dev/null

prepare: #res
	@echo "Building with $(TOOLCHAIN)..."

finalize: #hxc
	@([ -e $(PROJECT)_dsk.hfe ] && cp -f $(PROJECT)_dsk.hfe DSKA0000.HFE) || echo -n
	@([ -e $(PROJECT).brk ] || touch $(PROJECT).brk) || echo -n
	@echo   "[NFO]   ----------------------"
	@printf "[MEM]   main  : #%.4X .. #%.4X\\n" $$(($(START_ADDRESS))) $$(expr `cat $(PROJECT) | wc -c` + $$(($(START_ADDRESS))))
	@echo "Done"
# 	cp -f test2.sym flappy.sym


.PHONY: res pictures show showtap

res: pictures

pictures:
	@make -C res/pics all

show: $(PROJECT).dsk
	$(Q) [ -f "$(PWD)/$(PROJECT).dsk" ] && cd $(EMUDIR) && $(EMU) $(EMUARG) "$(PWD)/$(PROJECT).dsk" $(EMUPARAM)

showtap: $(PROJECT).tap
	$(Q) [ -f "$(PWD)/$(PROJECT).tap" ] && cd $(EMUDIR) && $(EMU) $(EMUARG) "$(PWD)/$(PROJECT).tap" $(EMUPARAM)

