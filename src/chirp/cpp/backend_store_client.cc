#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_client.h"
#include "service_layer.grpc.pb.h"
#include "service_layer_client.h"

// rpc put Implementation
std::string KeyValueStoreClient::put(const std::string& key,
                                     const std::string& value,
                                     const std::string& type) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  if (type == "register") {
    // "register" signals new user to register
    context.AddMetadata("type", "register");
    // "chirp" signals new chirp
  } else if (type == "chirp") {
    context.AddMetadata("type", "chirp");
  } else if (type == "follow") {  
    // "follow" signals an append to a following list
    context.AddMetadata("type", "follow");
  } else {
    std::cout << "Error: Invalid type @ store_client PUT" << std::endl;
  }

  // Data we are sending to the server.
  PutRequest request;
  request.set_key(key);
  request.set_value(value);

  // Container for the data we expect from the server.
  PutReply reply;

  // The actual RPC.
  Status status = stub_->put(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return "success";
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "failed";
  }
}

GetRequest KeyValueStoreClient::MakeGetRequest(const std::string& key) {
  GetRequest req;
  req.set_key(key);
  return req;
}

// rpc get Implementation
std::vector<Chirp> KeyValueStoreClient::get(const std::string& key,
                                            const std::string& type) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;
  if (type == "read") {
    context.AddMetadata("type", "read");
  } else {
    std::cout << "Error: Invalid metadata at KVS Client GET" << std::endl;
  }

  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
      stub_->get(&context));

  std::vector<GetRequest> requests;
  GetRequest r1 = MakeGetRequest(key);
  requests.push_back(r1);

  std::thread writer([&]() {
    for (const GetRequest& req : requests) {
      stream->Write(req);
    }
    stream->WritesDone();
  });

  // Container for the data we expect from the server.
  GetReply reply;
  std::vector<Chirp> chirp_thread;
  while (stream->Read(&reply)) {
    // Deserialize chirps
    std::string chirp_str = reply.value();
    Chirp chirp_catcher;
    chirp_catcher.ParseFromString(chirp_str);
    chirp_thread.push_back(chirp_catcher);
  }

  writer.join();
  Status status = stream->Finish();

  // Act upon its status.
  if (status.ok()) {
    return chirp_thread;
  } else {
    std::cout << "Error: Chirp ID not found." << std::endl;
    return chirp_thread;
  }
}

std::vector<std::string> KeyValueStoreClient::getFollowingList(
    const std::string& key, const std::string& type) {
  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;
  if (type == "monitor") {
    context.AddMetadata("type", "monitor");
  } else {
    std::cout << "Error: Invalid metadata at KVS Client getFollowingList"
              << std::endl;
  }

  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
      stub_->get(&context));

  std::vector<GetRequest> requests;
  GetRequest r1 = MakeGetRequest(key);
  requests.push_back(r1);

  std::thread writer([&]() {
    for (const GetRequest& req : requests) {
      stream->Write(req);
    }
    stream->WritesDone();
  });

  // Container for the data we expect from the server.
  GetReply reply;
  std::vector<std::string> following_list;
  while (stream->Read(&reply)) {
    following_list.push_back(reply.value());
  }

  writer.join();
  Status status = stream->Finish();

  // Act upon its status.
  if (status.ok()) {
    return following_list;
  } else {
    std::cout << "Error: Invalid chirp ID." << std::endl;
    return following_list;
  }
}

// rpc deletekey Implementation
std::string KeyValueStoreClient::deletekey(const std::string& key) {
  // Data we are sending to the server.
  DeleteRequest request;
  request.set_key(key);

  // Container for the data we expect from the server.
  DeleteReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->deletekey(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return "success";
  } else {
    return "failed";
  }
}