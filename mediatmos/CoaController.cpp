#include "CoaController.h"
#include <iostream>
#include "../dependencies/mongoose.h"

#include <curl/curl.h>
#include "rapidxml/rapidxml.hpp"
#include <algorithm>
#include <cstring>

using namespace rapidxml;

//=================================================================================================

class HttpClient
{
public:
  static std::string Get( const std::string& url )
  {
    CURL* handle = curl_easy_init();

    _Buffer buf;

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str() );
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, _WriteBuffer );
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buf);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if( !buf.buffer )
    {
      return Get( url );
    }

    return std::string( buf.buffer );
  }

  static char *EncodeUrl(char *str)
  {
    char *pstr = str, *buf = (char *)malloc(strlen(str) * 3 + 1), *pbuf = buf;
    while (*pstr) {
      if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
        *pbuf++ = *pstr;
      else if (*pstr == ' ')
        *pbuf++ = '+';
      else
        *pbuf++ = '%', *pbuf++ = _ToHex(*pstr >> 4), *pbuf++ = _ToHex(*pstr & 15);
      pstr++;
    }
    *pbuf = '\0';
    return buf;
  }

private:
  struct _Buffer
  {
    ~_Buffer()
    {
      free(buffer);
    }

    char* buffer = NULL;
    size_t size = 0;
  };

  static size_t _WriteBuffer(void *in_data, size_t block_size, size_t block_count, void *out_data)
  {
    _Buffer* buf = (_Buffer*)out_data;
    size_t prev_size = buf->size;

    size_t write_size = block_size * block_count;
    buf->size += write_size;

    buf->buffer = (char*)realloc(buf->buffer, buf->size);

    memcpy(buf->buffer + prev_size, in_data, write_size);
    return write_size;
  }

  static char _ToHex(char code)
  {
    static char hex[] = "0123456789abcdef";
    return hex[code & 15];
  }
};

//-------------------------------------------------------------------------------------------------

static std::map< std::string, std::string> GetAlbums( const std::string& artist )
{
  std::map< std::string, std::string> albums;

  char* encoded = HttpClient::EncodeUrl( const_cast<char*>( artist.c_str() ) );
  std::string artist_encoded( encoded );
  free(encoded);

  std::string result = HttpClient::Get( "http://musicbrainz.org/ws/2/release/?query=artist:%22" + artist_encoded + "%22+AND+country:%22US%22+AND+type:%22album%22+AND+status:%22official%22&type=release&limit=25&method=advanced" );

  xml_document<> doc;    // character type defaults to char

  try
  {
    doc.parse<0>(const_cast<char*>( result.c_str() ));    // 0 means default parse flags
  }
  catch(...){}

  xml_node<>* releases_node = doc.first_node("metadata");
  if( !releases_node || !releases_node->first_node() )
  {
    return albums;
  }
  releases_node = releases_node->first_node("release-list");

  xml_node<>* release_node = releases_node->first_node("release");
  while( release_node )
  {
    xml_node<>* artist_node = release_node->first_node("artist-credit")->first_node("name-credit")->first_node("artist")->first_node("name");

    std::string artist_name = artist_node->value();
    std::transform(artist_name.begin(), artist_name.end(), artist_name.begin(), ::toupper);

    if( artist == artist_name ||
        artist == ( "THE " + artist_name ) ||
        ( "THE " + artist ) == artist_name ||
        ( "THE " + artist ) == ( "THE " + artist_name ) )
    {
      xml_node<>* album_node = release_node->first_node("title");
      std::string album_upper( album_node->value() );
      std::transform(album_upper.begin(), album_upper.end(), album_upper.begin(), ::toupper);
      size_t bracket_pos = album_upper.find(" (");
      if( bracket_pos != std::string::npos )
      {
        album_upper = album_upper.substr(0, bracket_pos);
      }

      xml_node<>* date_node = release_node->first_node("date");

      albums[ album_upper ] = date_node ? date_node->value() : "";
    }

    release_node = release_node->next_sibling("release");
  }

  return albums;
}

//=================================================================================================

CoaController::CoaController()
{
  Interpp::Init_CoaController_FindAlbums();
  Interpp::Init_CoaController_GetAlbum();

  Interpp::RegisterObject( this, "CoaController" );
}

//-------------------------------------------------------------------------------------------------

CoaController::~CoaController() {}

//=================================================================================================

uint CoaController::FindAlbums( std::string artist )
{
  std::string artist_upper = artist;
  std::transform(artist_upper.begin(), artist_upper.end(), artist_upper.begin(), ::toupper);

  std::map< std::string, std::string> album_map = GetAlbums( artist_upper );
  std::vector< std::string > albums;

  for( auto avail_it = begin( album_map ); avail_it != end( album_map ); avail_it++ )
  {
    albums.push_back( "(" + avail_it->second + ") " + avail_it->first );
  }

  std::sort( begin( albums ), end( albums ) );

  _albums[ CoaServer::GetCurrentClient()->webSocketKey ] = albums;
  return albums.size();
}

//-------------------------------------------------------------------------------------------------

std::string CoaController::GetAlbum( uint index )
{
  std::vector< std::string > albums = _albums[ CoaServer::GetCurrentClient()->webSocketKey ];

  if( index < albums.size() )
  {
    return albums[index];
  }
  else
  {
    return "";
  }
}

//=================================================================================================
