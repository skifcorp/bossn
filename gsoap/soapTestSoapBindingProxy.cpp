/* soapTestSoapBindingProxy.cpp
   Generated by gSOAP 2.8.14 from webservice.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "soapTestSoapBindingProxy.h"

TestSoapBindingProxy::TestSoapBindingProxy()
{	this->soap = soap_new();
	this->own = true;
	TestSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

TestSoapBindingProxy::TestSoapBindingProxy(struct soap *_soap)
{	this->soap = _soap;
	this->own = false;
	TestSoapBindingProxy_init(_soap->imode, _soap->omode);
}

TestSoapBindingProxy::TestSoapBindingProxy(const char *url)
{	this->soap = soap_new();
	this->own = true;
	TestSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = url;
}

TestSoapBindingProxy::TestSoapBindingProxy(soap_mode iomode)
{	this->soap = soap_new();
	this->own = true;
	TestSoapBindingProxy_init(iomode, iomode);
}

TestSoapBindingProxy::TestSoapBindingProxy(const char *url, soap_mode iomode)
{	this->soap = soap_new();
	this->own = true;
	TestSoapBindingProxy_init(iomode, iomode);
	soap_endpoint = url;
}

TestSoapBindingProxy::TestSoapBindingProxy(soap_mode imode, soap_mode omode)
{	this->soap = soap_new();
	this->own = true;
	TestSoapBindingProxy_init(imode, omode);
}

TestSoapBindingProxy::~TestSoapBindingProxy()
{	if (this->own)
		soap_free(this->soap);
}

void TestSoapBindingProxy::TestSoapBindingProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this->soap, imode);
	soap_omode(this->soap, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns1", "http://localhost", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this->soap, namespaces);
}

void TestSoapBindingProxy::destroy()
{	soap_destroy(this->soap);
	soap_end(this->soap);
}

void TestSoapBindingProxy::reset()
{	destroy();
	soap_done(this->soap);
	soap_init(this->soap);
	TestSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void TestSoapBindingProxy::soap_noheader()
{	this->soap->header = NULL;
}

const SOAP_ENV__Header *TestSoapBindingProxy::soap_header()
{	return this->soap->header;
}

const SOAP_ENV__Fault *TestSoapBindingProxy::soap_fault()
{	return this->soap->fault;
}

const char *TestSoapBindingProxy::soap_fault_string()
{	return *soap_faultstring(this->soap);
}

const char *TestSoapBindingProxy::soap_fault_detail()
{	return *soap_faultdetail(this->soap);
}

int TestSoapBindingProxy::soap_close_socket()
{	return soap_closesock(this->soap);
}

int TestSoapBindingProxy::soap_force_close_socket()
{	return soap_force_closesock(this->soap);
}

void TestSoapBindingProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this->soap, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void TestSoapBindingProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this->soap, os);
}
#endif

char *TestSoapBindingProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this->soap, buf, len);
}
#endif

int TestSoapBindingProxy::send_HelloResponse(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse)
{	struct soap *soap = this->soap;
	struct __ns1__HelloResponse soap_tmp___ns1__HelloResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost/boss/ws/test.1cws";
	if (soap_action == NULL)
		soap_action = "http://localhost#Test:HelloResponse";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__HelloResponse.ns1__HelloResponse = ns1__HelloResponse;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__HelloResponse(soap, &soap_tmp___ns1__HelloResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__HelloResponse(soap, &soap_tmp___ns1__HelloResponse, "-ns1:HelloResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__HelloResponse(soap, &soap_tmp___ns1__HelloResponse, "-ns1:HelloResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int TestSoapBindingProxy::recv_HelloResponse(struct __ns1__HelloResponse& tmp)
{	struct soap *soap = this->soap;

	struct __ns1__HelloResponse *_param_1 = &tmp;
	soap_default___ns1__HelloResponse(soap, _param_1);
	soap_begin(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get___ns1__HelloResponse(soap, _param_1, "-ns1:HelloResponse", NULL);
	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
		soap->error = SOAP_NO_METHOD;
	if (soap->error
	 || soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int TestSoapBindingProxy::Hello(const char *endpoint, const char *soap_action, _ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse)
{	struct soap *soap = this->soap;
	struct __ns1__Hello soap_tmp___ns1__Hello;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost/boss/ws/test.1cws";
	if (soap_action == NULL)
		soap_action = "http://localhost#Test:Hello";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Hello.ns1__Hello = ns1__Hello;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Hello(soap, &soap_tmp___ns1__Hello);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Hello(soap, &soap_tmp___ns1__Hello, "-ns1:Hello", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__Hello(soap, &soap_tmp___ns1__Hello, "-ns1:Hello", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__HelloResponse)
		return soap_closesock(soap);
	ns1__HelloResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__HelloResponse->soap_get(soap, "ns1:HelloResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int TestSoapBindingProxy::send_HelloResponse_(const char *endpoint, const char *soap_action, _ns1__HelloResponse *ns1__HelloResponse)
{	struct soap *soap = this->soap;
	struct __ns1__HelloResponse_ soap_tmp___ns1__HelloResponse_;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost/boss/ws/test.1cws";
	if (soap_action == NULL)
		soap_action = "http://localhost#Test:HelloResponse";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__HelloResponse_.ns1__HelloResponse = ns1__HelloResponse;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__HelloResponse_(soap, &soap_tmp___ns1__HelloResponse_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__HelloResponse_(soap, &soap_tmp___ns1__HelloResponse_, "-ns1:HelloResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__HelloResponse_(soap, &soap_tmp___ns1__HelloResponse_, "-ns1:HelloResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int TestSoapBindingProxy::recv_HelloResponse_(struct __ns1__HelloResponse_& tmp)
{	struct soap *soap = this->soap;

	struct __ns1__HelloResponse_ *_param_2 = &tmp;
	soap_default___ns1__HelloResponse_(soap, _param_2);
	soap_begin(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get___ns1__HelloResponse_(soap, _param_2, "-ns1:HelloResponse", NULL);
	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
		soap->error = SOAP_NO_METHOD;
	if (soap->error
	 || soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int TestSoapBindingProxy::Hello_(const char *endpoint, const char *soap_action, _ns1__Hello *ns1__Hello, _ns1__HelloResponse *ns1__HelloResponse)
{	struct soap *soap = this->soap;
	struct __ns1__Hello_ soap_tmp___ns1__Hello_;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost/boss/ws/test.1cws";
	if (soap_action == NULL)
		soap_action = "http://localhost#Test:Hello";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__Hello_.ns1__Hello = ns1__Hello;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__Hello_(soap, &soap_tmp___ns1__Hello_);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__Hello_(soap, &soap_tmp___ns1__Hello_, "-ns1:Hello", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
    if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__Hello_(soap, &soap_tmp___ns1__Hello_, "-ns1:Hello", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__HelloResponse)
		return soap_closesock(soap);
	ns1__HelloResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__HelloResponse->soap_get(soap, "ns1:HelloResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */
