#include "CoaController.h"
#include "../src/CoaServer.h"

#include <iostream>
#include <stdio.h>
#include <thread>

//=================================================================================================

int main()
{
  CoaController controller;

  CoaServer::StartServer( "html", "80" );

  std::cout << "Mediatmos Server Running...\n";

  while( true )
  {
    std::this_thread::sleep_for( std::chrono::hours( 24 ) );
  }

  std::cout << "Mediatmos Server Stopped.\n";

  CoaServer::StopServer();

  return 0;
}

//=================================================================================================
