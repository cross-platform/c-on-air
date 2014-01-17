currentPage = 0;

//=================================================================================================

$( document ).ready( function( )
{
  scrollToHome = function( )
  {
    $( '#wrapper' ).scrollTo( $( "#home" ), 800 );
  }
} );

//-------------------------------------------------------------------------------------------------

$( window ).bind( 'orientationchange', function( e )
{
  resizePanel( );
} );

//-------------------------------------------------------------------------------------------------

$( window ).resize( function( )
{
  resizePanel( );
} );

//=================================================================================================

function resizePanel( )
{
  width = $( window ).width( );
  height = $( window ).height( );

  mask_width = width * $( '.page' ).length;

  $( '#wrapper, .page' ).css(
  {
    width : width,
    height : height
  } );
  $( '#mask' ).css(
  {
    width : mask_width,
    height : height
  } );

  $( '#wrapper' ).scrollTo( $( currentPage ), 0 );
}

//=================================================================================================