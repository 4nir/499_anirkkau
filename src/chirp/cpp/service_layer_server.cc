#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "service_layer_server.h"
#include "service_layer.h"

// Logic and data behind the server's behaviour - add implementation here.
Status ServiceLayerServiceImpl::registeruser(ServerContext* context, const RegisterRequest* request,
                RegisterReply* reply) {
  // Register Implementation
  std::cout << "Service Layer Server Pinged" << std::endl;

  KeyValueStoreClient store_client(grpc::CreateChannel(
    "localhost:50052", grpc::InsecureChannelCredentials()));
     
  store_client.put("1", request->username());
  return Status::OK;
}

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  ServiceLayerServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}


