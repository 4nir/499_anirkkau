HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ../protos

vpath %.proto $(PROTOS_PATH)

all: backend_store_server service_layer_server command_line key_value_store_tests service_layer_tests

command_line: command_line.cc backend_store.pb.o backend_store.grpc.pb.o service_layer.pb.o service_layer.grpc.pb.o backend_store_client.o service_layer_client.o
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags

backend_store_server: backend_store.pb.o backend_store.grpc.pb.o service_layer.pb.o service_layer.grpc.pb.o backend_store_client.o service_layer_client.o backend_store_server.o key_value_store.o
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags

service_layer_server: service_layer.pb.o service_layer.grpc.pb.o backend_store.pb.o backend_store.grpc.pb.o backend_store_client.o service_layer_client.o service_layer_server.o
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags

key_value_store_tests: key_value_store_tests.cc key_value_store.o
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags

service_layer_tests: service_layer.pb.o service_layer.grpc.pb.o backend_store.pb.o backend_store.grpc.pb.o service_layer_tests.cc key_value_store.o service_layer.o
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags
	
.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h command_line backend_store_server service_layer_server key_value_store_tests service_layer_tests