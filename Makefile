CC ?= gcc
PKG_CONFIG ?= pkg-config

CFLAGS += -Wall -Wextra -O2 $(shell $(PKG_CONFIG) --cflags gtk+-2.0 cairo)
LDLIBS += $(shell $(PKG_CONFIG) --libs gtk+-2.0 cairo)

OBJS = main.o gnect_engine.o
TEST_OBJS = smoke_test.o gnect_engine.o

.PHONY: all clean

all: exact-four-in-a-row

exact-four-in-a-row: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

smoke-test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LDLIBS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) exact-four-in-a-row smoke-test
