# Makefile
#
# Generated by phxrpc_pb2server from node.proto
#
#

include /usr/local/etc/phxrpc/phxrpc.mk

LDFLAGS := -L$(PHXRPC_ROOT)/lib -lphxrpc $(LDFLAGS)

# choose to use boost for network
#LDFLAGS := $(PLUGIN_BOOST_LDFLAGS) $(LDFLAGS)

SVR_OBJS = node.pb.o \
		node_service_impl.o \
		phxrpc_node_service.o \
		phxrpc_node_dispatcher.o \
		node_server_config.o \
		node_main.o \
		NodeData.o 

CLI_OBJS = node.pb.o \
		node_client.o \
		phxrpc_node_stub.o

TARGETS = libnode_client.a node_main node_tool_main

all: $(TARGETS)

node_main: $(SVR_OBJS)
	$(LINKER) $^ -L../AdminServer/ -ladmin_client -L../SimuClient/ -lsimu_client $(LDFLAGS) -o $@

libnode_client.a: $(CLI_OBJS)
	$(AR) $@ $^

node_tool_main: phxrpc_node_tool.o node_tool_impl.o node_tool_main.o
	$(LINKER) $^ -L. -lnode_client $(LDFLAGS) -o $@

########## message ##########

node.pb.cc: node.pb.h

node.pb.h: node.proto
	$(PROTOBUF_ROOT)/bin/protoc -I$(PROTOBUF_ROOT)/include --cpp_out=. -I$(PHXRPC_ROOT) -I. $^

########## client ##########

phxrpc_node_stub.cpp: phxrpc_node_stub.h
phxrpc_node_stub.o: phxrpc_node_stub.h
node_client.cpp: phxrpc_node_stub.h
node_client.o: phxrpc_node_stub.h

phxrpc_node_stub.h: node.proto
	$(PHXRPC_ROOT)/codegen/phxrpc_pb2client $(PBFLAGS) -f $^ -d . -p mqtt

########## service ##########

phxrpc_node_service.cpp: phxrpc_node_service.h
phxrpc_node_service.o: phxrpc_node_service.h
node_service_impl.cpp: phxrpc_node_service.h
node_service_impl.o: phxrpc_node_service.h
phxrpc_node_dispatcher.cpp: phxrpc_node_service.h
phxrpc_node_dispatcher.o: phxrpc_node_service.h
NodeData.o: NodeData.h

phxrpc_node_service.h: node.proto
	$(PHXRPC_ROOT)/codegen/phxrpc_pb2service $(PBFLAGS) -f $^ -d . -p mqtt

########## tool ##########

phxrpc_node_tool.cpp: phxrpc_node_tool.h
phxrpc_node_tool.o: phxrpc_node_tool.h
node_tool_impl.cpp: phxrpc_node_tool.h
node_tool_impl.o: phxrpc_node_tool.h
node_tool_main.cpp: phxrpc_node_tool.h
node_tool_main.o: phxrpc_node_tool.h

phxrpc_node_tool.h: node.proto
	$(PHXRPC_ROOT)/codegen/phxrpc_pb2tool $(PBFLAGS) -f $^ -d . -p mqtt

clean:
	@($(RM) $(TARGETS))
	@($(RM) *.o)
	@($(RM) phxrpc_*)
	@($(RM) *.pb.*)

