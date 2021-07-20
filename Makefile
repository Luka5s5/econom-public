.PHONY: all clean depclean

CC  = gcc
CXX = g++
# profilator -pg
CFLAGS  = -std=gnu99 -O3 -fPIC 
#-g -fsanitize=address -fsanitize=undefined -fstack-protector-strong -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare
CXFLAGS = -std=gnu++0x -O3 -msse3 -fPIC
#-g -fsanitize=address -fsanitize=undefined -fstack-protector-strong -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare

INCL = -I./backend
LIBDYN =
LIBSTAT =

BUILD = bin

LIBS = -lz -lm -pthread

SRC = backend

EXES = $(BUILD)/main
EXES += $(BUILD)/server 

OBJS = $(BUILD)/Game.o
OBJS += $(BUILD)/War.o
OBJS += $(BUILD)/Player.o
OBJS += $(BUILD)/City.o


all: $(OBJS) $(EXES)

$(OBJS:%.o=%.d): $(BUILD)/%.d: $(SRC)/%.cpp
	@echo -e "\033[0;32mBuilding deps for $(@:%.d=%.o)\033[0m"
	@set -e; rm -f $@; \
	$(CXX) -MM -MT '$(@:%.d=%.o)' $(CXFLAGS) $< ${INCL} > $@; \
	echo '	@echo -e "\033[0;32mBuilding $$@\033[0m"' >> $@; \
	echo '	$$(CXX) -c $$(CXFLAGS) $$< -o $$@ $$(INCL)' >> $@


$(EXES:%=%.d): $(BUILD)/%.d: $(SRC)/%.cpp
	@echo -e "\033[1;33mBuilding deps for $(@:%.d=%)\033[0m"
	@set -e; rm -f $@; \
	$(CXX) -MM -MT '$(@:%.d=%)' $(CXFLAGS) $< ${INCL} > $@.$$$$; \
	sed 's,.cpp,& $$(OBJS) ,' < $@.$$$$ > $@; \
	rm -f $@.$$$$; \
	echo '	@echo -e "\033[1;33mBuilding $$@\033[0m"' >> $@; \
	echo '	$$(CXX) $$(CXFLAGS) $$(LIBS) $$(LIBDYN) $$(INCL) -o $$@ $$< $$(OBJS) $$(LIBSTAT)' >> $@

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),depclean)
include $(EXES:%=%.d)
include $(OBJS:%.o=%.d)
endif
endif

clean: 
	rm -f $(EXES) $(OBJS)

depclean:
	rm -f $(COBJS:.o=.d) $(OBJS:.o=.d) $(EXES:%=%.d)
