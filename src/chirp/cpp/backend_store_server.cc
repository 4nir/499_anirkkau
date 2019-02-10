#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_server.h"

// Logic and data behind the server's behaviour - add implementation here.
    Status KeyValueStoreServiceImpl::put(ServerContext* context, const PutRequest* request,
                    PutReply* reply) {

      std::cout << "KVS Store Server PUT Pinged!" << std::endl;
      std::string key = request->key();
      std::string value = request->value();
      std::multimap< grpc::string_ref, grpc::string_ref > metadata = context->client_metadata();
      std::string type;

      // Get type from metadata
      auto data_iter = metadata.find("type");
        if(data_iter != metadata.end())
          {
            //Found type;
            std::string s((data_iter->second).data(), (data_iter->second).length());
            type = s;
          } else {
            std::cout << "Error: No metadata." << std::endl;
          }
      std::cout << "Metadata: " << type << std::endl;

      if(type == "register"){

      //If User (USR)
        std::map<std::string, std::vector<std::string> >::iterator it = chirpMap.find(key);
        if(it == chirpMap.end())
          {
            //Username doesn't already exist;
            std::vector<std::string> value_list;
            value_list.push_back(value);
            chirpMap.insert(std::make_pair(key, value_list));
            std::cout << "Inserted Key: " << key << "; Value: " << value << std::endl;
            
          } else {
            std::cout << "Error: Username already exists." << std::endl;
          }
      } else if(type == "chirp") {
            //If Chirp ID (CID)
            std::vector<std::string> value_list;
            value_list.push_back(value);
            chirpMap.insert(std::make_pair(key, value_list));
            std::cout << "Inserted Key: " << key << "; Value: " << value << std::endl;
      }
      return Status::OK;
    }

    Status KeyValueStoreServiceImpl::get(ServerContext* context,
                    ServerReaderWriter<GetReply, GetRequest>* stream) {

      std::cout << "KVS Store Server GET Pinged!" << std::endl;
      std::vector<GetRequest> received_requests;
      GetRequest request_catcher;
      while (stream->Read(&request_catcher)){
        std::cout << "Server recieved: " << request_catcher.key() << std::endl;
        received_requests.push_back(request_catcher);
        // Perform BackEndStore lookup
      }

      for (GetRequest req : received_requests) {
        std::string key_requested = req.key();
        std::vector<std::string> value_requested_list;
        std::map<std::string, std::vector<std::string> >::iterator it = chirpMap.find(key_requested);

        if(it != chirpMap.end())
        {
          //element found;
          GetReply reply;
          value_requested_list = it->second;
          for(std::string value_requested : value_requested_list){
            std::cout << "Key: " << key_requested << "; Value: " << value_requested << std::endl;
            reply.set_value(value_requested);
            stream->Write(reply);
          }
        } else {
          std::cout << "Key not found. " << std::endl;
        }
      }

      return Status::OK;
    }

    // TODO; Implement deletekey()
    Status KeyValueStoreServiceImpl::deletekey(ServerContext* context, const DeleteRequest* request,
                    DeleteReply* reply) {
      std::cout << "Deleted key and value pair." << std::endl;
      return Status::OK;
    }

void RunServer() {
  std::string server_address("0.0.0.0:50000");
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


