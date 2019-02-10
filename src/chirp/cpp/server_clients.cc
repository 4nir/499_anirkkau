#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include "backend_store.grpc.pb.h"
#include "service_layer.grpc.pb.h"
#include "server_clients.h"

//-------------------------KeyValueStoreClient-------------------------//

GetRequest KeyValueStoreClient::MakeGetRequest(const std::string& key){
  GetRequest req;
  req.set_key(key);
  return req;
}

// rpc put
std::string KeyValueStoreClient::put(const std::string& key, const std::string& value) {

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;
  
  if(value == ""){
    // "" signals new user to register
    context.AddMetadata("type", "register");
  } else {
    context.AddMetadata("type", "chirp");
  }

  // Data we are sending to the server.
  PutRequest request;
  request.set_key(key);
  request.set_value(value);

  // Container for the data we expect from the server.
  PutReply reply;

  // The actual RPC.
  Status status = stub_->put(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return "RPC succeeded";
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }
}

// rpc get (TODO: Implement stream functionality)
std::string KeyValueStoreClient::get(const std::string& key) {

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;


  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
  stub_->get(&context));

  GetRequest r1 = MakeGetRequest(key);

  std::thread writer([&]() {
        std::vector<GetRequest> requests{r1};
        for (const GetRequest& req : requests) {
          std::cout << "Sending message: " << req.key()
                    << std::endl;
          stream->Write(req);
        }
        stream->WritesDone();
  });

  // Container for the data we expect from the server.
  GetReply reply;
  while (stream->Read(&reply)) {
    std::cout << reply.value() << std::endl;
  }

  writer.join();
  Status status = stream->Finish();
  
  // Act upon its status.
  if (status.ok()) {
    return "GET client-stream recev worked!";
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
    return "GET client-stream recev failed.";
  }
}

// rpc deletekey
std::string KeyValueStoreClient::deletekey(const std::string& key) {
  // Data we are sending to the server.
  DeleteRequest request;
  request.set_key(key);

  // Container for the data we expect from the server.
  DeleteReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->deletekey(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return "RPC succeeded";
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }
}

//--------------------------ServiceLayerClient--------------------------//


// rpc registeruser
    std::string ServiceLayerClient::registeruser(const std::string& username) {

      // Context for the client. It could be used to convey extra information to
      // the server and/or tweak certain RPC behaviors.
      ClientContext context;

      // Data we are sending to the server.
      RegisterRequest request;
      request.set_username(username);

      // Container for the data we expect from the server.
      RegisterReply reply;

      // The actual RPC.
      Status status = stub_->registeruser(&context, request, &reply);

      // Act upon its status.
      if (status.ok()) {
        return "RPC succeeded";
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
      }
    }

    std::string ServiceLayerClient::chirp(const std::string& username, const std::string& text){
      // Context for the client. It could be used to convey extra information to
      // the server and/or tweak certain RPC behaviors.
      ClientContext context;

      // Data we are sending to the server.
      ChirpRequest request;
      request.set_username(username);
      request.set_text(text);

      // Container for the data we expect from the server.
      ChirpReply reply;

      // The actual RPC.
      Status status = stub_->chirp(&context, request, &reply);

      // Act upon its status.
      if (status.ok()) {
        return "RPC succeeded";
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
      }
    }

    std::string ServiceLayerClient::read(const std::string& id){
      // Context for the client. It could be used to convey extra information to
      // the server and/or tweak certain RPC behaviors.
      ClientContext context;

      // Data we are sending to the server.
      ReadRequest request;
      request.set_chirp_id(id);

      // Container for the data we expect from the server.
      ReadReply reply;

      // The actual RPC.
      Status status = stub_->read(&context, request, &reply);

      // Act upon its status.
      if (status.ok()) {
        return "RPC succeeded";
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
      }
    }

//--------------------------HelperFunctions--------------------------//

std::string HelperFunctions::GenerateRandomChirpID(){
  srand(time(NULL));

  const std::string CurrentClientID = "cid//";  
  std::ostringstream os;
  for (int i = 0; i < 7; ++i)
  {
      int digit = rand() % 10;
      os << digit;
  }
  std::string result = CurrentClientID + os.str();

  return result;
}
