//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Message_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(Message)

Message::Message(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

Message::Message(const Message& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Message::~Message()
{
}

Message& Message::operator=(const Message& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Message::copy(const Message& other)
{
    this->seqNum = other.seqNum;
    this->payload = other.payload;
    this->trailer = other.trailer;
    this->frameType = other.frameType;
    this->ackNum = other.ackNum;
    this->errorType = other.errorType;
    this->errorString = other.errorString;
    this->messageState = other.messageState;
    this->messageType = other.messageType;
}

void Message::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->payload);
    doParsimPacking(b,this->trailer);
    doParsimPacking(b,this->frameType);
    doParsimPacking(b,this->ackNum);
    doParsimPacking(b,this->errorType);
    doParsimPacking(b,this->errorString);
    doParsimPacking(b,this->messageState);
    doParsimPacking(b,this->messageType);
}

void Message::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->payload);
    doParsimUnpacking(b,this->trailer);
    doParsimUnpacking(b,this->frameType);
    doParsimUnpacking(b,this->ackNum);
    doParsimUnpacking(b,this->errorType);
    doParsimUnpacking(b,this->errorString);
    doParsimUnpacking(b,this->messageState);
    doParsimUnpacking(b,this->messageType);
}

int Message::getSeqNum() const
{
    return this->seqNum;
}

void Message::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

const char * Message::getPayload() const
{
    return this->payload.c_str();
}

void Message::setPayload(const char * payload)
{
    this->payload = payload;
}

char Message::getTrailer() const
{
    return this->trailer;
}

void Message::setTrailer(char trailer)
{
    this->trailer = trailer;
}

int Message::getFrameType() const
{
    return this->frameType;
}

void Message::setFrameType(int frameType)
{
    this->frameType = frameType;
}

int Message::getAckNum() const
{
    return this->ackNum;
}

void Message::setAckNum(int ackNum)
{
    this->ackNum = ackNum;
}

int Message::getErrorType() const
{
    return this->errorType;
}

void Message::setErrorType(int errorType)
{
    this->errorType = errorType;
}

const char * Message::getErrorString() const
{
    return this->errorString.c_str();
}

void Message::setErrorString(const char * errorString)
{
    this->errorString = errorString;
}

int Message::getMessageState() const
{
    return this->messageState;
}

void Message::setMessageState(int messageState)
{
    this->messageState = messageState;
}

int Message::getMessageType() const
{
    return this->messageType;
}

void Message::setMessageType(int messageType)
{
    this->messageType = messageType;
}

class MessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_seqNum,
        FIELD_payload,
        FIELD_trailer,
        FIELD_frameType,
        FIELD_ackNum,
        FIELD_errorType,
        FIELD_errorString,
        FIELD_messageState,
        FIELD_messageType,
    };
  public:
    MessageDescriptor();
    virtual ~MessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(MessageDescriptor)

MessageDescriptor::MessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Message)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

MessageDescriptor::~MessageDescriptor()
{
    delete[] propertyNames;
}

bool MessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Message *>(obj)!=nullptr;
}

const char **MessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *MessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int MessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int MessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_payload
        FD_ISEDITABLE,    // FIELD_trailer
        FD_ISEDITABLE,    // FIELD_frameType
        FD_ISEDITABLE,    // FIELD_ackNum
        FD_ISEDITABLE,    // FIELD_errorType
        FD_ISEDITABLE,    // FIELD_errorString
        FD_ISEDITABLE,    // FIELD_messageState
        FD_ISEDITABLE,    // FIELD_messageType
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *MessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "seqNum",
        "payload",
        "trailer",
        "frameType",
        "ackNum",
        "errorType",
        "errorString",
        "messageState",
        "messageType",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int MessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "payload") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "trailer") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "frameType") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "ackNum") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "errorType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "errorString") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "messageState") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *MessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_seqNum
        "string",    // FIELD_payload
        "char",    // FIELD_trailer
        "int",    // FIELD_frameType
        "int",    // FIELD_ackNum
        "int",    // FIELD_errorType
        "string",    // FIELD_errorString
        "int",    // FIELD_messageState
        "int",    // FIELD_messageType
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **MessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void MessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Message'", field);
    }
}

const char *MessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_payload: return oppstring2string(pp->getPayload());
        case FIELD_trailer: return long2string(pp->getTrailer());
        case FIELD_frameType: return long2string(pp->getFrameType());
        case FIELD_ackNum: return long2string(pp->getAckNum());
        case FIELD_errorType: return long2string(pp->getErrorType());
        case FIELD_errorString: return oppstring2string(pp->getErrorString());
        case FIELD_messageState: return long2string(pp->getMessageState());
        case FIELD_messageType: return long2string(pp->getMessageType());
        default: return "";
    }
}

void MessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_payload: pp->setPayload((value)); break;
        case FIELD_trailer: pp->setTrailer(string2long(value)); break;
        case FIELD_frameType: pp->setFrameType(string2long(value)); break;
        case FIELD_ackNum: pp->setAckNum(string2long(value)); break;
        case FIELD_errorType: pp->setErrorType(string2long(value)); break;
        case FIELD_errorString: pp->setErrorString((value)); break;
        case FIELD_messageState: pp->setMessageState(string2long(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Message'", field);
    }
}

omnetpp::cValue MessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_payload: return pp->getPayload();
        case FIELD_trailer: return pp->getTrailer();
        case FIELD_frameType: return pp->getFrameType();
        case FIELD_ackNum: return pp->getAckNum();
        case FIELD_errorType: return pp->getErrorType();
        case FIELD_errorString: return pp->getErrorString();
        case FIELD_messageState: return pp->getMessageState();
        case FIELD_messageType: return pp->getMessageType();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Message' as cValue -- field index out of range?", field);
    }
}

void MessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_payload: pp->setPayload(value.stringValue()); break;
        case FIELD_trailer: pp->setTrailer(omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_frameType: pp->setFrameType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ackNum: pp->setAckNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_errorType: pp->setErrorType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_errorString: pp->setErrorString(value.stringValue()); break;
        case FIELD_messageState: pp->setMessageState(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Message'", field);
    }
}

const char *MessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr MessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void MessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Message *pp = omnetpp::fromAnyPtr<Message>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Message'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

