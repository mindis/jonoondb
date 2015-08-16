#pragma once

#include <cstdint>

namespace jonoondb_api {
// Forward declaration
class Status;
enum class FieldType
: std::int32_t;
enum class SchemaType
: std::int32_t;
class Field;

class DocumentSchema {
 public:
  virtual ~DocumentSchema() {
  }
  virtual const char* GetSchemaText() const = 0;
  virtual SchemaType GetSchemaType() const = 0;
  virtual Status GetFieldType(const char* fieldName,
                              FieldType& fieldType) const = 0;
  virtual std::size_t GetRootFieldCount() const = 0;
  virtual Status GetRootField(size_t index, Field*& field) const = 0;
  virtual Status AllocateField(Field*& field) const = 0;
};
}
