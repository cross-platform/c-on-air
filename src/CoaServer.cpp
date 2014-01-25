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

#include <CoaServer.h>

#include <iostream>
#include <string.h>

#include <interpp.h>
#include <mongoose.h>

mg_context* CoaServer::_ctx = NULL;
CoaClientInfo CoaServer::_clientInfo;

#ifdef _WIN32
  std::map< HANDLE, CoaClientInfo > CoaServer::_clientRegistry;
  std::map< HANDLE, CoaClientInfo >::const_iterator CoaServer::_clientRegistryIt;
#else
  std::map< pthread_t, CoaClientInfo > CoaServer::_clientRegistry;
  std::map< pthread_t, CoaClientInfo >::const_iterator CoaServer::_clientRegistryIt;
#endif

//=================================================================================================

void CoaServer::StartServer( std::string htmlRoot, std::string portNo, std::string threadNo )
{
  StopServer();

  struct mg_callbacks callbacks;

  const char* options[] =
  {
    "request_timeout_ms", "0",
    "enable_directory_listing", "no",
    "num_threads", threadNo.c_str(),
    "listening_ports", portNo.c_str(),
    "document_root", htmlRoot.c_str(),
    NULL
  };

  memset( &callbacks, 0, sizeof( callbacks ) );
  callbacks.websocket_connect = SocketConnect;
  callbacks.websocket_ready = SocketReady;
  callbacks.websocket_data = SocketReceive;

  _ctx = mg_start( &callbacks, NULL, options );
}

//-------------------------------------------------------------------------------------------------

void CoaServer::StopServer()
{
  if( _ctx != NULL )
  {
    mg_stop( _ctx );
    _ctx = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CoaServer::SocketSend( mg_connection* conn, std::string& message )
{
  if( message.size() > 0xFFFFFFFF )
  {
    return;
  }

  // move data to payload section
  unsigned long messageSize = message.size();
  unsigned char headerLength = 0;

  if( messageSize <= 125 )
  {
    headerLength = 2;
  }
  else if( messageSize >= 126 && messageSize <= 0xFFFF )
  {
    headerLength = 4;
  }
  else
  {
    headerLength = 10;
  }

  message.resize( messageSize + headerLength );
  memcpy( ( char* ) &message[headerLength], &message[0], messageSize );

  // construct header
  message[0] = ( char ) 0x81;

  if( messageSize <= 125 )
  {
    message[1] = messageSize & 0xFF;
  }
  else if( messageSize >= 126 && messageSize <= 0xFFFF )
  {
    message[1] = 126;
    message[2] = ( messageSize >> 8 ) & 0xFF;
    message[3] = messageSize & 0xFF;
  }
  else
  {
    message[1] = 127;
    message[2] = 0/*( messageSize >> 56 ) & 0xFF*/;
    message[3] = 0/*( messageSize >> 48 ) & 0xFF*/;
    message[4] = 0/*( messageSize >> 40 ) & 0xFF*/;
    message[5] = 0/*( messageSize >> 32 ) & 0xFF*/;
    message[6] = ( messageSize >> 24 ) & 0xFF;
    message[7] = ( messageSize >> 16 ) & 0xFF;
    message[8] = ( messageSize >> 8 ) & 0xFF;
    message[9] = messageSize & 0xFF;
  }

  // send packet
  mg_write( conn, ( char* ) &message[0], message.size() );
}

//-------------------------------------------------------------------------------------------------

int CoaServer::SocketConnect( const mg_connection* conn )
{
  CoaClientInfo clientInfo;
  mg_request_info* remoteInfo = mg_get_request_info( ( mg_connection* ) conn );
  clientInfo.clientIp = remoteInfo->remote_ip;
  clientInfo.clientPort = remoteInfo->remote_port;
  clientInfo.webSocketKey = mg_get_header( conn, "Sec-WebSocket-Key" );

  #ifdef _WIN32
    _clientRegistry[ GetCurrentThread() ] = clientInfo;
  #else
    _clientRegistry[ pthread_self() ] = clientInfo;
  #endif

  std::stringstream ss;
  ss << remoteInfo->remote_ip;
  std::string remoteIp = ss.str();

  std::cout << "Client: " << remoteIp << " Connected.\n";
  return 0;
}

//-------------------------------------------------------------------------------------------------

void CoaServer::SocketReady( mg_connection* conn ) {}

//-------------------------------------------------------------------------------------------------

int CoaServer::SocketReceive( mg_connection* conn, int flags, char* data, size_t data_len )
{
  data[data_len] = '\0';

  std::string methodResult = Interpp::Execute( data );
  SocketSend( conn, methodResult );

  return 1;
}

//-------------------------------------------------------------------------------------------------

const CoaClientInfo* CoaServer::GetCurrentClient()
{
  #ifdef _WIN32
    _clientRegistryIt = _clientRegistry.find( GetCurrentThread() );
  #else
    _clientRegistryIt = _clientRegistry.find( pthread_self() );
  #endif

  if( _clientRegistryIt == _clientRegistry.end() )
  {
    return NULL;
  }

  return &_clientRegistryIt->second;
}

//=================================================================================================
