# Detect toolchain or OS by different methods
# First check if PATH contains any ';' (that's the seperator on Windows ; afaik nobody sane uses it on any other OS for any purpose so if it contains any of them we're assuming this is Windows)
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
	# Then we get the name through uname
    detected_OS := $(shell uname 2>/dev/null || echo __UnknownOS)
	# And then detect some common Unix-on-Windows toolchains
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MINGW,$(detected_OS))
endif

# If we're on MINGW, we can use native Windows stuff
ifeq ($(detected_OS),MINGW)
	detected_OS := Windows
endif

WARNINGS := -pedantic -Wall -Wextra -Walloc-zero -Wbool-compare -Wcast-align -Wcast-qual -Wchar-subscripts -Wchkp -Wdangling-else -Wdisabled-optimization -Wduplicated-branches -Wduplicated-cond -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winvalid-pch -Wlogical-not-parentheses -Wlogical-op -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wnoexcept -Wnoexcept-type -Wnormalized=nfc -Woverloaded-virtual -Wpointer-arith -Wregister -Wrestrict -Wsign-promo -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing=3 -Wstrict-null-sentinel -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-override -Wswitch-bool -Wundef -Wunreachable-code -Wunused -Wunused-local-typedefs -Wuseless-cast -Wvariadic-macros -Wwrite-strings -Wzero-as-null-pointer-constant -Wno-multichar -Wno-unused-parameter

OPTIMISATIONS := -O3 -frename-registers
# OPTIMISATIONS += -flto
# Link-time optimizing makes the linking super long so it's are unactivated by default

CXXFLAGS = $(CXX) $(OPTIMISATIONS) $(WARNINGS) -Isrc -std=c++17 -c -MMD -MP -MF $@.d
LDFLAGS := $(CXX) $(OPTIMISATIONS) $(WARNINGS) -s

HELLOCONCURRENTWORLD_FILES := helloConcurrentWorld/main.cpp

HELLOCONCURRENTWORLD_OBJS := $(addprefix obj/, $(addsuffix .o, $(HELLOCONCURRENTWORLD_FILES)))
HELLOCONCURRENTWORLD_DEPS := $(addsuffix .d, $(HELLOCONCURRENTWORLD_OBJS))

all: bin/helloConcurrentWorld

bin/helloConcurrentWorld: $(HELLOCONCURRENTWORLD_OBJS)
	@mkdir -p $(@D)
	$(LDFLAGS) $(HELLOCONCURRENTWORLD_OBJS) -o $@

# general compile
obj/%.o: src/%
	@mkdir -p $(@D)
	$(CXXFLAGS) $< -o $@
	
include $(wildcard $(HELLOCONCURRENTWORLD_DEPS))

# Remove all objects files and the binary
clean:
	@rm -rf obj bin/helloConcurrentWorld
