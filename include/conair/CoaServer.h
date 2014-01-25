/************************************************************************
C++ On Air - Light-weight C++ web application server library
Copyright (c) 2012-2013 Marcus Tomlinson

This file is part of ConAir.

The BSD 2-Clause License:
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************/

#ifndef COASERVER_H
#define COASERVER_H

#ifdef _WIN32
  #include <windows.h>
#else
  #include <pthread.h>
#endif

#include <map>
#include <string>

struct mg_connection;
struct mg_context;

//=================================================================================================

struct CoaClientInfo
{
  long clientIp;            // Client's IP address
  int clientPort;           // Client's port
  const char* webSocketKey; // Client's web socket key
};

//=================================================================================================

class CoaServer
{
public:
  static void StartServer( std::string htmlRoot, std::string portNo = "80", std::string threadNo = "50" );
  static void StopServer();

  static void SocketSend( mg_connection* conn, std::string& message );
  static int SocketConnect( const mg_connection * conn );
  static void SocketReady( mg_connection* conn );
  static int SocketReceive( mg_connection* conn, int flags, char* data, size_t data_len );

  static const CoaClientInfo* GetCurrentClient();

private:
  static mg_context* _ctx;
  static CoaClientInfo _clientInfo;

  #ifdef _WIN32
    static std::map< HANDLE, CoaClientInfo > _clientRegistry;
    static std::map< HANDLE, CoaClientInfo >::const_iterator _clientRegistryIt;
  #else
    static std::map< pthread_t, CoaClientInfo > _clientRegistry;
    static std::map< pthread_t, CoaClientInfo >::const_iterator _clientRegistryIt;
  #endif
};

//=================================================================================================

#endif // COASERVER_H
