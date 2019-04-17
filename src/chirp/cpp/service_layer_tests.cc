#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "backend_store.grpc.pb.h"
#include "key_value_store.h"
#include "service_layer.grpc.pb.h"
#include "service_layer.h"
using chirp::Chirp;
using chirp::Timestamp;

// Tests default constructor for ServiceLayer
TEST(ServiceLayerCreate, NewServiceLayer) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_NE(nullptr, &s);
}

// Tests Register method for ServiceLayer
// Registers same user twice, should fail on second attempt
TEST(ServiceLayerRegister, DoubleRegister) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  EXPECT_EQ(true, s.registeruser("alpha"));
  ASSERT_FALSE(s.registeruser("alpha"));
}

// Tests Register method for ServiceLayer
// Registers with empty username, should fail
TEST(ServiceLayerRegister, RegisterEmpty) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_FALSE(s.registeruser(""));
}

// Tests Follow method for ServiceLayer
// "robin" should now follow "batman", based on bool return of follow()
TEST(ServiceLayerFollow, BaseFollow) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_TRUE(s.registeruser("batman"));
  ASSERT_TRUE(s.registeruser("robin"));

  EXPECT_TRUE(s.follow("robin", "batman"));
}

// Tests Follow method for ServiceLayer; User follows 3 people
// "batman" should now follow "robin", "nightwing", annd "red hood"
TEST(ServiceLayerFollow, FollowThreeUsers) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_TRUE(s.registeruser("batman"));
  ASSERT_TRUE(s.registeruser("robin"));
  ASSERT_TRUE(s.registeruser("nightwing"));
  ASSERT_TRUE(s.registeruser("red hood"));

  EXPECT_TRUE(s.follow("batman", "robin"));
  EXPECT_TRUE(s.follow("batman", "nightwing"));
  EXPECT_TRUE(s.follow("batman", "red hood"));
}

// Tests Follow method for ServiceLayer; User follows non-existent username
// Should return false
TEST(ServiceLayerFollow, FollowNonexistent) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_TRUE(s.registeruser("batman"));

  EXPECT_FALSE(s.follow("batman", "joker"));
}

// Tests Follow method for ServiceLayer; Non-existent user tries to call
// follow() Should return false
TEST(ServiceLayerFollow, UserNonexistent) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  ASSERT_TRUE(s.registeruser("batman"));
  ASSERT_TRUE(s.registeruser("joker"));

  EXPECT_FALSE(s.follow("manbat", "joker"));
}

// Tests Read method for ServiceLayer
// Chirps are individual Root chirps
TEST(ServiceLayerRead, ReadThreeRootChirps) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("alpha");
  s.chirp("alpha", "hey1", "0");
  s.chirp("alpha", "hey2", "0");
  s.chirp("alpha", "hey3", "0");
  auto replies = s.read("cid//1");
  ASSERT_EQ(1, replies.size());
}

// Tests Read method for ServiceLayer
// Chirps are a reply chain of 3
TEST(ServiceLayerRead, ReadThreeReplyChirps) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("alpha");
  s.chirp("alpha", "chirp1", "0");
  s.chirp("alpha", "chirp2", "cid//1");
  s.chirp("alpha", "chirp3", "cid//1");
  auto replies = s.read("cid//1");
  ASSERT_EQ(3, replies.size());
}

// Tests Read method for ServiceLayer
// Chirps are a reply chain of 3
// We expect the the chirps to be read in
// the proper order
TEST(ServiceLayerRead, ReadOrderedReplies) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("alpha");
  s.chirp("alpha", "chirp1", "0");
  s.chirp("alpha", "chirp2", "cid//1");
  s.chirp("alpha", "chirp3", "cid//1");
  auto replies = s.read("cid//1");
  Chirp chirp_obj_1 = replies.at(0);
  Chirp chirp_obj_2 = replies.at(1);
  Chirp chirp_obj_3 = replies.at(2);
  EXPECT_EQ("chirp1", chirp_obj_1.text());
  EXPECT_EQ("chirp2", chirp_obj_2.text());
  EXPECT_EQ("chirp3", chirp_obj_3.text());
}

// Tests Read method for ServiceLayer
// Chirps has no replies
// Should return a thread of only the original chirp
TEST(ServiceLayerRead, ReadChirpWithNoReplies) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("alpha");
  s.chirp("alpha", "chirp1", "0");
  s.chirp("alpha", "chirp2", "0");
  s.chirp("alpha", "chirp3", "0");
  auto replies = s.read("cid//1");
  EXPECT_EQ(1, replies.size());
}

// Tests Read method for ServiceLayer
// The chain of 5 chirps will be a complex reply tree
// by two different users
// We expect the the chirps to be read in
// the proper order
TEST(ServiceLayerRead, ReadComplexOrderedReplies) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("alpha");
  s.registeruser("beta");
  s.chirp("alpha", "chirp1", "0");
  s.chirp("beta", "chirp2", "cid//1");
  s.chirp("alpha", "chirp3", "cid//1");
  s.chirp("beta", "chirp4", "cid//2");
  s.chirp("beta", "chirp5", "cid//4");

  auto replies = s.read("cid//1");

  Chirp chirp_obj_1 = replies.at(0);
  Chirp chirp_obj_2 = replies.at(1);
  Chirp chirp_obj_3 = replies.at(2);
  Chirp chirp_obj_4 = replies.at(3);
  Chirp chirp_obj_5 = replies.at(4);

  EXPECT_EQ("chirp1", chirp_obj_1.text());
  EXPECT_EQ("chirp2", chirp_obj_2.text());
  EXPECT_EQ("chirp4", chirp_obj_3.text());
  EXPECT_EQ("chirp5", chirp_obj_4.text());
  EXPECT_EQ("chirp3", chirp_obj_5.text());
}

// Tests Monitor method for ServiceLayer
// Invalid Username
TEST(ServiceLayerMonitor, MonitorInvalidUsername) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("robin");
  auto monitor_vec_1 = s.monitor("batman");  // vector should be empty
  EXPECT_EQ(0, monitor_vec_1.size());
}

// Tests Monitor method for ServiceLayer
// Since grpc isn't allowed, Monitor returns
// a vector of all relevent chirps since the first chirp
// if it called again, it the vector should contain
// any new chirps that were added in the between monitor calls
TEST(ServiceLayerMonitor, MonitorFunction) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("robin");
  s.registeruser("batman");
  auto monitor_vec_1 = s.monitor("batman");  // vector should be empty
  s.follow("batman", "robin");
  s.chirp("robin", "chirp1", "0");
  auto monitor_vec_2 = s.monitor("batman");  // vector should have 1 chirp

  EXPECT_EQ(0, monitor_vec_1.size());
  EXPECT_EQ(1, monitor_vec_2.size());
}

// Tests Monitor method for ServiceLayer
// Monitor returns
// a vector of all relevent chirps since the first chirp
// if it called again, it the vector should contain
// any new chirps that were added in the between monitor calls
TEST(ServiceLayerMonitor, MonitorComplexFunction) {
  KeyValueStoreClass store_client;
  ServiceLayer s(&store_client);
  s.registeruser("robin");
  s.registeruser("batman");
  auto monitor_vec_1 = s.monitor("batman");  // vector should be empty
  s.follow("batman", "robin");
  s.chirp("robin", "chirp1", "0");
  s.chirp("robin", "chirp2", "cid//1");
  s.chirp("robin", "chirp3", "cid//2");
  auto monitor_vec_2 = s.monitor("batman");  // vector should have 3 chirps

  EXPECT_EQ(0, monitor_vec_1.size());
  EXPECT_EQ(3, monitor_vec_2.size());
}

// Tests Stream(username, hashtag) method for ServiceLayer
// When a invalid `username` is given, stream method should return error
TEST(ServiceLayerStream, InvalidUserNameShouldReturnError) {}

// When a user that current `username` is following sent a chirp not containing
// `hashtag`, stream function should return a empty vector
TEST(ServiceLayerStream, NonRelatedChirpShouldNotStream) {}

// When a user that current `username` is following sent a chrip containing the
// `hashtag`, stream function should return a vector conains this chirp
TEST(ServiceLayerStream, SingleMatchingHashtagChirpShouldWork) {}

// When multiple users that `username` is following sent chirps containing the
// `hashtag`, stream function should return a vector conains those
// chirps
TEST(ServiceLayerStream, MultipleMatchingHashtagChirpShouldWork) {}

// When a user that current `username` is following sent a chrip containing
// multiple hashtags and one of them is `hashtag`, stream function should return
// a vector conains this chirp
TEST(ServiceLayerStream, ChirpWithMultipleHashtagShouldWorkForSingleMatching) {}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}