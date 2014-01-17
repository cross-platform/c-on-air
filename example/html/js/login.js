$( document ).ready( function( )
{
  $( "#email" ).focus( );

	//-------------------------------------------------------------------------------------------------

  $( "#email" ).keyup( function( event )
  {
    if( event.keyCode == 13 )
    {
      $( "#password" ).focus( );
    }
  } );

	//-------------------------------------------------------------------------------------------------

  $( "#password" ).keyup( function( event )
  {
    if( event.keyCode == 13 )
    {
      parent.ConAirExecute( "CoaController", "LogIn", $( "#email" ).val( ), $( "#password" ).val( ), LogInReply );
    }
  } );

} );

//-------------------------------------------------------------------------------------------------

function LogInReply( message )
{
  if( message == 'true' )
  {
    parent.scrollToHome( );
  }
  else
  {
    alert( "Email and / or Password incorrect. Please try again." )
  }
}

//=================================================================================================