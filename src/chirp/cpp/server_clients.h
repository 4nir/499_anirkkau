#ifndef SERVER_CLIENTS_H
#define SERVER_CLIENTS_H

#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <sstream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
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
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::DeleteRequest;
using chirp::DeleteReply;

using chirp::ServiceLayer;
using chirp::Timestamp;
using chirp::Chirp;
using chirp::RegisterRequest;
using chirp::RegisterReply;
using chirp::ChirpRequest;
using chirp::ChirpReply;
using chirp::FollowRequest;
using chirp::FollowReply;
using chirp::ReadRequest;
using chirp::ReadReply;
using chirp::MonitorRequest;
using chirp::MonitorReply;

class HelperFunctions {
  public:
     std::vector<std::string>* DFSReplyThread(std::map<std::string, std::vector<std::string> > chirpMap,
                                             std::vector<std::string> *reply_thread_vec,
                                             std::string chirp_id);
    std::string GenerateChirpID();
  private:
     static int chirp_count;
};

class KeyValueStoreClient {
  public:
    std::string put(const std::string& key, const std::string& value, const std::string& type);

    std::vector<Chirp> get(const std::string& key);

    std::string deletekey(const std::string& key);

    GetRequest MakeGetRequest(const std::string& key);

    KeyValueStoreClient(std::shared_ptr<Channel> channel) 
      : stub_(KeyValueStore::NewStub(channel)) {}

  private:
    std::unique_ptr<KeyValueStore::Stub> stub_;
};

class ServiceLayerClient {
  public:
    int chirp_count = 0;
    std::string registeruser(const std::string& username);
    std::string chirp(const std::string& username, const std::string& text, 
                      const std::string& parent_id);
    std::string follow(const std::string& username, const std::string& to_follow);
    std::string read(const std::string& id);
    std::string monitor(const std::string& username);

    ServiceLayerClient(std::shared_ptr<Channel> channel)
        : stub_(ServiceLayer::NewStub(channel)) {}

  private:
    std::unique_ptr<ServiceLayer::Stub> stub_;
};

#endif