
RM?=rm -f
PKGCONFIG?=pkg-config
PKGS?=sdl3

override CFLAGS+=$(shell $(PKGCONFIG) --cflags $(PKGS)) -g3
override LDFLAGS+=$(shell $(PKGCONFIG) --libs $(PKGS))

BINEXT?=
OBJEXT?=.o

EXEC?=dvi2avi$(BINEXT)
OBJS=dvi2avi$(OBJEXT)

all: $(EXEC)

clean:
	$(RM) $(EXEC) $(OBJS)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
