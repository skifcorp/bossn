/* soapStub.h
   Generated by gSOAP 2.8.14 from webservice.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#include <vector>
#define SOAP_NAMESPACE_OF_ns1	"http://localhost"
#include "stdsoap2.h"
#if GSOAP_VERSION != 20814
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE__ns1__exchange
#define SOAP_TYPE__ns1__exchange (8)
/* ns1:exchange */
class SOAP_CMAC _ns1__exchange
{
public:
	std::string param;	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique id SOAP_TYPE__ns1__exchange */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__exchange() { _ns1__exchange::soap_default(NULL); }
	virtual ~_ns1__exchange() { }
};
#endif

#ifndef SOAP_TYPE__ns1__exchangeResponse
#define SOAP_TYPE__ns1__exchangeResponse (9)
/* ns1:exchangeResponse */
class SOAP_CMAC _ns1__exchangeResponse
{
public:
	std::string return_;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique id SOAP_TYPE__ns1__exchangeResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__exchangeResponse() { _ns1__exchangeResponse::soap_default(NULL); }
	virtual ~_ns1__exchangeResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__accept
#define SOAP_TYPE__ns1__accept (10)
/* ns1:accept */
class SOAP_CMAC _ns1__accept
{
public:
	bool flag;	/* required element of type xsd:boolean */
	std::string platformId;	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique id SOAP_TYPE__ns1__accept */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__accept() { _ns1__accept::soap_default(NULL); }
	virtual ~_ns1__accept() { }
};
#endif

#ifndef SOAP_TYPE__ns1__acceptResponse
#define SOAP_TYPE__ns1__acceptResponse (11)
/* ns1:acceptResponse */
class SOAP_CMAC _ns1__acceptResponse
{
public:
	std::string return_;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique id SOAP_TYPE__ns1__acceptResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__acceptResponse() { _ns1__acceptResponse::soap_default(NULL); }
	virtual ~_ns1__acceptResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns1__exchangeResponse
#define SOAP_TYPE___ns1__exchangeResponse (18)
/* Operation wrapper: */
struct __ns1__exchangeResponse
{
public:
	_ns1__exchangeResponse *ns1__exchangeResponse;	/* optional element of type ns1:exchangeResponse */
public:
	int soap_type() const { return 18; } /* = unique id SOAP_TYPE___ns1__exchangeResponse */
};
#endif

#ifndef SOAP_TYPE___ns1__exchange
#define SOAP_TYPE___ns1__exchange (21)
/* Operation wrapper: */
struct __ns1__exchange
{
public:
	_ns1__exchange *ns1__exchange;	/* optional element of type ns1:exchange */
public:
	int soap_type() const { return 21; } /* = unique id SOAP_TYPE___ns1__exchange */
};
#endif

#ifndef SOAP_TYPE___ns1__acceptResponse
#define SOAP_TYPE___ns1__acceptResponse (24)
/* Operation wrapper: */
struct __ns1__acceptResponse
{
public:
	_ns1__acceptResponse *ns1__acceptResponse;	/* optional element of type ns1:acceptResponse */
public:
	int soap_type() const { return 24; } /* = unique id SOAP_TYPE___ns1__acceptResponse */
};
#endif

#ifndef SOAP_TYPE___ns1__accept
#define SOAP_TYPE___ns1__accept (27)
/* Operation wrapper: */
struct __ns1__accept
{
public:
	_ns1__accept *ns1__accept;	/* optional element of type ns1:accept */
public:
	int soap_type() const { return 27; } /* = unique id SOAP_TYPE___ns1__accept */
};
#endif

#ifndef SOAP_TYPE___ns1__exchangeResponse_
#define SOAP_TYPE___ns1__exchangeResponse_ (29)
/* Operation wrapper: */
struct __ns1__exchangeResponse_
{
public:
	_ns1__exchangeResponse *ns1__exchangeResponse;	/* optional element of type ns1:exchangeResponse */
public:
	int soap_type() const { return 29; } /* = unique id SOAP_TYPE___ns1__exchangeResponse_ */
};
#endif

#ifndef SOAP_TYPE___ns1__exchange_
#define SOAP_TYPE___ns1__exchange_ (31)
/* Operation wrapper: */
struct __ns1__exchange_
{
public:
	_ns1__exchange *ns1__exchange;	/* optional element of type ns1:exchange */
public:
	int soap_type() const { return 31; } /* = unique id SOAP_TYPE___ns1__exchange_ */
};
#endif

#ifndef SOAP_TYPE___ns1__acceptResponse_
#define SOAP_TYPE___ns1__acceptResponse_ (33)
/* Operation wrapper: */
struct __ns1__acceptResponse_
{
public:
	_ns1__acceptResponse *ns1__acceptResponse;	/* optional element of type ns1:acceptResponse */
public:
	int soap_type() const { return 33; } /* = unique id SOAP_TYPE___ns1__acceptResponse_ */
};
#endif

#ifndef SOAP_TYPE___ns1__accept_
#define SOAP_TYPE___ns1__accept_ (35)
/* Operation wrapper: */
struct __ns1__accept_
{
public:
	_ns1__accept *ns1__accept;	/* optional element of type ns1:accept */
public:
	int soap_type() const { return 35; } /* = unique id SOAP_TYPE___ns1__accept_ */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (36)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	int soap_type() const { return 36; } /* = unique id SOAP_TYPE_SOAP_ENV__Header */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (37)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
public:
	int soap_type() const { return 37; } /* = unique id SOAP_TYPE_SOAP_ENV__Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (39)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
public:
	int soap_type() const { return 39; } /* = unique id SOAP_TYPE_SOAP_ENV__Detail */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (41)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
public:
	int soap_type() const { return 41; } /* = unique id SOAP_TYPE_SOAP_ENV__Reason */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (42)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
public:
	int soap_type() const { return 42; } /* = unique id SOAP_TYPE_SOAP_ENV__Fault */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */
