//
// Generated file, do not edit! Created by nedtool 4.6 from broker_init.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "broker_init_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(Broker_init_msg);

Broker_init_msg::Broker_init_msg(const char *name, int kind) : ::cMessage(name,kind)
{
    this->srcId_var = 0;
}

Broker_init_msg::Broker_init_msg(const Broker_init_msg& other) : ::cMessage(other)
{
    copy(other);
}

Broker_init_msg::~Broker_init_msg()
{
}

Broker_init_msg& Broker_init_msg::operator=(const Broker_init_msg& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Broker_init_msg::copy(const Broker_init_msg& other)
{
    this->srcId_var = other.srcId_var;
}

void Broker_init_msg::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->srcId_var);
}

void Broker_init_msg::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->srcId_var);
}

int Broker_init_msg::getSrcId() const
{
    return srcId_var;
}

void Broker_init_msg::setSrcId(int srcId)
{
    this->srcId_var = srcId;
}

class Broker_init_msgDescriptor : public cClassDescriptor
{
  public:
    Broker_init_msgDescriptor();
    virtual ~Broker_init_msgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(Broker_init_msgDescriptor);

Broker_init_msgDescriptor::Broker_init_msgDescriptor() : cClassDescriptor("Broker_init_msg", "cMessage")
{
}

Broker_init_msgDescriptor::~Broker_init_msgDescriptor()
{
}

bool Broker_init_msgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Broker_init_msg *>(obj)!=NULL;
}

const char *Broker_init_msgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Broker_init_msgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Broker_init_msgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *Broker_init_msgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "srcId",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Broker_init_msgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcId")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Broker_init_msgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *Broker_init_msgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int Broker_init_msgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Broker_init_msg *pp = (Broker_init_msg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Broker_init_msgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Broker_init_msg *pp = (Broker_init_msg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrcId());
        default: return "";
    }
}

bool Broker_init_msgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Broker_init_msg *pp = (Broker_init_msg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrcId(string2long(value)); return true;
        default: return false;
    }
}

const char *Broker_init_msgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *Broker_init_msgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Broker_init_msg *pp = (Broker_init_msg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


