// Reminder: Modify typemap.dat to customize the header file generated by wsdl2h
/* webservice.h
   Generated by wsdl2h 2.8.14 from webservice.wsdl and typemap.dat
   2013-05-13 10:21:07 GMT

   DO NOT INCLUDE THIS FILE DIRECTLY INTO YOUR PROJECT BUILDS
   USE THE soapcpp2-GENERATED SOURCE CODE FILES FOR YOUR PROJECT BUILDS

   gSOAP XML Web services tools.
   Copyright (C) 2000-2013 Robert van Engelen, Genivia Inc. All Rights Reserved.
   Part of this software is released under one of the following licenses:
   GPL or Genivia's license for commercial use.
*/

/** @page page_notes Usage Notes

NOTE:

 - Run soapcpp2 on webservice.h to generate the SOAP/XML processing logic.
   Use soapcpp2 -I to specify paths for #import
   To build with STL, 'stlvector.h' is imported from 'import' dir in package.
   Use soapcpp2 -j to generate improved proxy and server classes.
 - Use wsdl2h -c and -s to generate pure C code or C++ code without STL.
 - Use 'typemap.dat' to control namespace bindings and type mappings.
   It is strongly recommended to customize the names of the namespace prefixes
   generated by wsdl2h. To do so, modify the prefix bindings in the Namespaces
   section below and add the modified lines to 'typemap.dat' to rerun wsdl2h.
 - Use Doxygen (www.doxygen.org) on this file to generate documentation.
 - Use wsdl2h -nname to use name as the base namespace prefix instead of 'ns'.
 - Use wsdl2h -Nname for service prefix and produce multiple service bindings
 - Use wsdl2h -d to enable DOM support for xsd:anyType.
 - Use wsdl2h -g to auto-generate readers and writers for root elements.
 - Use wsdl2h -b to auto-generate bi-directional operations (duplex ops).
 - Struct/class members serialized as XML attributes are annotated with a '@'.
 - Struct/class members that have a special role are annotated with a '$'.

WARNING:

   DO NOT INCLUDE THIS ANNOTATED FILE DIRECTLY IN YOUR PROJECT SOURCE CODE.
   USE THE FILES GENERATED BY soapcpp2 FOR YOUR PROJECT'S SOURCE CODE:
   THE soapStub.h FILE CONTAINS THIS CONTENT WITHOUT ANNOTATIONS.

LICENSE:

@verbatim
--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.

This software is released under one of the following two licenses:
GPL or Genivia's license for commercial use.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial-use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
@endverbatim

*/


//gsoapopt w

/******************************************************************************\
 *                                                                            *
 * Definitions                                                                *
 *   http://localhost                                                         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/


// STL vector containers (use option -s to remove STL dependency)
#import "stlvector.h"

/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/


/* NOTE:

It is strongly recommended to customize the names of the namespace prefixes
generated by wsdl2h. To do so, modify the prefix bindings below and add the
modified lines to typemap.dat to rerun wsdl2h:

ns1 = "http://localhost"

*/

#define SOAP_NAMESPACE_OF_ns1	"http://localhost"
//gsoap ns1   schema namespace:	http://localhost
//gsoap ns1   schema elementForm:	qualified
//gsoap ns1   schema attributeForm:	unqualified

/******************************************************************************\
 *                                                                            *
 * Built-in Schema Types and Top-Level Elements and Attributes                *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Forward Declarations                                                       *
 *                                                                            *
\******************************************************************************/



//  Forward declaration of class _ns1__exchange.
class _ns1__exchange;

//  Forward declaration of class _ns1__exchangeResponse.
class _ns1__exchangeResponse;

//  Forward declaration of class _ns1__accept.
class _ns1__accept;

//  Forward declaration of class _ns1__acceptResponse.
class _ns1__acceptResponse;

/******************************************************************************\
 *                                                                            *
 * Schema Types and Top-Level Elements and Attributes                         *
 *   http://localhost                                                         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Schema Complex Types and Top-Level Elements                                *
 *   http://localhost                                                         *
 *                                                                            *
\******************************************************************************/



/// Top-level root element "http://localhost":exchange

/// "http://localhost":exchange is a complexType.
/// class _ns1__exchange operations:
/// - soap_new__ns1__exchange(soap*) allocate
/// - soap_new__ns1__exchange(soap*, int num) allocate array
/// - soap_new_req__ns1__exchange(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__exchange(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__exchange(soap*, _ns1__exchange*) deserialize from a stream
/// - int soap_write__ns1__exchange(soap, _ns1__exchange*) serialize to a stream
class _ns1__exchange
{ public:
/// Element param of type xs:string.
    std::string                          param                          1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Top-level root element "http://localhost":exchangeResponse

/// "http://localhost":exchangeResponse is a complexType.
/// class _ns1__exchangeResponse operations:
/// - soap_new__ns1__exchangeResponse(soap*) allocate
/// - soap_new__ns1__exchangeResponse(soap*, int num) allocate array
/// - soap_new_req__ns1__exchangeResponse(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__exchangeResponse(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__exchangeResponse(soap*, _ns1__exchangeResponse*) deserialize from a stream
/// - int soap_write__ns1__exchangeResponse(soap, _ns1__exchangeResponse*) serialize to a stream
class _ns1__exchangeResponse
{ public:
/// Element return of type xs:string.
    std::string                          return_                        1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Top-level root element "http://localhost":accept

/// "http://localhost":accept is a complexType.
/// class _ns1__accept operations:
/// - soap_new__ns1__accept(soap*) allocate
/// - soap_new__ns1__accept(soap*, int num) allocate array
/// - soap_new_req__ns1__accept(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__accept(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__accept(soap*, _ns1__accept*) deserialize from a stream
/// - int soap_write__ns1__accept(soap, _ns1__accept*) serialize to a stream
class _ns1__accept
{ public:
/// Element flag of type xs:boolean.
    bool                                 flag                           1;	///< Required element.
/// Element platformId of type xs:string.
    std::string                          platformId                     1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};


/// Top-level root element "http://localhost":acceptResponse

/// "http://localhost":acceptResponse is a complexType.
/// class _ns1__acceptResponse operations:
/// - soap_new__ns1__acceptResponse(soap*) allocate
/// - soap_new__ns1__acceptResponse(soap*, int num) allocate array
/// - soap_new_req__ns1__acceptResponse(soap*, ...) allocate, set required members
/// - soap_new_set__ns1__acceptResponse(soap*, ...) allocate, set all public members
/// - int soap_read__ns1__acceptResponse(soap*, _ns1__acceptResponse*) deserialize from a stream
/// - int soap_write__ns1__acceptResponse(soap, _ns1__acceptResponse*) serialize to a stream
class _ns1__acceptResponse
{ public:
/// Element return of type xs:string.
    std::string                          return_                        1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Elements                                              *
 *   http://localhost                                                         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Additional Top-Level Attributes                                            *
 *   http://localhost                                                         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/

// This service supports SOAP 1.2 namespaces:
#import "soap12.h"

//gsoap ns1  service name:	bossnSoapBinding 
//gsoap ns1  service type:	bossnPortType 
//gsoap ns1  service port:	http://localhost/bossn/ws/bossn.1cws 
//gsoap ns1  service namespace:	http://localhost 
//gsoap ns1  service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage bossn Definitions

@section bossn_bindings Service Bindings

  - @ref bossnSoapBinding

@section bossn_more More Information

  - @ref page_notes "Usage Notes"

  - @ref page_XMLDataBinding "XML Data Binding"

  - @ref SOAP_ENV__Header "SOAP Header Content" (when applicable)

  - @ref SOAP_ENV__Detail "SOAP Fault Detail Content" (when applicable)


*/

/**

@page bossnSoapBinding Binding "bossnSoapBinding"

@section bossnSoap_port Port Documentation "bossnSoap"
 
				<wsi:Claim xmlns:wsi="http://ws-i.org/schemas/conformanceClaim/"
						conformsTo="http://ws-i.org/profiles/basic/1.1"/>
			

@section bossnSoapBinding_operations Operations of Binding  "bossnSoapBinding"

  - @ref __ns1__exchange

  - @ref __ns1__accept

  - @ref __ns1__exchange_

  - @ref __ns1__accept_

@section bossnSoapBinding_ports Endpoints of Binding  "bossnSoapBinding"

  - http://localhost/bossn/ws/bossn.1cws

Note: use wsdl2h option -Nname to change the service binding prefix name


*/

/******************************************************************************\
 *                                                                            *
 * Service Binding                                                            *
 *   bossnSoapBinding                                                         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__exchange                                                          *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__exchange" of service binding "bossnSoapBinding"

/**

Operation details:

  - SOAP document/literal style messaging

  - Action: "http://localhost#bossn:exchange"

  - Addressing input action: "http://localhost#bossn:exchange"

  - Addressing output action: "http://localhost#bossn:exchangeResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__exchange(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__exchange*                     ns1__exchange,
    // output parameters:
    _ns1__exchangeResponse*             ns1__exchangeResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__exchange(
    struct soap *soap,
    // input parameters:
    _ns1__exchange*                     ns1__exchange,
    // output parameters:
    _ns1__exchangeResponse*             ns1__exchangeResponse
  );
@endcode

C++ proxy class (defined in soapbossnSoapBindingProxy.h):
@code
  class bossnSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use proxy classes;

C++ service class (defined in soapbossnSoapBindingService.h):
@code
  class bossnSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	exchangeResponse SOAP
//gsoap ns1  service method-style:	exchangeResponse document
//gsoap ns1  service method-encoding:	exchangeResponse literal
//gsoap ns1  service method-action:	exchangeResponse http://localhost#bossn:exchangeResponse
/// Operation response "__ns1__exchangeResponse" of operation "__ns1__exchange"
int __ns1__exchangeResponse(
    _ns1__exchangeResponse*             ns1__exchangeResponse,	///< Input parameter
    void	///< One-way message: no output parameter
);
//gsoap ns1  service method-protocol:	exchange SOAP
//gsoap ns1  service method-style:	exchange document
//gsoap ns1  service method-encoding:	exchange literal
//gsoap ns1  service method-input-action:	exchange http://localhost#bossn:exchange
//gsoap ns1  service method-output-action:	exchange http://localhost#bossn:exchangeResponse
int __ns1__exchange(
    _ns1__exchange*                     ns1__exchange,	///< Input parameter
    _ns1__exchangeResponse*             ns1__exchangeResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__accept                                                            *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__accept" of service binding "bossnSoapBinding"

/**

Operation details:

  - SOAP document/literal style messaging

  - Action: "http://localhost#bossn:accept"

  - Addressing input action: "http://localhost#bossn:accept"

  - Addressing output action: "http://localhost#bossn:acceptResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__accept(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__accept*                       ns1__accept,
    // output parameters:
    _ns1__acceptResponse*               ns1__acceptResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__accept(
    struct soap *soap,
    // input parameters:
    _ns1__accept*                       ns1__accept,
    // output parameters:
    _ns1__acceptResponse*               ns1__acceptResponse
  );
@endcode

C++ proxy class (defined in soapbossnSoapBindingProxy.h):
@code
  class bossnSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use proxy classes;

C++ service class (defined in soapbossnSoapBindingService.h):
@code
  class bossnSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	acceptResponse SOAP
//gsoap ns1  service method-style:	acceptResponse document
//gsoap ns1  service method-encoding:	acceptResponse literal
//gsoap ns1  service method-action:	acceptResponse http://localhost#bossn:acceptResponse
/// Operation response "__ns1__acceptResponse" of operation "__ns1__accept"
int __ns1__acceptResponse(
    _ns1__acceptResponse*               ns1__acceptResponse,	///< Input parameter
    void	///< One-way message: no output parameter
);
//gsoap ns1  service method-protocol:	accept SOAP
//gsoap ns1  service method-style:	accept document
//gsoap ns1  service method-encoding:	accept literal
//gsoap ns1  service method-input-action:	accept http://localhost#bossn:accept
//gsoap ns1  service method-output-action:	accept http://localhost#bossn:acceptResponse
int __ns1__accept(
    _ns1__accept*                       ns1__accept,	///< Input parameter
    _ns1__acceptResponse*               ns1__acceptResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__exchange_                                                         *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__exchange_" of service binding "bossnSoapBinding"

/**

Operation details:

  - SOAP document/literal style messaging

  - Action: "http://localhost#bossn:exchange"

  - Addressing input action: "http://localhost#bossn:exchange"

  - Addressing output action: "http://localhost#bossn:exchangeResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__exchange_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__exchange*                     ns1__exchange,
    // output parameters:
    _ns1__exchangeResponse*             ns1__exchangeResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__exchange_(
    struct soap *soap,
    // input parameters:
    _ns1__exchange*                     ns1__exchange,
    // output parameters:
    _ns1__exchangeResponse*             ns1__exchangeResponse
  );
@endcode

C++ proxy class (defined in soapbossnSoapBindingProxy.h):
@code
  class bossnSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use proxy classes;

C++ service class (defined in soapbossnSoapBindingService.h):
@code
  class bossnSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	exchangeResponse_ SOAP
//gsoap ns1  service method-style:	exchangeResponse_ document
//gsoap ns1  service method-encoding:	exchangeResponse_ literal
//gsoap ns1  service method-action:	exchangeResponse_ http://localhost#bossn:exchangeResponse
/// Operation response "__ns1__exchangeResponse_" of operation "__ns1__exchange_"
int __ns1__exchangeResponse_(
    _ns1__exchangeResponse*             ns1__exchangeResponse,	///< Input parameter
    void	///< One-way message: no output parameter
);
//gsoap ns1  service method-protocol:	exchange_ SOAP
//gsoap ns1  service method-style:	exchange_ document
//gsoap ns1  service method-encoding:	exchange_ literal
//gsoap ns1  service method-input-action:	exchange_ http://localhost#bossn:exchange
//gsoap ns1  service method-output-action:	exchange_ http://localhost#bossn:exchangeResponse
int __ns1__exchange_(
    _ns1__exchange*                     ns1__exchange,	///< Input parameter
    _ns1__exchangeResponse*             ns1__exchangeResponse	///< Output parameter
);

/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__accept_                                                           *
 *                                                                            *
\******************************************************************************/


/// Operation "__ns1__accept_" of service binding "bossnSoapBinding"

/**

Operation details:

  - SOAP document/literal style messaging

  - Action: "http://localhost#bossn:accept"

  - Addressing input action: "http://localhost#bossn:accept"

  - Addressing output action: "http://localhost#bossn:acceptResponse"

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__accept_(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    _ns1__accept*                       ns1__accept,
    // output parameters:
    _ns1__acceptResponse*               ns1__acceptResponse
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__accept_(
    struct soap *soap,
    // input parameters:
    _ns1__accept*                       ns1__accept,
    // output parameters:
    _ns1__acceptResponse*               ns1__acceptResponse
  );
@endcode

C++ proxy class (defined in soapbossnSoapBindingProxy.h):
@code
  class bossnSoapBindingProxy;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use proxy classes;

C++ service class (defined in soapbossnSoapBindingService.h):
@code
  class bossnSoapBindingService;
@endcode
Important: use soapcpp2 option '-j' (or '-i') to generate greatly improved and easy-to-use service classes;

*/

//gsoap ns1  service method-protocol:	acceptResponse_ SOAP
//gsoap ns1  service method-style:	acceptResponse_ document
//gsoap ns1  service method-encoding:	acceptResponse_ literal
//gsoap ns1  service method-action:	acceptResponse_ http://localhost#bossn:acceptResponse
/// Operation response "__ns1__acceptResponse_" of operation "__ns1__accept_"
int __ns1__acceptResponse_(
    _ns1__acceptResponse*               ns1__acceptResponse,	///< Input parameter
    void	///< One-way message: no output parameter
);
//gsoap ns1  service method-protocol:	accept_ SOAP
//gsoap ns1  service method-style:	accept_ document
//gsoap ns1  service method-encoding:	accept_ literal
//gsoap ns1  service method-input-action:	accept_ http://localhost#bossn:accept
//gsoap ns1  service method-output-action:	accept_ http://localhost#bossn:acceptResponse
int __ns1__accept_(
    _ns1__accept*                       ns1__accept,	///< Input parameter
    _ns1__acceptResponse*               ns1__acceptResponse	///< Output parameter
);

/**

@page bossnSoapBinding Binding "bossnSoapBinding"

@section bossnSoapBinding_policy_enablers Policy Enablers of Binding  "bossnSoapBinding"

Based on policies, this service imports

  - WS-Policy reminders and enablers:
    - WS-Addressing 1.0 (2005/08, accepts 2004/08):
	@code
	#import "wsa5.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsaapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	// See the user guide gsoap/doc/wsa/html/index.html
	@endcode
    - WS-Addressing (2004/08):
	@code
	#import "wsa.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsaapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	// See the user guide gsoap/doc/wsa/html/index.html
	@endcode
    - WS-ReliableMessaging 1.0:
	@code
	#import "wsrm5.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsrmapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	soap_register_plugin(soap, soap_wsrm); // register the wsrm plugin in your code
	// See the user guide gsoap/doc/wsrm/html/index.html
	@endcode
    - WS-ReliableMessaging 1.1:
	@code
	#import "wsrm.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsrmapi.h"
	soap_register_plugin(soap, soap_wsa); // register the wsa plugin in your code
	soap_register_plugin(soap, soap_wsrm); // register the wsrm plugin in your code
	// See the user guide gsoap/doc/wsrm/html/index.html
	@endcode
    - WS-Security (SOAP Message Security) 1.0 (accepts 1.1):
	@code
	#import "wsse.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsseapi.h"
	soap_register_plugin(soap, soap_wsse); // register the wsse plugin in your code
	// See the user guide gsoap/doc/wsse/html/index.html
	@endcode
    - WS-Security (SOAP Message Security) 1.1 (accepts 1.0):
	@code
	#import "wsse11.h" // to be added to this header file for the soapcpp2 build step
	@endcode
	@code
	#include "plugin/wsseapi.h"
	soap_register_plugin(soap, soap_wsse); // register the wsse plugin in your code
	// See the user guide gsoap/doc/wsse/html/index.html
	@endcode
    - HTTP Digest Authentication:
	@code
	#include "plugin/httpda.h"
	soap_register_plugin(soap, soap_http_da); // register the HTTP DA plugin in your code
	// See the user guide gsoap/doc/httpda/html/index.html
	@endcode
*/


/******************************************************************************\
 *                                                                            *
 * XML Data Binding                                                           *
 *                                                                            *
\******************************************************************************/


/**

@page page_XMLDataBinding XML Data Binding

SOAP/XML services use data bindings contractually bound by WSDL and auto-
generated by wsdl2h and soapcpp2 (see Service Bindings). Plain data bindings
are adopted from XML schemas as part of the WSDL types section or when running
wsdl2h on a set of schemas to produce non-SOAP-based XML data bindings.

The following readers and writers are C/C++ data type (de)serializers auto-
generated by wsdl2h and soapcpp2. Run soapcpp2 on this file to generate the
(de)serialization code, which is stored in soapC.c[pp]. Include "soapH.h" in
your code to import these data type and function declarations. Only use the
soapcpp2-generated files in your project build. Do not include the wsdl2h-
generated .h file in your code.

Data can be read in XML and deserialized from:
  - a file descriptor, using soap->recvfd = fd
  - a socket, using soap->socket = ...
  - a C++ stream, using soap->is = ...
  - a buffer, using the soap->frecv() callback

Data can be serialized in XML and written to:
  - a file descriptor, using soap->sendfd = fd
  - a socket, using soap->socket = ...
  - a C++ stream, using soap->os = ...
  - a buffer, using the soap->fsend() callback

The following options are available for (de)serialization control:
  - soap->encodingStyle = NULL; to remove SOAP 1.1/1.2 encodingStyle
  - soap_mode(soap, SOAP_XML_TREE); XML without id-ref (no cycles!)
  - soap_mode(soap, SOAP_XML_GRAPH); XML with id-ref (including cycles)
  - soap_set_namespaces(soap, struct Namespace *nsmap); to set xmlns bindings


@section ns1 Top-level root elements of schema "http://localhost"

  - <ns1:exchange> @ref _ns1__exchange
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__exchange(struct soap*, _ns1__exchange*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__exchange(struct soap*, _ns1__exchange*);
    @endcode

  - <ns1:exchangeResponse> @ref _ns1__exchangeResponse
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__exchangeResponse(struct soap*, _ns1__exchangeResponse*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__exchangeResponse(struct soap*, _ns1__exchangeResponse*);
    @endcode

  - <ns1:accept> @ref _ns1__accept
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__accept(struct soap*, _ns1__accept*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__accept(struct soap*, _ns1__accept*);
    @endcode

  - <ns1:acceptResponse> @ref _ns1__acceptResponse
    @code
    // Reader (returns SOAP_OK on success):
    soap_read__ns1__acceptResponse(struct soap*, _ns1__acceptResponse*);
    // Writer (returns SOAP_OK on success):
    soap_write__ns1__acceptResponse(struct soap*, _ns1__acceptResponse*);
    @endcode

*/

/* End of webservice.h */
