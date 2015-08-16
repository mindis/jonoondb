#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

namespace jonoondb_api {
// Forward declarations
class Status;
class IndexInfo;
class Indexer;
enum class FieldType
: std::int32_t;

class IndexerFactory {
 public:
  static Status CreateIndexer(
      const IndexInfo& indexInfo,
      std::unordered_map<std::string, FieldType>& fieldName, Indexer*& indexer);
 private:
  IndexerFactory() = delete;
  IndexerFactory(const IndexerFactory&) = delete;
  IndexerFactory(IndexerFactory&&) = delete;
};
}

