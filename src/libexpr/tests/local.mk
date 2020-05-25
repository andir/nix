check: libexpr-tests_RUN

programs += libexpr-tests

libexpr-tests_DIR := $(d)

libexpr-tests_INSTALL_DIR :=

libexpr-tests_SOURCES := $(wildcard $(d)/*.cc)

libexpr-tests_CXXFLAGS += -I src/libutil -I src/libstore -I src/libfetchers -I src/libmain -I src/libexpr

libexpr-tests_LIBS = libutil libstore libfetchers libnixrus libexpr

libexpr-tests_LDFLAGS := $(GTEST_LIBS)
