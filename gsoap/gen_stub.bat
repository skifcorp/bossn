wsdl2h -f -b -I%GSOAP_PATH%\import\ -I%GSOAP_PATH% -o webservice.h -v webservice.wsdl
soapcpp2 -C -j -L -I%GSOAP_PATH%\import\ webservice.h
