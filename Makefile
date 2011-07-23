CC      = gcc
CFLAGS  = -O3
LDFLAGS = -lm

BIN = vnstat-cgi
OBJ = cgi.o config.o error.o language.o main.o output.o vnstat.o
SRC = $(OBJ:%.o=src/%.c)
DEPENDFILE = DEPEND

all: dep $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

dep: $(DEPENDFILE)

$(DEPENDFILE): $(SRC)
	$(CC) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)

clean:
	rm -rf $(BIN) $(OBJ) $(DEPENDFILE)

.PHONY: all dep clean
