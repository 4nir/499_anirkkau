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
#include "service_layer.h"

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  KeyValueStoreClient store_client(grpc::CreateChannel(
      "localhost:50052", grpc::InsecureChannelCredentials()));  

  ServiceLayerClient service_client(grpc::CreateChannel(
  "localhost:50051", grpc::InsecureChannelCredentials()));  

  //TODO: Delete test_chirp_str
  std::string test_chirp_str = "Hey from Client";

  store_client.put("Dick Grayson", test_chirp_str);
  store_client.put("Jason Todd", test_chirp_str);
  store_client.put("Tim Drake", test_chirp_str);
  std::string reply = service_client.registeruser("Damian Wayne");
  std::cout << "Greeter received: " << reply << std::endl;

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