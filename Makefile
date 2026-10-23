# NHL '94 (GENS) in C: make builds build/gens94 (the game in a window) and build/golden
# (the game headless, for test/check.sh); make dist builds build/dist/gens94, one file with SDL2 in
# it. Needs gcc (or clang), make and SDL2 (pkg-config sdl2).
CC      ?= gcc
WARN    := -Wall -Wextra -Wshadow -Wstrict-prototypes -Wmissing-prototypes -Werror
GAMECF  := -std=gnu11 $(WARN) -Wno-unused-parameter -O2 -Igame -Iplatform -Iruntime/include
RTCF    := -std=c11 $(WARN) -O2 -DNDEBUG -DMDRT_HAVE_NUKED -Iruntime/include
SDLCF   := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDLLIB  := $(filter-out -mwindows,$(shell pkg-config --libs sdl2 2>/dev/null))
SDLSTATIC := $(filter-out -mwindows,$(shell pkg-config --static --libs sdl2 2>/dev/null))
ifeq ($(OS),Windows_NT)
EXE     := .exe
endif

GAME    := $(wildcard game/*/*.c)
RUNTIME := $(wildcard runtime/src/*/*.c)
GAMEO   := $(GAME:%.c=build/obj/%.o) build/obj/platform/hal.o
WINDOWO := build/obj/platform/main.o build/obj/platform/padsetup.o build/obj/platform/menu.o build/obj/platform/settings.o
RTO     := $(RUNTIME:%.c=build/obj/%.o) build/obj/runtime/third_party/nuked-opn2/ym3438.o

all: build/gens94$(EXE) build/golden$(EXE)

build/obj/game/%.o: game/%.c $(wildcard game/*.h) $(wildcard game/*/*.h) $(wildcard platform/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(GAMECF) -c $< -o $@
build/obj/platform/%.o: platform/%.c $(wildcard game/*.h) $(wildcard platform/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(GAMECF) $(SDLCF) -c $< -o $@
build/obj/runtime/%.o: runtime/%.c $(wildcard runtime/include/mdrt/*.h) $(wildcard runtime/src/*.h) $(wildcard runtime/src/*/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(RTCF) $(if $(findstring third_party,$<),-Wno-error) -c $< -o $@
build/obj/test/%.o: test/%.c $(wildcard game/*.h) $(wildcard platform/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(GAMECF) -c $< -o $@

build/gens94$(EXE): $(GAMEO) $(RTO) $(WINDOWO)
	$(CC) -o $@ $^ $(SDLLIB) -lm
build/golden$(EXE): $(GAMEO) $(RTO) build/obj/test/golden.o
	$(CC) -o $@ $^ -lm

# one file to give a player: SDL2 and the compiler's runtime in it, nothing but the system's DLLs needed
dist: build/dist/gens94$(EXE)
build/dist/gens94$(EXE): $(GAMEO) $(RTO) $(WINDOWO)
	@mkdir -p $(dir $@)
	$(CC) -static -s -o $@ $^ $(SDLSTATIC) -lm

clean:
	rm -rf build

.PHONY: all clean dist
