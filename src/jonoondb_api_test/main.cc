#include <iostream>
#include <exception>
#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include <memory>
#include <fstream>
#include <streambuf>
#include <boost/exception/exception.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/filesystem.hpp>
#include "test_utils.h"
#include "buffer_impl.h"
#include "enums.h"
#include "tweet_generated.h"
#include "database.h"
#include "test/test_config_generated.h"

using namespace std;
using namespace boost::filesystem;
using namespace jonoondb_test;
using namespace jonoondb_api;
using namespace flatbuffers;

namespace jonoondb_test {
string g_TestRootDirectory;
string g_ResourcesFolderPath;

std::string GetSchemaFilePath(const std::string& fileName) {
  return g_ResourcesFolderPath + "/jonoondb_api_test/" + fileName;
}

Buffer GetTweetObject2(std::size_t tweetId, std::size_t userId,
                       std::string& nameStr, std::string& textStr,
                       double rating) {
  // create user object
  FlatBufferBuilder fbb;
  auto name = fbb.CreateString(nameStr);
  auto user = CreateUser(fbb, name, userId);

  // create tweet
  auto text = fbb.CreateString(textStr);
  auto tweet = CreateTweet(fbb, tweetId, text, user, rating);

  fbb.Finish(tweet);
  auto size = fbb.GetSize();
  
  Buffer buffer((char*)fbb.GetBufferPointer(), size, size);
  return buffer;
}

BufferImpl GetTweetObject() {
  // create user object
  FlatBufferBuilder fbb;
  auto name = fbb.CreateString("Zarian");
  auto user = CreateUser(fbb, name, 1);

  // create tweet
  auto text = fbb.CreateString("Say hello to my little friend!");
  auto tweet = CreateTweet(fbb, 1, text, user);

  fbb.Finish(tweet);
  auto size = fbb.GetSize();
  return BufferImpl((char*)fbb.GetBufferPointer(), size, size);
}

void RemoveAndCreateFile(const char* path, size_t fileSize) {
  std::remove(path);
  std::unique_ptr<FILE, int (*)(FILE*)> file(std::fopen(path, "wb"),
                                             std::fclose);  // unique_ptr will call fclose on destruction
  std::unique_ptr<char> buffer(new char[fileSize]);
  std::fwrite(buffer.get(), sizeof(char), fileSize, file.get());
}
}  // namespace jonoondb_test

bool SetUpDirectory(const char* directoryPath) {
  try {
    auto a = remove_all(directoryPath);
    auto b = create_directory(directoryPath);
  } catch (boost::exception& ex) {
    cout
        << "Error occured while trying to remove and recreate the unit test folder at path "
        << directoryPath << endl;
    cout << "Error: " << diagnostic_information(ex) << endl;
    return false;
  } catch (std::exception& ex) {
    cout
        << "Error occured while trying to remove and recreate the unit test folder at path "
        << directoryPath << endl;
    cout << "Error: " << ex.what() << endl;
    return false;
  }

  return true;
}

int main(int argc, char **argv) {  
  path tempPath = TEST_FOLDER_PATH;  
  g_TestRootDirectory = tempPath.generic_string();
  tempPath = RESOURCES_FOLDER_PATH;
  g_ResourcesFolderPath = tempPath.generic_string();
  if (SetUpDirectory(g_TestRootDirectory.c_str())) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  } else {
    return -1;
  }
}
