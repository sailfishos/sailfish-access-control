NAME ?= sailfishaccesscontrol
ROOT ?= /tmp/test-$(NAME)

PREFIX ?= /usr
LIBDIR ?= $(PREFIX)/lib
INCDIR ?= $(PREFIX)/include/$(NAME)

PKGCFGDIR ?= $(PREFIX)/lib/pkgconfig

SO ?= .so.0

TEMPLATE_COPY = sed\
 -e 's:@NAME@:${NAME}:g'\
 -e 's:@VERSION@:${VERSION}:g'\
 -e 's:@ROOT@:${ROOT}:g'\
 -e 's:@PREFIX@:${PREFIX}:g'\
 -e 's:@LIBDIR@:${LIBDIR}:g'\
 -e 's:@INCDIR@:${INCDIR}:g'\
 -e 's:@DOCDIR@:${DOCDIR}:g'\
 -e 's:@MANDIR@:${MANDIR}:g'\
 -e 's:@PKGCFGDIR@:${PKGCFGDIR}:g'\
 < $< > $@

# ----------------------------------------------------------------------------
# Global Flags
# ----------------------------------------------------------------------------

CPPFLAGS += -D_GNU_SOURCE

CFLAGS   += -Wall
CFLAGS   += -Wmissing-prototypes
CFLAGS   += -Wunused-result
CFLAGS   += -W
CFLAGS   += -std=c99
CFLAGS   += -O2
CFLAGS   += -fPIC
CFLAGS   += -g

LDFLAGS += -fPIC

LDFLAGS  += -g
LDLIBS   += -Wl,--as-needed
# flags from pkgtool

PKG_NAMES    := glib-2.0
PKG_CFLAGS   := $(shell pkg-config --cflags $(PKG_NAMES))
PKG_LDLIBS   := $(shell pkg-config --libs   $(PKG_NAMES))

CFLAGS   += $(PKG_CFLAGS)
LDLIBS   += $(PKG_LDLIBS)

# ----------------------------------------------------------------------------
# Top Level Targets
# ----------------------------------------------------------------------------

TARGETS += lib$(NAME)$(SO)

.PHONY: build clean distclean mostlyclean install

build:: $(TARGETS)

extra:: $(EXTRA)

all:: build extra

clean:: mostlyclean
	$(RM) $(TARGETS) $(EXTRA)

distclean:: clean

mostlyclean::
	$(RM) *.o *~

install:: $(addprefix install-,libsailfishaccesscontrol libsailfishaccesscontrol-dev)

# ----------------------------------------------------------------------------
# Pattern rules
# ----------------------------------------------------------------------------

install-%-lib:
	$(if $<, install -m755 -d $(ROOT)$(LIBDIR))
	$(if $<, install -m755 $^ $(ROOT)$(LIBDIR))

install-%-inc:
	$(if $<, install -m755 -d $(ROOT)$(INCDIR))
	$(if $<, install -m644 $^ $(ROOT)$(INCDIR))

%.pc     : %.pc.tpl ; $(TEMPLATE_COPY)
%        : %.tpl    ; $(TEMPLATE_COPY)

% : %.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(CFLAGS)

%$(SO): LDFLAGS += -shared -Wl,-soname,$@

%$(SO):
	$(CC) -o $@  $^ $(LDFLAGS) $(LDLIBS) $(CFLAGS)

%.pic.o : CFLAGS += -fPIC
%.pic.o : CFLAGS += -fvisibility=hidden

%.pic.o : %.c
	@echo "Compile Dynamic: $<"
	@$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

%.o     : %.c
	@echo "Compile Static: $<"
	@$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

# ----------------------------------------------------------------------------
# libsailfishaccesscontrol
# ----------------------------------------------------------------------------

lib$(NAME)_src =\
 sailfishaccesscontrol.c

lib$(NAME)_obj = $(libsailfishaccesscontrol_src:.c=.o)

lib$(NAME)$(SO) : $(libsailfishaccesscontrol_obj:.o=.pic.o)

install-lib$(NAME)-dll: lib$(NAME)$(SO)
install-lib$(NAME):: $(addprefix install-lib$(NAME)-, dll)
	mkdir -p $(ROOT)$(LIBDIR)/
	install -m755 lib$(NAME)$(SO) $(ROOT)$(LIBDIR)/

# ----------------------------------------------------------------------------
# libsailfishaccesscontrol-dev
# ----------------------------------------------------------------------------

install-libsailfishaccesscontrol-dev-inc: sailfishaccesscontrol.h

install-libsailfishaccesscontrol-dev:: $(addprefix install-libsailfishaccesscontrol-dev-, lib inc) $(NAME).pc
	ln -sf lib$(NAME)$(SO) $(ROOT)$(LIBDIR)/lib$(NAME).so
	install -m755 -d $(ROOT)$(PKGCFGDIR)
	install -m644 $(NAME).pc $(ROOT)$(PKGCFGDIR)/

clean::
	$(RM) $(NAME).pc
