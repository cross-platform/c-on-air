#ifndef COACONTROLLER_H
#define COACONTROLLER_H

#include <map>
#include <string>

#include <CoaServer.h>
#include <interpp.h>

//=================================================================================================

class CoaController
{
public:
  CoaController();
  ~CoaController();

  bool LogIn( std::string email, std::string password );
  unsigned char GetAccessLevel();

private:
  std::map< const CoaClientInfo*, unsigned char > _clients;
  std::map< const CoaClientInfo*, unsigned char >::const_iterator _clientsIt;
};

// Register methods with Interpp
// =============================
INTERPP_REGISTER_METHOD_RETURN( CoaController, LogIn, bool, std::string, std::string )
INTERPP_REGISTER_METHOD_RETURN( CoaController, GetAccessLevel, unsigned char )

//=================================================================================================

#endif // COACONTROLLER_H
