## User configurations ###
# Compilers and flags
CC ?= mpicc
FC ?= mpifort
CFLAGS ?= -O0
FFLAGS ?= -O0
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=
INCFLAGS ?=

# Dependency locations
EXA_DIR ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exaHmholtz

### Fortran interface ###
UNDERSCORE ?= 1

### Meta info about the package ###
SRCDIR       =src
BUILDDIR     =build
EXAMPLESDIR  =examples
TESTSDIR     =tests
INTERFACESDIR=interfaces

LDFLAGS  += -L$(EXA_DIR)/lib -lexa -lm
INCFLAGS += -I$(EXA_DIR)/include -I$(SRCDIR) -I$(INTERFACESDIR)
libName   = exaHmholtz

### Include template makefile ###
-include $(EXA_DIR)/share/exa-base.mk

.PHONY: lib
lib: lib-base

.PHONY: examples
examples: install examples-base

.PHONY: tests
tests: install tests-base
	@cp $(EXA_DIR)/share/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh

.PHONY: install
install: lib install-base
	@cp -r kernels $(PREFIX)/

.PHONY: all
all: lib examples tests install
