#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_server.h"
#include "server_clients.h"

using namespace std;

DEFINE_string(user, "", "username");
DEFINE_bool(register, false, "user wants to register");
DEFINE_bool(follow, false, "user wants to follow");
DEFINE_bool(chirp, false, "user wants to post chirp");
DEFINE_bool(reply, false, "user wants to reply to chirp");
DEFINE_bool(read, false, "user wants to read chirp thread");
DEFINE_string(command, "", "read command from command line");

int main(int argc, char** argv) {

  HelperFunctions helper;
  ServiceLayerClient service_client(grpc::CreateChannel(
  "localhost:50002", grpc::InsecureChannelCredentials())); 

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_register)
     FLAGS_command += "register";   // implied by --consider_made_up_languages
  if (FLAGS_chirp)
     FLAGS_command += "chirp";   // implied by --consider_made_up_languages
  if (FLAGS_follow)
     FLAGS_command += "follow";   // implied by --consider_made_up_languages
  if (FLAGS_read)
     FLAGS_command += "read";   // implied by --consider_made_up_languages

  if(FLAGS_command.find("register") != string::npos){
      service_client.registeruser(argv[1]);
   }

  if(FLAGS_user != ""){
    if(FLAGS_command.find("chirp") != string::npos){
      if((!FLAGS_reply) && argc == 2){
        // Root Chirp
      std::cout << "Root chirping" << std::endl;
      std::string username = FLAGS_user;
      std::string text(argv[1]);
      std::string output = service_client.chirp(username, text, "0");
      std::cout << output << std::endl;
      } else if (FLAGS_reply && argc == 3){
        // Reply Chirp
      std::cout << "Reply chirping" << std::endl;
      std::string username = FLAGS_user;
      std::string text(argv[1]);
      std::string reply_id(argv[2]);
      std::string output = service_client.chirp(username, text, reply_id);
      std::cout << output << std::endl;
      } else {
        std::cout << "Error: Invalid syntax for --chirp" << std::endl;
      }
  }
    if(FLAGS_command.find("follow") != string::npos){
        if(argc == 2){
          std::string username = FLAGS_user;
          std::string output = service_client.follow(username, argv[1]);
          std::cout << output << std::endl;
        }
    }

    if(FLAGS_command.find("read") != string::npos){
        std::string output = service_client.read(argv[1]);
        std::cout << output << std::endl;
    }
  } else {
    std::cout << "Error: User flag required" << std::endl;
  }


  return 0;
}