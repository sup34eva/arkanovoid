NACL_SDK_ROOT ?= $(dir $(lastword $(MAKEFILE_LIST)))../bower_components/nacl_sdk/pepper_34/

WARNINGS := -Wno-long-long -Wall -Wswitch-enum -pedantic -Werror
CXXFLAGS := -pthread -std=gnu++98 $(WARNINGS)

GETOS := python $(abspath $(NACL_SDK_ROOT)/tools/getos.py)
OSHELPERS = python $(abspath $(NACL_SDK_ROOT)/tools/oshelpers.py)
OSNAME := $(shell $(GETOS))
RM := $(OSHELPERS) rm

PNACL_TC_PATH := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_pnacl)
PNACL_CXX := $(PNACL_TC_PATH)/bin/pnacl-clang++
PNACL_FINALIZE := $(PNACL_TC_PATH)/bin/pnacl-finalize
CXXFLAGS := -I$(abspath $(NACL_SDK_ROOT)/include)
LDFLAGS := -L$(abspath $(NACL_SDK_ROOT)/lib/pnacl/Release) -lppapi_cpp -lppapi

CYGWIN ?= nodosfilewarning
export CYGWIN

all: server.pexe

clean:
	$(RM) server.pexe server.bc

server.bc: server.cc
	$(PNACL_CXX) -o $@ $< -O2 $(CXXFLAGS) $(LDFLAGS)

server.pexe: server.bc
	$(PNACL_FINALIZE) -o $@ $<

HTTPD_PY := python $(NACL_SDK_ROOT)/tools/httpd.py

.PHONY: serve
serve: all
	$(HTTPD_PY) -C $(CURDIR)
