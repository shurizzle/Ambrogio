SOURCES	= Application.cpp Router.cpp Request.cpp Response.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
LDFLAGS	= -lboost_regex -lfcgi++
CFLAGS	= -Wall
TARGET	= libambrogio
AR		 ?= ar

all: $(TARGET)-shared $(TARGET)-static

$(TARGET)-shared: $(OBJECTS)
	@echo "CXX -shared $(TARGET).so.0.1"
	@$(CXX) -shared -Wl,-soname,$(TARGET).so.0 -o $(TARGET).so.0.1 $(OBJECTS) $(LDFLAGS)

$(TARGET)-static: $(OBJECTS)
	@echo "AR $(TARGET).a"
	@$(AR) rcs $(TARGET).a $(OBJECTS)

.cpp.o:
	@echo "CXX -c $@"
	@$(CXX) -fPIC -c $(CFLAGS) -o $@ $<

clean-output:
	@echo "CLEAN"

clean-target:
	-@rm $(TARGET).so.0.1 $(TARGET).a 2>/dev/null

clean-objs:
	-@rm $(OBJECTS) 2>/dev/null

clean-test:
	-@rm test

clean: clean-output clean-objs clean-target clean-test

test: all
	g++ -std=c++0x -DUSE_CPP11=1 -L. -o test test.cpp -lfcgi -lboost_regex -lambrogio
	./test

.PHONY: all $(TARGET)-shared $(TARGET)-static test clean
