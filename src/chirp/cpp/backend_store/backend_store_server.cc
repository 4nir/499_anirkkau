#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using chirp::GetRequest;
using chirp::GetReply;
// using chirp::DeleteRequest;
// using chirp::DeleteReply;

// Logic and data behind the server's behaviour - add implementation here.
class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
  Status put(ServerContext* context, const PutRequest* request,
                  PutReply* reply) {
    std::cout << "Stored key and value pair." << std::endl;
    return Status::OK;
  }

  Status get(ServerContext* context, const GetRequest* request,
                  GetReply* reply) {
    std::string prefix("This is the value.");
    reply->set_value(prefix);               
    return Status::OK;
  }

  // Status deletekey(ServerContext* context, const DeleteRequest* request,
  //                 DeleteReply* reply) override {
  //   return Status::OK;
  // }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  KeyValueStoreServiceImpl service;

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


