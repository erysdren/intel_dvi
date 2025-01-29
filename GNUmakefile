
RM?=rm -f
PKGCONFIG?=pkg-config
PKGS?=sdl3

override CFLAGS+=$(shell $(PKGCONFIG) --cflags $(PKGS)) -std=c23
override LDFLAGS+=$(shell $(PKGCONFIG) --libs $(PKGS))

BINEXT?=
OBJEXT?=.o

AVS2PNG_EXEC=avs2png$(BINEXT)
AVS2PNG_OBJS=avs2png$(OBJEXT)

UVI2PNG_EXEC=uvi2png$(BINEXT)
UVI2PNG_OBJS=uvi2png$(OBJEXT)

SHARED_OBJS=utils$(OBJEXT) stb_image_write$(OBJEXT)

all: $(AVS2PNG_EXEC) $(UVI2PNG_EXEC)

clean:
	$(RM) $(AVS2PNG_EXEC) $(UVI2PNG_EXEC) $(AVS2PNG_OBJS) $(UVI2PNG_OBJS) $(SHARED_OBJS)

debug: CFLAGS+=-g -O0 -DDEBUG
debug: $(AVS2PNG_EXEC)
debug: $(UVI2PNG_EXEC)

release: CFLAGS+=-O3 -DNDEBUG
release: $(AVS2PNG_EXEC)
release: $(UVI2PNG_EXEC)

$(AVS2PNG_EXEC): $(AVS2PNG_OBJS) $(SHARED_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(UVI2PNG_EXEC): $(UVI2PNG_OBJS) $(SHARED_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

help:
	@echo targets: all clean debug release $(AVS2PNG_EXEC) $(UVI2PNG_EXEC)
