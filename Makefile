CFLAGS += -Wall -std=c11

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man

.PHONY: all
all: mixd

.PHONY: clean
clean:
	rm -f mixd

.PHONY: install
install: mixd
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1
	install mixd $(DESTDIR)$(BINDIR)/mixd
	install mixd.1 $(DESTDIR)$(MANDIR)/man1/mixd.1
