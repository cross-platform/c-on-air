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

var busyWithCommand = true;
var websocketCallback;

//=================================================================================================

window.onload = function( )
{
  websocket = new WebSocket( 'ws://' + window.location.host );

  //-------------------------------------------------------------------------------------------------

  websocket.onopen = function( ev )
  {
    busyWithCommand = false;
  };

  //-------------------------------------------------------------------------------------------------

  websocket.onclose = function( ev )
  {
    busyWithCommand = true;
  };

  //-------------------------------------------------------------------------------------------------

  websocket.onerror = function( ev )
  {
    //alert( ev.data );
  };

  //-------------------------------------------------------------------------------------------------

  websocket.onmessage = function( ev )
  {
    if( busyWithCommand )
    {
      busyWithCommand = false;
      websocketCallback( ev.data );
    }
  };
};

//-------------------------------------------------------------------------------------------------

function ConAirExecute( objectName, methodName, parameters, callback )
{
  if( !busyWithCommand )
  {
    var script = objectName + "." + methodName + '(';

    for( var i = 2; i < arguments.length - 1; i++ )
    {
      if( typeof arguments[i] == 'string' )
      {
        script += "'" + arguments[i].replace(/'/g, "\\'") + "'";
      }
      else
      {
        script += arguments[i];
      }
      
      if( i != arguments.length - 2 )
      {
        script += ",";
      }
    }

    script += ")";

    var callbackFunction = arguments[arguments.length - 1];

    websocketCallback = function( returnValue )
    {
      callbackFunction( returnValue );
    };

    busyWithCommand = true;
    websocket.send( script );
  }
}

//=================================================================================================