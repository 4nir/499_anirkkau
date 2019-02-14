#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include "backend_store.grpc.pb.h"
#include "backend_store_server.h"
#include "server_clients.h"

int main(int argc, char** argv) {
  std::string username = "anir";
  
  HelperFunctions helper;

  std::string chirp_id_1 = "cid//10001";
  std::string chirp_id_2 = "cid//10002";
  std::string chirp_id_3 = "cid//10003";
  std::string chirp_id_4 = "cid//10004";
  std::string chirp_id_5 = "cid//10005";
  std::string chirp_id_6 = "cid//10006";
  std::string chirp_id_7 = "cid//10007";

  ServiceLayerClient service_client(grpc::CreateChannel(
  "localhost:50002", grpc::InsecureChannelCredentials()));  

  service_client.registeruser("Damian Wayne");
  service_client.chirp(username, "Lemons", chirp_id_1, "0");
  service_client.chirp(username, "Are", chirp_id_2, chirp_id_1);
  service_client.chirp(username, "Evil", chirp_id_3, chirp_id_1);
  service_client.chirp(username, "True", chirp_id_4, chirp_id_2);
  // service_client.chirp(username, "End", chirp_id_5, chirp_id_3);
  service_client.chirp(username, "WHACK", chirp_id_6, chirp_id_1);
  service_client.chirp(username, "Sniper Rifle", chirp_id_7, chirp_id_4);

  service_client.read(chirp_id_1);


  return 0;
}

//==========================SERIALIZATION QUICK TESTS====================//

  // // Serialize Chirp to be stored
  // std::string chirp_owner = "John";
  // chirp::ChirpMessage chirp;
  // chirp.set_message("Hello World.");

  // std::string chirp_str = chirp.SerializeAsString();

  // // Test Chirp Serialization
  // std::cout << "chirp_str: ";
  // std::for_each(chirp_str.begin(), chirp_str.end(),
  //               [](unsigned char i) { std::cout << std::hex << std::uppercase << (int) i; });
  // std::cout << std::endl;