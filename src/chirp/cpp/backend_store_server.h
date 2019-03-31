#ifndef BACKEND_SERVER_H
#define BACKEND_SERVER_H

#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "service_layer_client.h"
#include "key_value_store.h"

class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
  
  public:
  
    Status put(ServerContext* context, const PutRequest* request,
                    PutReply* reply);
    Status get(ServerContext* context,
                    ServerReaderWriter<GetReply, GetRequest>* stream);
    Status deletekey(ServerContext* context, const DeleteRequest* request,
                    DeleteReply* reply);
    std::vector<std::string>* DFSReplyThread(std::map<std::string, std::vector<std::string> > chirp_map,
                                             std::vector<std::string> *reply_thread_vec,
                                             std::string chirp_id);
  private:
    KeyValueStoreClass store_;
};

#endif //BACKEND_SERVER_H