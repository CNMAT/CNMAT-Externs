LIBO_TEST_BASENAMES = osc_test osc_message_s_test osc_class_test osc_message_u_test osc_expr_test osc_parser_test osc_set_s_test

INCLUDES = -I.. -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers
CFLAGS += -Wall -Wno-trigraphs -arch i386 -O3 -funroll-loops
CC = gcc

all: $(LIBO_TEST_BASENAMES) ../libo.a

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -framework CoreServices -L.. -lo -lfl -o $@ $@.c

.PHONY: clean
clean:
	rm -rf $(LIBO_TEST_BASENAMES)
