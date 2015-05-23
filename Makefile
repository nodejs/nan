TOPLEVEL ?= $(dir $(lastword $(MAKEFILE_LIST)))
CPPLINT ?= $(TOPLEVEL)/cpplint.py
PYTHON ?= python
BUILDTYPE ?= Release
MODULES = symbols strings
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
	nan_callbacks.h \
	nan_callbacks_12_inl.h \
	nan_callbacks_pre_12_inl.h \
	nan_converters.h \
	nan_converters_43_inl.h \
	nan_converters_pre_43_inl.h \
	nan_implementation_12_inl.h \
	nan_implementation_pre_12_inl.h \
	nan_maybe_43_inl.h \
	nan_maybe_pre_43_inl.h \
	nan_new.h \
	nan_persistent_12_inl.h \
	nan_persistent_pre_12_inl.h \
	nan_string_bytes.h \
	nan_weak.h \
	test/cpp/accessors.cpp \
	test/cpp/accessors2.cpp \
	test/cpp/asyncworker.cpp \
	test/cpp/asyncprogressworker.cpp \
	test/cpp/asyncworkererror.cpp \
	test/cpp/bufferworkerpersistent.cpp \
	test/cpp/error.cpp \
	test/cpp/gc.cpp \
	test/cpp/indexedinterceptors.cpp \
	test/cpp/morenews.cpp \
	test/cpp/converters.cpp \
	test/cpp/isolatedata.cpp \
	test/cpp/makecallback.cpp \
	test/cpp/morenews.cpp \
	test/cpp/multifile1.cpp \
	test/cpp/multifile2.cpp \
	test/cpp/multifile2.h \
	test/cpp/namedinterceptors.cpp \
	test/cpp/nancallback.cpp \
	test/cpp/nannew.cpp \
	test/cpp/news.cpp \
	test/cpp/objectwraphandle.cpp \
	test/cpp/persistent.cpp \
	test/cpp/returnemptystring.cpp \
	test/cpp/returnnull.cpp \
	test/cpp/returnundefined.cpp \
	test/cpp/returnvalue.cpp \
	test/cpp/settemplate.cpp \
	test/cpp/strings.cpp \
	test/cpp/symbols.cpp \
	test/cpp/threadlocal.cpp \
	test/cpp/trycatch.cpp \
	test/cpp/weak.cpp \
	test/cpp/weak2.cpp \
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

$(ADDONS): nan.h nan_new.h nan_implementation_pre_12_inl.h nan_implementation_12_inl.h \
		nan_callbacks.h nan_callbacks_12_inl.h nan_callbacks_pre_12_inl.h \
		nan_converters.h nan_converters_43_inl.h nan_converters_pre_43_inl.h \
		nan_maybe_43_inl.h nan_maybe_pre_43_inl.h \
		nan_persistent_12_inl.h nan_persistent_pre_12_inl.h nan_weak.h \
		nan_string_bytes.h test/binding.gyp $(SOURCES)
	cd test/ && ../node_modules/.bin/node-gyp rebuild
