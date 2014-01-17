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

#include "CoaJson.h"

#include "../dependencies/cJSON.h"

//=================================================================================================

CoaJson::CoaJson()
: _json( NULL )
{
  _json = cJSON_CreateObject();
}

//-------------------------------------------------------------------------------------------------

CoaJson::~CoaJson()
{
  cJSON_Delete( _json );
}

//=================================================================================================

void CoaJson::ClearJson()
{
  cJSON_Delete( _json );
  _json = cJSON_CreateObject();
}

//-------------------------------------------------------------------------------------------------

bool CoaJson::ReadJson( std::string json )
{
  cJSON_Delete( _json );
  _json = cJSON_Parse( json.c_str() );

  if( !_json )
  {
    return false;
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

std::string CoaJson::WriteJson()
{
  if( !_json )
  {
    return "";
  }

  return cJSON_PrintUnformatted( _json );
}

//-------------------------------------------------------------------------------------------------

bool CoaJson::GetValue( std::string memberName, std::string& returnValue )
{
  if( !_json )
  {
    return false;
  }

  cJSON* item = cJSON_GetObjectItem( _json, memberName.c_str() );

  if( !item )
  {
    return false;
  }

  returnValue = item->valuestring;
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CoaJson::GetArray( std::string arrayName, std::vector< std::string >& returnArray )
{
  if( !_json )
  {
    return false;
  }

  cJSON* arrayItem = cJSON_GetObjectItem( _json, arrayName.c_str() );

  if( !arrayItem )
  {
    return false;
  }

  int arraySize = cJSON_GetArraySize( arrayItem );

  for( int i = 0; i < arraySize; i++ )
  {
    returnArray.push_back( cJSON_GetArrayItem( arrayItem, i )->valuestring );
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

bool CoaJson::AddValue( std::string memberName, std::string memberValue )
{
  if( !_json )
  {
    return false;
  }

  cJSON_AddStringToObject( _json, memberName.c_str(), memberValue.c_str() );
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CoaJson::AddArray( std::string arrayName, std::vector< std::string >& valueArray )
{
  if( !_json )
  {
    return false;
  }

  const char** stringArray;
  stringArray = new const char*[ valueArray.size() ];

  for( unsigned int i = 0; i < valueArray.size(); i++ )
  {
    stringArray[i] = valueArray[i].c_str();
  }

  cJSON* arrayItem = cJSON_CreateStringArray( stringArray, valueArray.size() );

  delete stringArray;

  cJSON_AddItemToObject( _json, arrayName.c_str(), arrayItem );

  return true;
}

//=================================================================================================
