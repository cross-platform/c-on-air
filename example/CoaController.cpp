#include <CoaController.h>
#include <iostream>

//=================================================================================================

CoaController::CoaController()
{
  Interpp::Init_CoaController_LogIn();
  Interpp::Init_CoaController_GetAccessLevel();

  Interpp::RegisterObject( this, "CoaController" );
}

//-------------------------------------------------------------------------------------------------

CoaController::~CoaController() {}

//=================================================================================================

bool CoaController::LogIn( std::string email, std::string password )
{
  // validate email and password -for demo purposes we will just check for 'admin' / 'admin'  
  if( email == "admin" && password == "admin" )
  {
    // login successful: add client to clients registry
    _clients[ CoaServer::GetCurrentClient() ] = 1;
    return true;
  }

  return false;
}

//-------------------------------------------------------------------------------------------------

unsigned char CoaController::GetAccessLevel()
{
  // get current calling client
  _clientsIt = _clients.find( CoaServer::GetCurrentClient() );

  if( _clientsIt != _clients.end() )
  {
    // client found: return client's access level
    return _clientsIt->second;
  }
  else
  {
    return 0;
  }
}

//=================================================================================================
