#ifndef BACKEND_STORE_CLIENT_H
#define BACKEND_STORE_CLIENT_H

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"

// Client Includes
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

// Server Includes
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

// Chirp Includes
using chirp::DeleteReply;
using chirp::DeleteRequest;
using chirp::GetReply;
using chirp::GetRequest;
using chirp::KeyValueStore;
using chirp::PutReply;
using chirp::PutRequest;

using chirp::Chirp;
using chirp::ChirpReply;
using chirp::ChirpRequest;
using chirp::FollowReply;
using chirp::FollowRequest;
using chirp::MonitorReply;
using chirp::MonitorRequest;
using chirp::ReadReply;
using chirp::ReadRequest;
using chirp::RegisterReply;
using chirp::RegisterRequest;
using chirp::ServiceLayer;
using chirp::Timestamp;

// gRPC client to communicate with KeyValueServiceImpl
class KeyValueStoreClient {
 public:
  // Puts a key-value pair into KVS
  // @key: key that will be put into KVS
  // @value: value that will be put into KVS
  // @type: provides extra information for the KVS server
  //@return: string representing success or failure of put()
  std::string put(const std::string& key, const std::string& value,
                  const std::string& type);

  // Helps set up GetRequests for client-server interaction
  GetRequest MakeGetRequest(const std::string& key);

  // Gets a vector of values from KVS
  // @key: key that will be used to get
  // @value: value that will be used to get
  // @type: provides extra information for the KVS server
  //@return: vector of chirps for a given key
  std::vector<Chirp> get(const std::string& key, const std::string& type);

  // For Monitor: Retrieves list of username a user follows
  // @key: key that will be used to query KVS
  // @type: provides extra information for the KVS server
  //@return: vector of strings that represent usernames that the key follows
  std::vector<std::string> getFollowingList(const std::string& key,
                                            const std::string& type);

  // Deletes a key from the KVS
  // @key: key that will be used to delete
  //@return: string representing success or failure of deletekey()
  std::string deletekey(const std::string& key);

  KeyValueStoreClient(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}

 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

#endif  // BACKEND_STORE_CLIENT_H