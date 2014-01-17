#include "CoaController.h"
#include "../src/CoaServer.h"

#include <iostream>
#include <stdio.h>

//=================================================================================================

int main()
{
  CoaController controller;

  CoaServer::StartServer( "html", "8080" );

  std::cout << "Browse to http://localhost:8080\n";

  std::string command;
  while( command != "exit" )
  {
    std::cin >> command;
  }

  CoaServer::StopServer();

  return 0;
}

//=================================================================================================
