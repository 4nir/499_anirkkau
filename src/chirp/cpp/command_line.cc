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

  ServiceLayerClient service_client(grpc::CreateChannel(
  "localhost:50002", grpc::InsecureChannelCredentials()));  

  service_client.registeruser("Damian Wayne");
  service_client.chirp(username, "Lemons");
  service_client.read("test1");


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