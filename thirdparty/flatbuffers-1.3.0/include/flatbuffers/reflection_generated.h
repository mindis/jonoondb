// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_REFLECTION_REFLECTION_H_
#define FLATBUFFERS_GENERATED_REFLECTION_REFLECTION_H_

#include "flatbuffers/flatbuffers.h"


namespace reflection {

struct Type;
struct EnumVal;
struct Enum;
struct Field;
struct Object;
struct Schema;

enum BaseType {
  None = 0,
  UType = 1,
  Bool = 2,
  Byte = 3,
  UByte = 4,
  Short = 5,
  UShort = 6,
  Int = 7,
  UInt = 8,
  Long = 9,
  ULong = 10,
  Float = 11,
  Double = 12,
  String = 13,
  Vector = 14,
  Obj = 15,
  Union = 16
};

inline const char **EnumNamesBaseType() {
  static const char *names[] = { "None", "UType", "Bool", "Byte", "UByte", "Short", "UShort", "Int", "UInt", "Long", "ULong", "Float", "Double", "String", "Vector", "Obj", "Union", nullptr };
  return names;
}

inline const char *EnumNameBaseType(BaseType e) { return EnumNamesBaseType()[e]; }

struct Type FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  BaseType base_type() const { return static_cast<BaseType>(GetField<int8_t>(4, 0)); }
  BaseType element() const { return static_cast<BaseType>(GetField<int8_t>(6, 0)); }
  int32_t index() const { return GetField<int32_t>(8, -1); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, 4 /* base_type */) &&
           VerifyField<int8_t>(verifier, 6 /* element */) &&
           VerifyField<int32_t>(verifier, 8 /* index */) &&
           verifier.EndTable();
  }
};

struct TypeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_base_type(BaseType base_type) { fbb_.AddElement<int8_t>(4, static_cast<int8_t>(base_type), 0); }
  void add_element(BaseType element) { fbb_.AddElement<int8_t>(6, static_cast<int8_t>(element), 0); }
  void add_index(int32_t index) { fbb_.AddElement<int32_t>(8, index, -1); }
  TypeBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  TypeBuilder &operator=(const TypeBuilder &);
  flatbuffers::Offset<Type> Finish() {
    auto o = flatbuffers::Offset<Type>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<Type> CreateType(flatbuffers::FlatBufferBuilder &_fbb,
   BaseType base_type = None,
   BaseType element = None,
   int32_t index = -1) {
  TypeBuilder builder_(_fbb);
  builder_.add_index(index);
  builder_.add_element(element);
  builder_.add_base_type(base_type);
  return builder_.Finish();
}

struct EnumVal FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(4); }
  int64_t value() const { return GetField<int64_t>(6, 0); }
  bool KeyCompareLessThan(const EnumVal *o) const { return value() < o->value(); }
  int KeyCompareWithValue(int64_t val) const { return value() < val ? -1 : value() > val; }
  const Object *object() const { return GetPointer<const Object *>(8); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 4 /* name */) &&
           verifier.Verify(name()) &&
           VerifyField<int64_t>(verifier, 6 /* value */) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 8 /* object */) &&
           verifier.VerifyTable(object()) &&
           verifier.EndTable();
  }
};

struct EnumValBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(4, name); }
  void add_value(int64_t value) { fbb_.AddElement<int64_t>(6, value, 0); }
  void add_object(flatbuffers::Offset<Object> object) { fbb_.AddOffset(8, object); }
  EnumValBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  EnumValBuilder &operator=(const EnumValBuilder &);
  flatbuffers::Offset<EnumVal> Finish() {
    auto o = flatbuffers::Offset<EnumVal>(fbb_.EndTable(start_, 3));
    fbb_.Required(o, 4);  // name
    return o;
  }
};

inline flatbuffers::Offset<EnumVal> CreateEnumVal(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> name = 0,
   int64_t value = 0,
   flatbuffers::Offset<Object> object = 0) {
  EnumValBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_object(object);
  builder_.add_name(name);
  return builder_.Finish();
}

struct Enum FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(4); }
  bool KeyCompareLessThan(const Enum *o) const { return *name() < *o->name(); }
  int KeyCompareWithValue(const char *val) const { return strcmp(name()->c_str(), val); }
  const flatbuffers::Vector<flatbuffers::Offset<EnumVal>> *values() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<EnumVal>> *>(6); }
  uint8_t is_union() const { return GetField<uint8_t>(8, 0); }
  const Type *underlying_type() const { return GetPointer<const Type *>(10); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 4 /* name */) &&
           verifier.Verify(name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 6 /* values */) &&
           verifier.Verify(values()) &&
           verifier.VerifyVectorOfTables(values()) &&
           VerifyField<uint8_t>(verifier, 8 /* is_union */) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 10 /* underlying_type */) &&
           verifier.VerifyTable(underlying_type()) &&
           verifier.EndTable();
  }
};

struct EnumBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(4, name); }
  void add_values(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EnumVal>>> values) { fbb_.AddOffset(6, values); }
  void add_is_union(uint8_t is_union) { fbb_.AddElement<uint8_t>(8, is_union, 0); }
  void add_underlying_type(flatbuffers::Offset<Type> underlying_type) { fbb_.AddOffset(10, underlying_type); }
  EnumBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  EnumBuilder &operator=(const EnumBuilder &);
  flatbuffers::Offset<Enum> Finish() {
    auto o = flatbuffers::Offset<Enum>(fbb_.EndTable(start_, 4));
    fbb_.Required(o, 4);  // name
    fbb_.Required(o, 6);  // values
    fbb_.Required(o, 10);  // underlying_type
    return o;
  }
};

inline flatbuffers::Offset<Enum> CreateEnum(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> name = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EnumVal>>> values = 0,
   uint8_t is_union = 0,
   flatbuffers::Offset<Type> underlying_type = 0) {
  EnumBuilder builder_(_fbb);
  builder_.add_underlying_type(underlying_type);
  builder_.add_values(values);
  builder_.add_name(name);
  builder_.add_is_union(is_union);
  return builder_.Finish();
}

struct Field FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(4); }
  bool KeyCompareLessThan(const Field *o) const { return *name() < *o->name(); }
  int KeyCompareWithValue(const char *val) const { return strcmp(name()->c_str(), val); }
  const Type *type() const { return GetPointer<const Type *>(6); }
  uint16_t id() const { return GetField<uint16_t>(8, 0); }
  uint16_t offset() const { return GetField<uint16_t>(10, 0); }
  int64_t default_integer() const { return GetField<int64_t>(12, 0); }
  double default_real() const { return GetField<double>(14, 0.0); }
  uint8_t deprecated() const { return GetField<uint8_t>(16, 0); }
  uint8_t required() const { return GetField<uint8_t>(18, 0); }
  uint8_t key() const { return GetField<uint8_t>(20, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 4 /* name */) &&
           verifier.Verify(name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 6 /* type */) &&
           verifier.VerifyTable(type()) &&
           VerifyField<uint16_t>(verifier, 8 /* id */) &&
           VerifyField<uint16_t>(verifier, 10 /* offset */) &&
           VerifyField<int64_t>(verifier, 12 /* default_integer */) &&
           VerifyField<double>(verifier, 14 /* default_real */) &&
           VerifyField<uint8_t>(verifier, 16 /* deprecated */) &&
           VerifyField<uint8_t>(verifier, 18 /* required */) &&
           VerifyField<uint8_t>(verifier, 20 /* key */) &&
           verifier.EndTable();
  }
};

struct FieldBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(4, name); }
  void add_type(flatbuffers::Offset<Type> type) { fbb_.AddOffset(6, type); }
  void add_id(uint16_t id) { fbb_.AddElement<uint16_t>(8, id, 0); }
  void add_offset(uint16_t offset) { fbb_.AddElement<uint16_t>(10, offset, 0); }
  void add_default_integer(int64_t default_integer) { fbb_.AddElement<int64_t>(12, default_integer, 0); }
  void add_default_real(double default_real) { fbb_.AddElement<double>(14, default_real, 0.0); }
  void add_deprecated(uint8_t deprecated) { fbb_.AddElement<uint8_t>(16, deprecated, 0); }
  void add_required(uint8_t required) { fbb_.AddElement<uint8_t>(18, required, 0); }
  void add_key(uint8_t key) { fbb_.AddElement<uint8_t>(20, key, 0); }
  FieldBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  FieldBuilder &operator=(const FieldBuilder &);
  flatbuffers::Offset<Field> Finish() {
    auto o = flatbuffers::Offset<Field>(fbb_.EndTable(start_, 9));
    fbb_.Required(o, 4);  // name
    fbb_.Required(o, 6);  // type
    return o;
  }
};

inline flatbuffers::Offset<Field> CreateField(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> name = 0,
   flatbuffers::Offset<Type> type = 0,
   uint16_t id = 0,
   uint16_t offset = 0,
   int64_t default_integer = 0,
   double default_real = 0.0,
   uint8_t deprecated = 0,
   uint8_t required = 0,
   uint8_t key = 0) {
  FieldBuilder builder_(_fbb);
  builder_.add_default_real(default_real);
  builder_.add_default_integer(default_integer);
  builder_.add_type(type);
  builder_.add_name(name);
  builder_.add_offset(offset);
  builder_.add_id(id);
  builder_.add_key(key);
  builder_.add_required(required);
  builder_.add_deprecated(deprecated);
  return builder_.Finish();
}

struct Object FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(4); }
  bool KeyCompareLessThan(const Object *o) const { return *name() < *o->name(); }
  int KeyCompareWithValue(const char *val) const { return strcmp(name()->c_str(), val); }
  const flatbuffers::Vector<flatbuffers::Offset<Field>> *fields() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Field>> *>(6); }
  uint8_t is_struct() const { return GetField<uint8_t>(8, 0); }
  int32_t minalign() const { return GetField<int32_t>(10, 0); }
  int32_t bytesize() const { return GetField<int32_t>(12, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 4 /* name */) &&
           verifier.Verify(name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 6 /* fields */) &&
           verifier.Verify(fields()) &&
           verifier.VerifyVectorOfTables(fields()) &&
           VerifyField<uint8_t>(verifier, 8 /* is_struct */) &&
           VerifyField<int32_t>(verifier, 10 /* minalign */) &&
           VerifyField<int32_t>(verifier, 12 /* bytesize */) &&
           verifier.EndTable();
  }
};

struct ObjectBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(4, name); }
  void add_fields(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Field>>> fields) { fbb_.AddOffset(6, fields); }
  void add_is_struct(uint8_t is_struct) { fbb_.AddElement<uint8_t>(8, is_struct, 0); }
  void add_minalign(int32_t minalign) { fbb_.AddElement<int32_t>(10, minalign, 0); }
  void add_bytesize(int32_t bytesize) { fbb_.AddElement<int32_t>(12, bytesize, 0); }
  ObjectBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ObjectBuilder &operator=(const ObjectBuilder &);
  flatbuffers::Offset<Object> Finish() {
    auto o = flatbuffers::Offset<Object>(fbb_.EndTable(start_, 5));
    fbb_.Required(o, 4);  // name
    fbb_.Required(o, 6);  // fields
    return o;
  }
};

inline flatbuffers::Offset<Object> CreateObject(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> name = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Field>>> fields = 0,
   uint8_t is_struct = 0,
   int32_t minalign = 0,
   int32_t bytesize = 0) {
  ObjectBuilder builder_(_fbb);
  builder_.add_bytesize(bytesize);
  builder_.add_minalign(minalign);
  builder_.add_fields(fields);
  builder_.add_name(name);
  builder_.add_is_struct(is_struct);
  return builder_.Finish();
}

struct Schema FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::Vector<flatbuffers::Offset<Object>> *objects() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Object>> *>(4); }
  const flatbuffers::Vector<flatbuffers::Offset<Enum>> *enums() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Enum>> *>(6); }
  const flatbuffers::String *file_ident() const { return GetPointer<const flatbuffers::String *>(8); }
  const flatbuffers::String *file_ext() const { return GetPointer<const flatbuffers::String *>(10); }
  const Object *root_table() const { return GetPointer<const Object *>(12); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 4 /* objects */) &&
           verifier.Verify(objects()) &&
           verifier.VerifyVectorOfTables(objects()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, 6 /* enums */) &&
           verifier.Verify(enums()) &&
           verifier.VerifyVectorOfTables(enums()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 8 /* file_ident */) &&
           verifier.Verify(file_ident()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 10 /* file_ext */) &&
           verifier.Verify(file_ext()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 12 /* root_table */) &&
           verifier.VerifyTable(root_table()) &&
           verifier.EndTable();
  }
};

struct SchemaBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_objects(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Object>>> objects) { fbb_.AddOffset(4, objects); }
  void add_enums(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Enum>>> enums) { fbb_.AddOffset(6, enums); }
  void add_file_ident(flatbuffers::Offset<flatbuffers::String> file_ident) { fbb_.AddOffset(8, file_ident); }
  void add_file_ext(flatbuffers::Offset<flatbuffers::String> file_ext) { fbb_.AddOffset(10, file_ext); }
  void add_root_table(flatbuffers::Offset<Object> root_table) { fbb_.AddOffset(12, root_table); }
  SchemaBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  SchemaBuilder &operator=(const SchemaBuilder &);
  flatbuffers::Offset<Schema> Finish() {
    auto o = flatbuffers::Offset<Schema>(fbb_.EndTable(start_, 5));
    fbb_.Required(o, 4);  // objects
    fbb_.Required(o, 6);  // enums
    return o;
  }
};

inline flatbuffers::Offset<Schema> CreateSchema(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Object>>> objects = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Enum>>> enums = 0,
   flatbuffers::Offset<flatbuffers::String> file_ident = 0,
   flatbuffers::Offset<flatbuffers::String> file_ext = 0,
   flatbuffers::Offset<Object> root_table = 0) {
  SchemaBuilder builder_(_fbb);
  builder_.add_root_table(root_table);
  builder_.add_file_ext(file_ext);
  builder_.add_file_ident(file_ident);
  builder_.add_enums(enums);
  builder_.add_objects(objects);
  return builder_.Finish();
}

inline const reflection::Schema *GetSchema(const void *buf) { return flatbuffers::GetRoot<reflection::Schema>(buf); }

inline bool VerifySchemaBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<reflection::Schema>(); }

inline const char *SchemaIdentifier() { return "BFBS"; }

inline bool SchemaBufferHasIdentifier(const void *buf) { return flatbuffers::BufferHasIdentifier(buf, SchemaIdentifier()); }

inline const char *SchemaExtension() { return "bfbs"; }

inline void FinishSchemaBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<reflection::Schema> root) { fbb.Finish(root, SchemaIdentifier()); }

}  // namespace reflection

#endif  // FLATBUFFERS_GENERATED_REFLECTION_REFLECTION_H_
