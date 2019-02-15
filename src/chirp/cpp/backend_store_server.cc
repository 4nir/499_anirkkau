#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_server.h"
#include "server_clients.h"

// Logic and data behind the server's behaviour - add implementation here.
    Status KeyValueStoreServiceImpl::put(ServerContext* context, const PutRequest* request,
                    PutReply* reply) {

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
      // std::cout << "PUT Metadata type: " << type << std::endl;

      if(type == "register"){

      //If User (USR)
        auto it = chirpMap.find(key);
        if(it == chirpMap.end())
          {
            //Username doesn't already exist;
            std::vector<std::string> value_list;
            chirpMap.insert(std::make_pair(key, value_list));
            std::cout << "Registered new user: " << key << std::endl;
            
          } else {
            std::cout << "Error: Username already exists." << std::endl;
          }
      } else if(type == "chirp") {
        //If Chirp ID (CID)
        Chirp chirp_catcher;
        chirp_catcher.ParseFromString(value);
        std::string chirp_id = chirp_catcher.id();
        std::string chirp_parent_id = chirp_catcher.parent_id();
        // std::cout << "chirp_catcher.id(): " << chirp_id << std::endl;
        // std::cout << "chirp_catcher.parent_id(): " << chirp_parent_id << std::endl;

        // For chirps in chirpMap, index 0 contains byte string form, rest are serialized chirp replies

        if(chirp_parent_id == "0"){                               // Root chirp 

          // Store chirp id : fresh reply vector
          std::vector<std::string> value_list;
          value_list.push_back(value);
          chirpMap.insert(std::make_pair(key, value_list));
          std::cout << "Inserted Key: " << key  << std::endl;
        } else {                                                   //  Reply chirp
          // Step 1: Append chirp bytes to parent chirp's reply vector
          auto it = chirpMap.find(chirp_parent_id);
          if(it != chirpMap.end())
          {
            (it->second).push_back(key);
          } else {
            std::cout << "Error: Parent ID not found in map." << std::endl;
          }

          // Step 2: Store chirp id : fresh reply vector
          std::vector<std::string> value_list;
          value_list.push_back(value);
          chirpMap.insert(std::make_pair(key, value_list));
          std::cout << "Inserted Key: " << key  << std::endl;
        }
      } else if (type == "follow") { // otherwise, it's an append to a following list
          std::string user_to_follow = value;
          auto it = chirpMap.find(key);
          if(it != chirpMap.end())
          {
            std::vector<std::string> value_list = it->second;
            value_list.push_back(user_to_follow);
            chirpMap.erase(key);
            chirpMap.insert(std::make_pair(key, value_list));

            // Test following list
            std::cout << "User " << key << " now following: ";
            for(int i = 0; i < value_list.size(); i++){
              std::cout << value_list.at(i) << " ";
            }
            std::cout << "\n";
            
          } else {
            std::cout << "Error: Username doesn't exist." << std::endl;
          }

      } else {
        std::cout << "Error: Invalid type @ store_server PUT" << std::endl;
      }
      return Status::OK;
    }

    Status KeyValueStoreServiceImpl::get(ServerContext* context,
                    ServerReaderWriter<GetReply, GetRequest>* stream) {
      // Get type from metadata
      std::multimap< grpc::string_ref, grpc::string_ref > metadata = context->client_metadata();
      std::string type;
      auto data_iter = metadata.find("type");
        if(data_iter != metadata.end())
          {
            //Found type;
            std::string s((data_iter->second).data(), (data_iter->second).length());
            type = s;
          } else {
            std::cout << "Error: No metadata." << std::endl;
          }
      

      std::vector<GetRequest> received_requests;
      GetRequest request_catcher;
      while (stream->Read(&request_catcher)){
        received_requests.push_back(request_catcher);
      }

      for (GetRequest req : received_requests) {
        std::string key_requested = req.key();
        std::map<std::string, std::vector<std::string> >::iterator it = chirpMap.find(key_requested);

        if(it == chirpMap.end())
        {
          std::cout << "Error: Key not found. " << std::endl;
        } else {
          //element found;

          //TDOD: Depending on type, modify GET behaviour (read vs. monitor)

          if(type == "read"){
            // DFSSearch that returns vector of entire thread (of chirp bytes)
            std::vector<std::string>* reply_thread_vec = new std::vector<std::string>();
            std::vector<std::string>* full_thread_vec;
            HelperFunctions helper;
            full_thread_vec = helper.DFSReplyThread(chirpMap, reply_thread_vec, key_requested);
            GetReply reply;
            std::string value_requested;
            for(int i = 0; i < full_thread_vec->size(); i++){
              value_requested = full_thread_vec->at(i);
              reply.set_value(value_requested);
              stream->Write(reply);
            }
          } else if(type == "monitor"){
              auto it = chirpMap.find(key_requested);
              if(it != chirpMap.end())
              {
                  std::vector<std::string> follow_list = it->second;
                  GetReply reply;
                  for(int i = 0; i < follow_list.size(); i++){
                  std::string value_requested = follow_list.at(i);
                  reply.set_value(value_requested);
                  stream->Write(reply);
                }
              } else {
                std::cout << "Error: Username doesn't exist." << std::endl;
              }

          } else {
            std::cout << "Error: Invalid type at KVS Server READ" << std::endl;
          }
          // delete reply_thread_vec;
        }
      }

      return Status::OK;
    }

    Status KeyValueStoreServiceImpl::deletekey(ServerContext* context, const DeleteRequest* request,
                    DeleteReply* reply) {
      chirpMap.erase(request->key());
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


