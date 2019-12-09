## User configurations ###
# Compilers and flags
CC ?= mpicc
CFLAGS ?= -O0
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

# Dependency locations
EXADIR ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exaHmholtz

### Meta info about the package ###
SRCDIR      = src
BUILDDIR    = build
DEPDIR      = .deps
EXAMPLESDIR = examples
TESTSDIR    = tests

LDFLAGS += -L$(EXADIR)/lib -lexa
incflags = -I$(EXADIR)/include
libname  = exaHmholtz

### Include template makefile ###
-include $(EXADIR)/share/exa-base.mk

.PHONY: lib
lib: lib-base

.PHONY: examples
examples: examples-base

.PHONY: tests
tests: tests-base
	@cp $(EXADIR)/share/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh

.PHONY: install
install: install-base

.PHONY: all
all: lib examples tests install
