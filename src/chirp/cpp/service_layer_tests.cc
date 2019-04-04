#include <gtest/gtest.h>

#include "service_layer.h"
#include "key_value_store.h"

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
  s.chirp("alpha", "hey1", "0");
  s.chirp("alpha", "hey2", "cid//1");
  s.chirp("alpha", "hey3", "cid//1");
  auto replies = s.read("cid//1");
  ASSERT_EQ(3, replies.size());
}



int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}