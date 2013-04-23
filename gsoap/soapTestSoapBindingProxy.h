/* soapTestSoapBindingProxy.h
   Generated by gSOAP 2.8.14 from webservice.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapTestSoapBindingProxy_H
#define soapTestSoapBindingProxy_H
#include "soapH.h"

class SOAP_CMAC TestSoapBindingProxy
{ public:
	struct soap *soap;
	bool own;
	/// Endpoint URL of service 'TestSoapBindingProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	TestSoapBindingProxy();
	/// Constructor to use/share an engine state
	TestSoapBindingProxy(struct soap*);
	/// Constructor with endpoint URL
	TestSoapBindingProxy(const char *url);
	/// Constructor with engine input+output mode control
	TestSoapBindingProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	TestSoapBindingProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	TestSoapBindingProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~TestSoapBindingProxy();
	/// Initializer used by constructors
	virtual	void TestSoapBindingProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (uses soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service one-way send operation 'send_HelloResponse' (returns error code or SOAP_OK)
	virtual	int send_HelloResponse(_ns1__HelloResponse *ns1__HelloResponse) { return this->send_HelloResponse(NULL, NULL, ns1__HelloResponse); }
	virtual	int send_HelloResponse(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse);
	/// Web service one-way receive operation 'recv_HelloResponse' (returns error code or SOAP_OK);
	virtual	int recv_HelloResponse(struct __ns1__HelloResponse&);
	/// Web service receive of HTTP Accept acknowledgment for one-way send operation 'send_HelloResponse' (returns error code or SOAP_OK)
	virtual	int recv_HelloResponse_empty_response() { return soap_recv_empty_response(this->soap); }
	/// Web service one-way synchronous send operation 'HelloResponse' with HTTP Accept/OK response receive (returns error code or SOAP_OK)
	virtual	int HelloResponse(_ns1__HelloResponse *ns1__HelloResponse) { return this->HelloResponse(NULL, NULL, ns1__HelloResponse); }
	virtual	int HelloResponse(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse) { if (this->send_HelloResponse(endpoint, soap_action, ns1__HelloResponse) || soap_recv_empty_response(this->soap)) return this->soap->error; return SOAP_OK; }

	/// Web service operation 'Hello' (returns error code or SOAP_OK)
	virtual	int Hello(_ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse) { return this->Hello(NULL, NULL, ns1__Hello, ns1__HelloResponse); }
	virtual	int Hello(const char *endpoint, const char *soap_action, _ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse);

	/// Web service one-way send operation 'send_HelloResponse' (returns error code or SOAP_OK)
	virtual	int send_HelloResponse_(_ns1__HelloResponse *ns1__HelloResponse) { return this->send_HelloResponse_(NULL, NULL, ns1__HelloResponse); }
	virtual	int send_HelloResponse_(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse);
	/// Web service one-way receive operation 'recv_HelloResponse' (returns error code or SOAP_OK);
	virtual	int recv_HelloResponse_(struct __ns1__HelloResponse_&);
	/// Web service receive of HTTP Accept acknowledgment for one-way send operation 'send_HelloResponse' (returns error code or SOAP_OK)
	virtual	int recv_HelloResponse__empty_response() { return soap_recv_empty_response(this->soap); }
	/// Web service one-way synchronous send operation 'HelloResponse' with HTTP Accept/OK response receive (returns error code or SOAP_OK)
	virtual	int HelloResponse_(_ns1__HelloResponse *ns1__HelloResponse) { return this->HelloResponse_(NULL, NULL, ns1__HelloResponse); }
	virtual	int HelloResponse_(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse) { if (this->send_HelloResponse_(endpoint, soap_action, ns1__HelloResponse) || soap_recv_empty_response(this->soap)) return this->soap->error; return SOAP_OK; }

	/// Web service operation 'Hello' (returns error code or SOAP_OK)
	virtual	int Hello_(_ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse) { return this->Hello_(NULL, NULL, ns1__Hello, ns1__HelloResponse); }
	virtual	int Hello_(const char *endpoint, const char *soap_action, _ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse);
};
#endif
