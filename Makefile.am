SUBDIRS = src

resourcesdir=$(datadir)/$(PACKAGE)

install-data-hook:
	@mkdir -p $(resourcesdir)
	@cp -rv $(srcdir)/resources/gfx $(resourcesdir)
	@cp -rv $(srcdir)/resources/sfx $(resourcesdir)
	@cp -rv $(srcdir)/resources/obj $(resourcesdir)
	@cp -rv $(srcdir)/resources/lvl $(resourcesdir)
