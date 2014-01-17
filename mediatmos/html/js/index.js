$( document ).ready( function( )
{
  $( "#artist" ).focus( );

  //-------------------------------------------------------------------------------------------------

  $( "#artist" ).keyup( function( event )
  {
    if( event.keyCode == 13 && $( "#artist" ).val( ) )
    {
      $( "#artist" ).prop( 'disabled', true );
      ConAirExecute( "CoaController", "FindAlbums", $( "#artist" ).val( ), FindReply );
    }
  } );

} );

//-------------------------------------------------------------------------------------------------

var _album_count;
var _album_index;
var _album_list;

function FindReply( album_count )
{
	_album_count = album_count;
	
  if( _album_count > 0 )
  {
    _album_index = 0;
    _album_list = "";
    ConAirExecute( "CoaController", "GetAlbum", _album_index++, GetReply );
  }
	else
	{
		$( "#artist" ).prop( 'disabled', false );
	}	
}

function GetReply( message )
{
  _album_list += message + '\n';

	if( _album_index < _album_count )
	{
    ConAirExecute( "CoaController", "GetAlbum", _album_index++, GetReply );
	}
	else
	{
		alert( _album_list );
		$( "#artist" ).val('');
		$( "#artist" ).prop( 'disabled', false );
	}	
}

//=================================================================================================