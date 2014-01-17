#ifndef COACONTROLLER_H
#define COACONTROLLER_H

#include <map>
#include <string>

#include "../src/CoaServer.h"
#include "../dependencies/interpp.h"

//=================================================================================================

class CoaController
{
public:
  CoaController();
  ~CoaController();

  uint FindAlbums( std::string artist );
  std::string GetAlbum( uint index );

private:
  std::map< const char*, std::vector< std::string > > _albums;
};

// Register methods with Interpp
// =============================
INTERPP_REGISTER_METHOD_RETURN( CoaController, FindAlbums, uint, std::string )
INTERPP_REGISTER_METHOD_RETURN( CoaController, GetAlbum, std::string, uint )

//=================================================================================================

#endif // COACONTROLLER_H
