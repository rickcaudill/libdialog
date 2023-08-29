CFLAGS   += -c -fexceptions
CXXFLAGS += -c -fexceptions -I $(DIALOG_INCLUDE_PATH)
AOPTS    = -c

VPATH = ./ fontrequester
LIBBIN = /system/libraries
LIBNAME = libdialog.so.1

OBJS	= action.o actionlist.o standardwindow.o \
		  alert.o splash.o inputbox.o \
		  fontrequester.o fontrequesterview.o \
		  colorrequester.o

OBJDIR	:= objs

include ../makefile.rules


all :	$(OBJDIR) $(OBJDIR)/$(LIBNAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/$(LIBNAME): $(OBJS)
	$(CXX) -shared -Xlinker "-soname=$(LIBNAME)" $(OBJS) -lm -lsyllable -o   $(DIALOG_LIB_PATH)/$(LIBNAME)

dist: install

#install: $(OBJDIR)/$(LIBNAME)
#	install -s $(DOCK_LIB_PATH)/$(LIBNAME) $(IMAGE)/$(LIBBIN)/$(LIBNAME)
#	ln -sf $(LIBNAME) $(IMAGE)/$(LIBBIN)/libdock.so


doc:
	-rm -R docs/*
	doxygen libdock.dox

#install-doc:
#	-rm -R /atheos/Documentation/dock/*
#	-mkdir -p /atheos/Documentation/dock
#	cp -R docs/html/* /atheos/Documentation/dock

clean:
	-rm -rf $(OBJS) $(OBJDIR)/*


-include $(OBJDIR)/*.d




