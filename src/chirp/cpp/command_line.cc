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

DEFINE_bool(register, false, "user wants to register");
DEFINE_string(user, "", "username");
DEFINE_bool(follow, false, "user wants to follow");
DEFINE_bool(chirp, false, "user wants to post chirp");
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

  if(FLAGS_command.find("register") != string::npos){
      service_client.registeruser(argv[1]);
   }
   if(FLAGS_command.find("chirp") != string::npos){

     // Root Chirp
      std::string username = FLAGS_user;
      std::string text(argv[1]);
      service_client.chirp(username, text, "0");

      // Reply Chirp
   }
  if(FLAGS_command.find("follow") != string::npos){
      service_client.follow(argv[1], argv[2]);
   }

  // service_client.chirp(username, "Lemons", chirp_id_1, "0");
  // service_client.chirp(username, "Are", chirp_id_2, chirp_id_1);
  // service_client.chirp(username, "Evil", chirp_id_3, chirp_id_1);
  // service_client.chirp(username, "True", chirp_id_4, chirp_id_2);
  // // service_client.chirp(username, "End", chirp_id_5, chirp_id_3);
  // service_client.chirp(username, "WHACK", chirp_id_6, chirp_id_1);
  // service_client.chirp(username, "Sniper Rifle", chirp_id_7, chirp_id_4);

  // service_client.read(chirp_id_1);


  return 0;
}