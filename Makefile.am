SUBDIRS = src

resourcesdir=$(datadir)/$(PACKAGE)

install-data-hook:
	@mkdir -p $(resourcesdir)
	@cp -rv $(srcdir)/resources/* $(resourcesdir)
