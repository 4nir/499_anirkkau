#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "service_layer_server.h"
#include "server_clients.h"

// Logic and data behind the server's behaviour - add implementation here.
Status ServiceLayerServiceImpl::registeruser(ServerContext* context, const RegisterRequest* request,
                RegisterReply* reply) {
  // Register Implementation

  KeyValueStoreClient store_client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string new_user = request->username();

  // "" in the value field signals to the KVS server it's a new user
  store_client.put(new_user, "register", "register");
  return Status::OK;
}

Status ServiceLayerServiceImpl::chirp(ServerContext* context, const ChirpRequest* request,
                    ChirpReply* reply) {

  KeyValueStoreClient store_client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  HelperFunctions helper;
  std::string chirp_username = request->username();
  std::string chirp_text = request->text();
  std::string chirp_id = helper.GenerateChirpID();
  std::string chirp_parent_id = request->parent_id();

  // Generate Chirp
  Chirp chirp;
  chirp.set_username(chirp_username);
  chirp.set_text(chirp_text);
  chirp.set_id(chirp_id);
  chirp.set_parent_id(chirp_parent_id);

  // Serialize Chirp
  std::string chirp_str = chirp.SerializeAsString();

  // // Test Chirp Serialization
  // std::cout << "chirp_str: ";
  // std::for_each(chirp_str.begin(), chirp_str.end(),
  //               [](unsigned char i) { std::cout << std::hex << std::uppercase << (int) i; });
  // std::cout << std::endl;
  std::string type = "chirp";
  store_client.put(chirp_id, chirp_str, type);
  
  return Status::OK;
}

Status ServiceLayerServiceImpl::follow(ServerContext* context, const FollowRequest* request,
                    FollowReply* reply){
  // Follow Implementation

  KeyValueStoreClient store_client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string username = request->username();
  std::string to_follow = request->to_follow();

  // "" in the value field signals to the KVS server it's a new user
  std::string type = "follow";
  store_client.put(username, to_follow, "follow");
  return Status::OK;
}

Status ServiceLayerServiceImpl::read(ServerContext* context, const ReadRequest* request,
                    ReadReply* reply) {

  KeyValueStoreClient store_client(grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials()));
  std::string chirp_id = request->chirp_id();

  std::vector<Chirp> chirp_thread;
  chirp_thread = store_client.get(chirp_id);
  //Loop over 
  for (Chirp chirp : chirp_thread){
    std::cout << chirp.text() << std::endl;
  }

  //TODO: Add thread to reply object

  return Status::OK;
}



void RunServer() {
  std::string server_address("0.0.0.0:50002");
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


