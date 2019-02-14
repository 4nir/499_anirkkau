#ifndef SERVICE_SERVER_H
#define SERVICE_SERVER_H

#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "server_clients.h"

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

// Logic and data behind the server's behaviour - add implementation here.
class ServiceLayerServiceImpl final : public ServiceLayer::Service {

  public:
    Status registeruser(ServerContext* context, const RegisterRequest* request,
                    RegisterReply* reply);
    Status chirp(ServerContext* context, const ChirpRequest* request,
                    ChirpReply* reply);
    Status follow(ServerContext* context, const FollowRequest* request,
                    FollowReply* reply);
    Status read(ServerContext* context, const ReadRequest* request,
    ReadReply* reply);
    
};


#endif


