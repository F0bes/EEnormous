EE_BIN = EEnormous.elf
EE_OBJS = EEnormous.o
EE_OBJS += EEmitter/Block.o EEmitter/Emitter.o EEmitter/Test.o
EE_LIBS = -lkernel -lstdc++

EE_CXXFLAGS = -Werror -std=c++20

all: $(EE_BIN)

clean:
	rm -f $(EE_OBJS) $(EE_BIN)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

wsl: $(EE_BIN)
	$(PCSX2) -elf "$(shell wslpath -w $(shell pwd))/$(EE_BIN)"

emu: $(EE_BIN)
	$(PCSX2) -elf "$(shell pwd)/$(EE_BIN)"

reset:
	ps2client reset
	ps2client netdump

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
