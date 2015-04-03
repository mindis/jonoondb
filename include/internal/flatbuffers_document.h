#pragma once

#include <memory>
#include "document.h"
#include "flatbuffers/idl.h"

namespace jonoondb_api {

// Forward declarations
class Status;

class FlatbuffersDocument final : public Document {
 public:
  static Status Construct(const char* schema, int schemaID,
                          const Buffer& buffer,
                          FlatbuffersDocument*& flatbuffersDocument);

  Status GetScalarValueAsInt8(const char* fieldName, int8_t& val) const
      override;
  Status GetScalarValueAsInt16(const char* fieldName, int16_t& val) const
      override;
  Status GetScalarValueAsInt32(const char* fieldName, int32_t& val) const
      override;
  Status GetScalarValueAsInt64(const char* fieldName, int64_t& val) const
      override;

  Status GetScalarValueAsUInt8(const char* fieldName, uint8_t& val) const
      override;
  Status GetScalarValueAsUInt16(const char* fieldName, uint16_t& val) const
      override;
  Status GetScalarValueAsUInt32(const char* fieldName, uint32_t& val) const
      override;
  Status GetScalarValueAsUInt64(const char* fieldName, uint64_t& val) const
      override;

  Status GetScalarValueAsFloat(const char* fieldName, float& val) const
      override;
  Status GetScalarValueAsDouble(const char* fieldName, double& val) const
      override;

  Status GetStringValue(const char* fieldName, char*& val) const override;
  Status GetDocumentValue(const char* fieldName, Document*& val) const override;
  Status AllocateSubDocument(Document*& doc) const override;
  void Dispose() override;

  std::unique_ptr<flatbuffers::DynamicTableReader> m_dynTableReader;

 private:
  FlatbuffersDocument(
      std::shared_ptr<flatbuffers::Parser> parser,
      std::unique_ptr<flatbuffers::DynamicTableReader> dynTableReader);
  Status GetMissingFieldErrorStatus(const char* fieldName) const;
  std::shared_ptr<flatbuffers::Parser> m_parser;

};

}  // jonoondb_api