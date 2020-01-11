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
EXADIR ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exaHmholtz

### Fortran interface ###
UNDERSCORE ?= 1

LDFLAGS  += -L$(EXADIR)/lib -lexa -lm
INCFLAGS += -I$(EXADIR)/include
libName   = exaHmholtz

### Include template makefile ###
-include $(EXADIR)/share/exa-base.mk

.PHONY: lib
lib: lib-base

.PHONY: examples
examples: install examples-base

.PHONY: tests
tests: install tests-base
	@cp $(EXADIR)/share/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh

.PHONY: install
install: lib install-base
	@cp -r kernels $(PREFIX)/

.PHONY: all
all: lib examples tests install
