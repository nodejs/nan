TOPLEVEL ?= $(dir $(lastword $(MAKEFILE_LIST)))
CPPLINT ?= $(TOPLEVEL)/cpplint.py
PYTHON ?= python
BUILDTYPE ?= Release
MODULES = symbols strings optionvalues
SOURCES = $(MODULES:%=test/cpp/%.cpp)
ADDONS = $(MODULES:%=test/build/$(BUILDTYPE)/%.node)

LINT_SOURCES = \
	examples/async_pi_estimate/addon.cc \
	examples/async_pi_estimate/async.cc \
	examples/async_pi_estimate/async.h \
	examples/async_pi_estimate/pi_est.cc \
	examples/async_pi_estimate/pi_est.h \
	examples/async_pi_estimate/sync.cc \
	examples/async_pi_estimate/sync.h \
	nan.h \
	nan_implementation_12_inl.h \
	nan_implementation_pre_12_inl.h \
	nan_new.h \
	test/cpp/asyncworker.cpp \
        test/cpp/asyncprogressworker.cpp \
        test/cpp/asyncworkererror.cpp \
        test/cpp/bufferworkerpersistent.cpp \
        test/cpp/gc.cpp \
        test/cpp/morenews.cpp \
	test/cpp/multifile1.cpp \
	test/cpp/multifile2.cpp \
	test/cpp/multifile2.h \
        test/cpp/news.cpp \
	test/cpp/optionvalues.cpp \
	test/cpp/persistent.cpp \
	test/cpp/returnemptystring.cpp \
	test/cpp/returnnull.cpp \
	test/cpp/returnundefined.cpp \
	test/cpp/returnvalue.cpp \
	test/cpp/settergetter.cpp \
	test/cpp/strings.cpp \
	test/cpp/symbols.cpp \
	test/cpp/test_nan_new.cpp \
        test/cpp/weak.cpp \
	node_modules/node-gyp/gyp/data/win/large-pdb-shim.cc

FILTER = -whitespace/parens

.PHONY: lint

lint:
	cd $(TOPLEVEL) && $(PYTHON) $(CPPLINT) --filter=$(FILTER) $(LINT_SOURCES)

test: $(ADDONS)
	npm test

forcetest:
	cd test/ && node-gyp rebuild && cd ..
	npm test

$(ADDONS): nan.h nan_new.h nan_implementation_pre_12_inl.h nan_implementation_12_inl.h test/binding.gyp $(SOURCES)
	cd test/ && node-gyp rebuild
