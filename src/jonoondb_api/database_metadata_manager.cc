#include <string>
#include <memory>
#include <sstream>
#include <boost/filesystem.hpp>
#include "sqlite3.h"
#include "database_metadata_manager.h"
#include "status.h"
#include "string_utils.h"
#include "exception_utils.h"
#include "sqlite_utils.h"
#include "exception_utils.h"
#include "index_info.h"
#include "serializer_utils.h"
#include "enums.h"

using namespace std;
using namespace boost::filesystem;
using namespace jonoondb_api;

DatabaseMetadataManager::DatabaseMetadataManager(const char* dbPath,
                                                 const char* dbName,
                                                 bool createDBIfMissing)
    : m_dbPath(dbPath),
      m_dbName(dbName),
      m_createDBIfMissing(createDBIfMissing),
      m_metadataDBConnection(nullptr) {
}

DatabaseMetadataManager::~DatabaseMetadataManager() {
  if (m_metadataDBConnection != nullptr) {
    if (sqlite3_close(m_metadataDBConnection) != SQLITE_OK) {
      //Todo: Handle SQLITE_BUSY response here
    }
    m_metadataDBConnection = nullptr;
  }
}

Status DatabaseMetadataManager::Initialize() {
  string errorMessage;

  path pathObj(m_dbPath);
  pathObj += m_dbName;
  pathObj += ".dat";

  m_fullDbPath = pathObj.string();

  if (!boost::filesystem::exists(pathObj) && !m_createDBIfMissing) {
    return Status(kStatusMissingDatabaseFileCode, errorMessage.c_str(),
                  (int32_t) errorMessage.length());
  }

  int sqliteCode = sqlite3_open(pathObj.string().c_str(),
                                &m_metadataDBConnection);  //, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  auto status = CreateTables();
  if (!status.OK()) {
    return status;
  }

  status = PrepareStatements();

  return status;
}

Status DatabaseMetadataManager::CreateTables() {
  // Set DB Pragmas
  int sqliteCode = 0;
  sqliteCode = sqlite3_exec(m_metadataDBConnection,
                            "PRAGMA synchronous = FULL;", 0, 0, 0);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  sqliteCode = sqlite3_exec(m_metadataDBConnection,
                            "PRAGMA journal_mode = WAL;", 0, 0, 0);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  sqliteCode = sqlite3_busy_handler(m_metadataDBConnection,
                                    SQLiteUtils::SQLiteGenericBusyHandler,
                                    nullptr);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  // Create the necessary tables if they do not exist
  string sql =
      "create table if not exists CollectionSchema(CollectionName text primary key, CollectionSchema text, CollectionSchemaType int)";
  auto status = SQLiteUtils::ExecuteSQL(m_metadataDBConnection, sql);
  if (!status.OK()) {
    return status;
  }

  sql =
      "create table if not exists CollectionIndex(IndexName text primary key, CollectionName text, IndexInfo blob)";
  status = SQLiteUtils::ExecuteSQL(m_metadataDBConnection, sql);
  if (!status.OK()) {
    return status;
  }

  //sql = "create table if not exists CollectionDocumentFile(FileKey int primary key, FileName text, FileDataLength int, foreign key(CollectionName) references CollectionMetadata(CollectionName))";
  sql =
      "create table if not exists CollectionDocumentFile(FileKey int primary key, FileName text, FileDataLength int, CollectionName text)";

  status = SQLiteUtils::ExecuteSQL(m_metadataDBConnection, sql);
  if (!status.OK()) {
    return status;
  }

  return Status();
}

Status DatabaseMetadataManager::PrepareStatements() {
  int sqliteCode =
      sqlite3_prepare_v2(
          m_metadataDBConnection,
          "insert into CollectionIndex (IndexName, CollectionName, IndexInfo) values (?, ?, ?)",  // stmt
          -1,  // If greater than zero, then stmt is read up to the first null terminator
          &m_insertCollectionIndexStmt,  //Statement that is to be prepared
          0  // Pointer to unused portion of stmt
          );

  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  sqliteCode =
      sqlite3_prepare_v2(
          m_metadataDBConnection,
          "insert into CollectionSchema (CollectionName, CollectionSchema, CollectionSchemaType) values (?, ?, ?)",  // stmt
          -1,  // If greater than zero, then stmt is read up to the first null terminator
          &m_insertCollectionSchemaStmt,  //Statement that is to be prepared
          0  // Pointer to unused portion of stmt
          );

  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  return Status();
}

Status DatabaseMetadataManager::Open(
    const char* dbPath, const char* dbName, bool createDBIfMissing,
    DatabaseMetadataManager*& databaseMetadataManager) {
  string errorMessage;

  // Validate function arguments
  if (StringUtils::IsNullOrEmpty(dbPath)) {
    errorMessage = "Argument dbPath is null or empty.";
    return Status(kStatusInvalidArgumentCode, errorMessage.c_str(),
                  errorMessage.length());
  }

  if (StringUtils::IsNullOrEmpty(dbName)) {
    errorMessage = "Argument dbName is null or empty.";
    return Status(kStatusInvalidArgumentCode, errorMessage.c_str(),
                  errorMessage.length());
  }

  unique_ptr<DatabaseMetadataManager> dbMetadataManager(
      new DatabaseMetadataManager(dbPath, dbName, createDBIfMissing));
  auto status = dbMetadataManager->Initialize();
  if (!status.OK()) {
    return status;
  }

  databaseMetadataManager = dbMetadataManager.release();

  return status;
}

Status DatabaseMetadataManager::AddCollection(const char* name,
                                              SchemaType schemaType,
                                              const char* schema,
                                              const IndexInfo indexes[],
                                              size_t indexesLength) {
  //statement guard will make sure that the statement is cleared and reset when statementGuard object goes out of scope
  unique_ptr<sqlite3_stmt, Status (*)(sqlite3_stmt*)> statementGuard(
      m_insertCollectionSchemaStmt, SQLiteUtils::ClearAndResetStatement);
  

  // 1. Prepare stmt to add data in CollectionSchema table
  int code = sqlite3_bind_text(m_insertCollectionSchemaStmt, 1,  // Index of wildcard
                                     name,  // CollectionName
                                     -1,  // length of the string is the number of bytes up to the first zero terminator
                                     SQLITE_STATIC);

  if (code != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);    

  code = sqlite3_bind_text(m_insertCollectionSchemaStmt, 2,  // Index of wildcard
                                 schema,  // CollectionSchema
                                 -1,  // length of the string is the number of bytes up to the first zero terminator
                                 SQLITE_STATIC);
  if (code != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);

  code = sqlite3_bind_int(m_insertCollectionSchemaStmt, 3,  // Index of wildcard
                                static_cast<int>(schemaType));
  if (code != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);

  // 2. Start Transaction before issuing insert
  code = sqlite3_exec(m_metadataDBConnection, "BEGIN", 0, 0, 0);
  if (code != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);

  code = sqlite3_step(m_insertCollectionSchemaStmt);
  if (code != SQLITE_DONE) {
    if (code == SQLITE_CONSTRAINT) {
      //Key already exists
      ostringstream ss;
      ss << "Collection with name \"" << name << "\" already exists.";
      std::string errorMsg = ss.str();
      sqlite3_exec(m_metadataDBConnection, "ROLLBACK", 0, 0, 0);
      return Status(kStatusCollectionAlreadyExistCode, errorMsg.c_str(),
                    errorMsg.length());
    } else {
      sqlite3_exec(m_metadataDBConnection, "ROLLBACK", 0, 0, 0);
      return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);
    }
  }

  // 3. Add all the collection indexes
  Status sts;
  for (int i = 0; i < indexesLength; i++) {
    sts = CreateIndex(name, indexes[i]);
    if (!sts) break;
  }

  //4. Commit or Rollback the transaction based on status of inserts
  if (!sts) {
    sqlite3_exec(m_metadataDBConnection, "ROLLBACK", 0, 0, 0);    
  } else {
    code = sqlite3_exec(m_metadataDBConnection, "COMMIT", 0, 0, 0);
    if (code != SQLITE_OK) {
      // Comment copied from sqlite documentation. It is recommended that
      // applications respond to the errors listed above by explicitly issuing
      // a ROLLBACK command.If the transaction has already been rolled back
      // automatically by the error response, then the ROLLBACK command will
      // fail with an error, but no harm is caused by this.
      sqlite3_exec(m_metadataDBConnection, "ROLLBACK", 0, 0, 0);
      return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(code);
    }    
  }

  return sts;
}

Status DatabaseMetadataManager::CreateIndex(const char* collectionName,
                                            const IndexInfo& indexInfo) {
  unique_ptr<sqlite3_stmt, Status (*)(sqlite3_stmt*)> statementGuard(
      m_insertCollectionIndexStmt, SQLiteUtils::ClearAndResetStatement);

  int sqliteCode = sqlite3_bind_text(m_insertCollectionIndexStmt, 1,  // Index of wildcard
                                     indexInfo.GetName(), -1,  // length of the string is the number of bytes up to the first zero terminator
                                     SQLITE_STATIC);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  sqliteCode = sqlite3_bind_text(m_insertCollectionIndexStmt, 2,  // Index of wildcard
                                 collectionName, -1,  // length of the string is the number of bytes up to the first zero terminator
                                 SQLITE_STATIC);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  Buffer buffer;
  auto status = SerializerUtils::IndexInfoToBytes(indexInfo, buffer);
  if (!status.OK()) {
    return status;
  }

  sqliteCode = sqlite3_bind_blob(m_insertCollectionIndexStmt, 3,  // Index of wildcard
                                 buffer.GetData(), buffer.GetLength(),
                                 SQLITE_STATIC);
  if (sqliteCode != SQLITE_OK)
    return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);

  //Now insert the record
  sqliteCode = sqlite3_step(m_insertCollectionIndexStmt);
  if (sqliteCode != SQLITE_DONE) {
    if (sqliteCode == SQLITE_CONSTRAINT) {
      //Key already exists
      std::string errorMsg = "Index with the same name already exists.";
      return Status(kStatusIndexAlreadyExistCode, errorMsg.c_str(),
                    errorMsg.length());
    } else {
      return ExceptionUtils::GetSQLiteErrorStatusFromSQLiteErrorCode(sqliteCode);
    }
  }

  return Status();
}

const char* DatabaseMetadataManager::GetFullDBPath() const {
  return m_fullDbPath.c_str();
}
