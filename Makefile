## User configurations ###
# Compilers and flags
CC ?= mpicc
FC ?= mpifort
CFLAGS ?= -O0
FFLAGS ?= -O0
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

### Fortran interface ###
UNDERSCORE ?= 1

# Dependency locations
EXADIR ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exaHmholtz

### Meta info about the package ###
SRCDIR       = src
BUILDDIR     = build
EXAMPLESDIR  = examples
INTERFACESDIR= interfaces
TESTSDIR     = tests

LDFLAGS += -L$(EXADIR)/lib -lexa -lm
incflags = -I$(EXADIR)/include -I$(INTERFACESDIR)/nek
libname  = exaHmholtz

### Include template makefile ###
-include $(EXADIR)/share/exa-base.mk

.PHONY: interfaces
interfaces: interfaces-base

$(BUILDDIR)/interfaces/nek/%.o: $(INTERFACESDIR)/nek/%.c
	$(compile.c) -I$(INTERFACESDIR)/nek -c $< -o $@

.PHONY: lib
lib: interfaces lib-base

.PHONY: examples
examples: install examples-base

.PHONY: tests
tests: install tests-base
	@cp $(TESTSDIR)/t[0-9][0-9]-*.[^c]* $(BUILDDIR)/$(TESTSDIR)/
	@cp $(EXADIR)/share/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh

.PHONY: install
install: lib install-base
	@cp -r kernels $(PREFIX)/
	@mkdir -p $(PREFIX)/interfaces/nek
	@cp -r interfaces/nek/share $(PREFIX)/interfaces/nek
	@cp interfaces/nek/*.h $(PREFIX)/include/

.PHONY: all
all: lib examples tests install interfaces

$(shell mkdir -p $(BUILDDIR)/$(INTERFACESDIR)/nek)
