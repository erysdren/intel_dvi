
RM?=rm -f
PKGCONFIG?=pkg-config
PKGS?=sdl3

override CFLAGS+=$(shell $(PKGCONFIG) --cflags $(PKGS)) -g3
override LDFLAGS+=$(shell $(PKGCONFIG) --libs $(PKGS))

BINEXT?=
OBJEXT?=.o

EXEC?=dviconv$(BINEXT)
OBJS=dviconv$(OBJEXT)

all: $(EXEC)

clean:
	$(RM) $(EXEC) $(OBJS)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
