SOURCES	= sources/Application.cpp sources/Router.cpp sources/Request.cpp sources/Response.cpp
OBJECTS	= $(patsubst sources/%, build/%, $(SOURCES:.cpp=.o))
LDFLAGS	= -lboost_regex -lboost_thread -lfcgi
CFLAGS	= -Wall -I./includes/
TARGET	= libambrogio
AR		 ?= ar
PREFIX ?= /usr
VMAJOR	= 0
VMINOR	= 1
VERSION	= $(VMAJOR).$(VMINOR)

all: $(TARGET).so.$(VERSION) $(TARGET).a

install: all ambrogio.pc
	@mkdir -p $(DESTDIR)/$(PREFIX)/{include,lib,lib/pkgconfig}
	@cp ambrogio.pc $(DESTDIR)/$(PREFIX)/lib/pkgconfig
	@cp includes/Ambrogio $(DESTDIR)/$(PREFIX)/include
	@cp $(TARGET).so.$(VERSION) $(TARGET).a $(DESTDIR)/$(PREFIX)/lib
	@cd $(DESTDIR)/$(PREFIX)/lib && ldconfig -l $(TARGET).so.$(VERSION) && ln -s $(TARGET).so.$(VMAJOR) $(TARGET).so

$(TARGET).so.$(VERSION): $(OBJECTS)
	@echo "CXX -shared $(TARGET).so.0.1"
	@$(CXX) -shared -Wl,-soname,$(TARGET).so.0 -o $(TARGET).so.0.1 $(OBJECTS) $(LDFLAGS)

$(TARGET).a: $(OBJECTS)
	@echo "AR $(TARGET).a"
	@$(AR) rcs $(TARGET).a $(OBJECTS)

$(OBJECTS): build/%.o: sources/%.cpp make-build-dir $(SOURCES)
	@echo "CXX -c $<"
	@$(CXX) -fPIC -c $(CFLAGS) -o $@ $<

make-build-dir:
	-@mkdir -p build

ambrogio.pc:
	@echo WRITE ambrogio.pc
	@echo "prefix=$(DESTDIR)/$(PREFIX)" > ambrogio.pc
	@echo 'exec_prefix=$${prefix}' >> ambrogio.pc
	@echo 'libdir=$${exec_prefix}/lib' >> ambrogio.pc
	@echo	'includedir=$${prefix}/include' >> ambrogio.pc
	@echo >> ambrogio.pc
	@echo 'Name: Ambrogio' >> ambrogio.pc
	@echo 'Description: Minimal C++ Web Framework' >> ambrogio.pc
	@echo 'Version: $(VERSION)' >> ambrogio.pc
	@echo 'Requires:' >> ambrogio.pc
	@echo 'Libs: -L$${libdir} -lambrogio' >> ambrogio.pc
	@echo 'Libs.private: -lboost_regex -lboost_thread -lfcgi' >> ambrogio.pc
	@echo 'Cflags: -I$${includedir}' >> ambrogio.pc

clean-output:
	@echo "CLEAN"

clean-target:
	-@rm -f $(TARGET).so.$(VERSION) $(TARGET).a 2>/dev/null

clean-pc:
	-@rm -f ambrogio.pc 2>/dev/null

clean-objs:
	-@rm -f $(OBJECTS) 2>/dev/null
	-@rm -rf build 2>/dev/null

clean-test:
	-@rm -f test/test $(TARGET).so* 2>/dev/null

clean: clean-output clean-objs clean-target clean-pc clean-test

test: $(TARGET).so.$(VERSION)
	@ldconfig -l $(TARGET).so.$(VERSION)
	-@ln -s $(TARGET).so.$(VMAJOR) $(TARGET).so 2>/dev/null
	$(CXX) -std=c++0x -DUSE_CPP11=1 -L. $(CFLAGS) -o test/test test/test.cpp $(LDFLAGS) -lambrogio
	test/test

.PHONY: all test clean install
