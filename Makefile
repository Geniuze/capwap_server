CPPFLAGS+= -Iinclude -Wall -fpermissive -g
LDFLAGS += -g
OBJS := dbi.o main.o server.o ubus.o capwap_state.o capwap.o \
        capwap_elements.o capwap_vselement.o business.o dstring.o \
		kvlist.o ap_list.o interface.o

all: capwap_server

capwap_server: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
