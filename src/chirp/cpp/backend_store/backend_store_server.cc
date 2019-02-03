#ifndef BACKEND_SERVER
#define BACKEND_SERVER

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"

#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::DeleteRequest;
using chirp::DeleteReply;

GetReply MakeGetReply(const std::string& value) {
  GetReply reply;
  reply.set_value(value);
  return reply;
}

// Logic and data behind the server's behaviour - add implementation here.
class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
    Status put(ServerContext* context, const PutRequest* request,
                    PutReply* reply) {
      // Hash map
      std::string key = request->key();
      std::string value = request->value();
      chirpMap.insert(std::make_pair(key, value));
      return Status::OK;
    }

    Status get(ServerContext* context,
                    ServerReaderWriter<GetReply, GetRequest>* stream) {
      
      std::vector<GetRequest> received_requests;
      GetRequest request_catcher;
      while (stream->Read(&request_catcher)){
        std::cout << "Server recieved: " << request_catcher.key() << std::endl;
        received_requests.push_back(request_catcher);
        // Perform BackEndStore lookup
      }

      for (GetRequest req : received_requests) {
        std::string key_requested = req.key();
        std::string value_requested;
        std::map<std::string, std::string>::iterator it = chirpMap.find(key_requested);

        if(it != chirpMap.end())
        {
          //element found;
          value_requested = it->second;
          std::cout << "Key: " << key_requested << "; Value: " << value_requested << std::endl;
          GetReply reply = MakeGetReply(value_requested);
          stream->Write(reply);
        } else {
          value_requested = "Not found.";
          std::cout << "Key: " << key_requested << "; Value: " << value_requested << std::endl;
        }
      }

      return Status::OK;
    }

    // TODO; Implement deletekey()
    Status deletekey(ServerContext* context, const DeleteRequest* request,
                    DeleteReply* reply) {
      std::cout << "Deleted key and value pair." << std::endl;
      return Status::OK;
    }
  private:
    std::map<std::string, std::string> chirpMap;
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


