BIN = fat16
SRCS = $(shell ls *.c)
HDRS = $(shell ls *.h)
OBJS = $(SRCS:.c=.o)
CFLAGS = -I.

%.o : %.c $(HDRS)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(BIN)