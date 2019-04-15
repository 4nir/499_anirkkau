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
#include "service_layer_client.h"

// ServiceLayer server
class ServiceLayerServiceImpl final : public ServiceLayer::Service {
 public:
  // Handles registeruser command received from ServiceLayerClient
  Status registeruser(ServerContext* context, const RegisterRequest* request,
                      RegisterReply* reply);

  // Handles chirp command received from ServiceLayerClient
  Status chirp(ServerContext* context, const ChirpRequest* request,
               ChirpReply* reply);

  // Handles follow command received from ServiceLayerClient
  Status follow(ServerContext* context, const FollowRequest* request,
                FollowReply* reply);

  // Handles read command received from ServiceLayerClient
  Status read(ServerContext* context, const ReadRequest* request,
              ReadReply* reply);

  // Handles monitor command received from ServiceLayerClient
  Status monitor(ServerContext* context, const MonitorRequest* request,
                 ServerWriter<MonitorReply>* writer);

  // Handles stream command received from ServiceLayerClient
  Status stream(ServerContext* context, const StreamRequest* request,
                ServerWriter< ::chirp::StreamReply>* writer) override;

  // Generates chirp ID to be stored in KVS
  // @return: returns unique chirp ID in string form
  std::string GenerateChirpID();

 private:
  // Keeps track of every chirp (for Monitor)
  std::vector<std::string> chirp_log_;

  // Count of chirps
  int chirp_count_ = 0;
};

#endif  // SERVICE_SERVER_H
