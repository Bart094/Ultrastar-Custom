program engine_test;
(*
 * This code was created by Jeff Molofee '99
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc.
 * email to leggett@eecs.tulane.edu
 *)

{$IFDEF FPC}
  {$mode delphi}{$H+}
{$ENDIF}

{$APPTYPE Console}

uses
  moduleloader in '../../JEDI-SDL/SDL/Pas/moduleloader.pas',
  SDL          in '../../JEDI-SDL/SDL/Pas/sdl.pas',
  gl           in '../../JEDI-SDL/OpenGL/Pas/gl.pas',
  glext        in '../../JEDI-SDL/OpenGL/Pas/glext.pas',
  glu          in '../../JEDI-SDL/OpenGL/Pas/glu.pas',
  {$IFNDEF FPC}
  ctypes       in '../../ctypes/ctypes.pas',
  {$ENDIF}
  FreeType     in '../freetype.pas',
  UFont        in 'UFont.pas',
  //UFont        in '../../../base/UFont.pas',
  UUnicodeUtils in '../../../base/UUnicodeUtils.pas',
  math,
  sysutils;

const
  // screen width, height, and bit depth
  SCREEN_WIDTH  = 640;
  SCREEN_HEIGHT = 480;
  SCREEN_BPP    =  16;

  //FONT_FILE = 'Test.ttf';
  //FONT_FILE = 'C:/Windows/Fonts/Arial.ttf';
  //FONT_FILE = 'C:/Windows/Fonts/SimSun.ttf';
  //FONT_FILE = 'eurostarregularextended.ttf';
  FONT_FILE = '../../../../game/fonts/FreeSans/FreeSans.ttf';
  
var
  OurFont: TScalableFont;
  // This is our SDL surface
  surface: PSDL_Surface;
  cnt1, cnt2: GLfloat;

(* function to release/destroy our resources and restoring the old desktop *)
procedure Quit(returnCode: integer);
begin
  OurFont.Free;

  // clean up the window
  SDL_Quit( );

  // and exit appropriately
  Halt( returnCode );
end;

(* function to reset our viewport after a window resize *)
function resizeWindow(width: integer; height: integer): boolean;
begin
  // Protect against a divide by zero
  if ( height = 0 ) then
    height := 1;

  // Setup our viewport.
  glViewport( 0, 0, GLsizei(width), GLsizei(height) );

  // change to the projection matrix and set our viewing volume.
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );

  // Set our perspective
  //gluOrtho2D(0, width, 0, height);
  gluOrtho2D(0, 800, 0, 600);

  // Make sure we're chaning the model view and not the projection
  glMatrixMode( GL_MODELVIEW );

  // Reset The View
  glLoadIdentity( );

  Result := true;
end;

(* function to handle key press events *)
procedure handleKeyPress(keysym: PSDL_keysym);
begin
  case ( keysym^.sym ) of
    SDLK_ESCAPE:
    begin
      // ESC key was pressed
      Quit( 0 );
    end;
    SDLK_F1:
    begin
      // F1 key was pressed
      // this toggles fullscreen mode
      SDL_WM_ToggleFullScreen( surface );
    end;
  end;
end;

(* general OpenGL initialization function *)
function initGL(): boolean;
begin
  // Enable smooth shading
  glShadeModel( GL_SMOOTH );

  // Set the background black
  //glClearColor( 1, 1, 1.0, 1.0 );
  //glClearColor( 0.3, 0.7, 1.0, 1.0 );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );

  // Depth buffer setup
  glClearDepth( 1.0 );

  // Enables Depth Testing
  glEnable( GL_DEPTH_TEST );

  // The Type Of Depth Test To Do
  glDepthFunc( GL_LEQUAL );

  // Really Nice Perspective Calculations
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  OurFont := TFTScalableFont.Create(FONT_FILE, 64, 0.03);
  //OurFont := TFTFont.Create(FONT_FILE, 128);
  //OurFont := TFTScalableOutlineFont.Create(FONT_FILE, 64, 0.03);
  //OurFont.UseKerning := false;
  //TFTScalableOutlineFont(OurFont).SetOutlineColor(1, 0, 0, 1);
  //OurFont := TOutlineFont.Create(FONT_FILE, 32, 2);
  //OurFont.LineSpacing := OurFont.LineSpacing * 0.5;

  Result := true;
end;

var
  NextTime: cardinal;
  Counter: integer;

type
  TVector4d = array[0..3] of GLdouble;

function NewVector4d(a, b, c, d: GLdouble): TVector4d;
begin
  Result[0] := a;
  Result[1] := b;
  Result[2] := c;
  Result[3] := d;
end;

(* Here goes our drawing code *)
function drawGLScene(): boolean;
var
  msg: WideString;
  bounds: TBoundsDbl;
begin
  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);  // Clear Screen And Depth Buffer

  //msg := 'Here'#13'there'#13'be'#13#13'newlines'#13'.';
  //msg := 'Here'#13'newlines';
  msg := 'Active FreeType Text - ' + FloatToStr(cnt1);
  //msg := 'Hören'#13'其自诞生至今'#13'спецификация';

  // Red text
  glLoadIdentity();
  glTranslatef(cnt2, 240, 0);
  if (cnt2 > 800) then
    cnt2 := 0;
  glTranslatef(30, 40, 0);
  //glTranslatef(320, 240, 0);
  //glRotatef(cnt1, 0, 0, 1);
  //glScalef(1, 0.8 + 0.3*cos(cnt1/5), 1);

  OurFont.Style := [Italic, {Underline,} Reflect];
  //OurFont.GlyphSpacing := 10;
  //OurFont.SetOutlineColor(0.5, 0.5, 0.5);
  //OurFont.ReflectionSpacing := -4;
  //OurFont.UseKerning := false;
  OurFont.Height := 150;//cnt2;
  //OurFont.Reset;
  //OurFont.Aspect := 2;

  glColor3f(1, 1, 0);
  bounds := OurFont.BBox(msg);
  //glRectf(bounds.Left, OurFont.Ascender, bounds.Right, OurFont.Ascender-OurFont.Height);

  glColor4f(1, 1, 1, 1);
  //OurFont.ReflectionSpacing := 0;
  OurFont.Print(msg);

  cnt1 := cnt1 + 0.051;  // Increase The First Counter
  cnt2 := cnt2 + 0.005;  // Increase The First Counter

  SDL_GL_SwapBuffers( );

  Inc(Counter);

  if (NextTime < SDL_GetTicks()) then
  begin
    NextTime := SDL_GetTicks() + 2000;
    writeln('FPS: ' + floattostr(Counter / 2.0));
    Counter := 0;
  end;

  Result := true;
end;

var
  // Flags to pass to SDL_SetVideoMode
  videoFlags: integer;
  // main loop variable
  done: boolean = false;
  // used to collect events
  event: TSDL_Event;
  // this holds some info about our display
  videoInfo: PSDL_VideoInfo;
  // whether or not the window is active
  isActive: boolean = true;

begin
  // initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO or SDL_INIT_TIMER ) < 0 ) then
  begin
    writeln( ErrOutput, 'Video initialization failed: ' + SDL_GetError() );
    Quit( 1 );
  end;

  // Fetch the video info
  videoInfo := SDL_GetVideoInfo( );

  if ( videoInfo = nil ) then
  begin
    writeln( ErrOutput, 'Video query failed: ' + SDL_GetError() );
    Quit( 1 );
  end;

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // Enable double buffering

  // the flags to pass to SDL_SetVideoMode
  videoFlags := SDL_OPENGL;                        // Enable OpenGL in SDL
  videoFlags := videoFlags or SDL_HWPALETTE;       // Store the palette in hardware
  videoFlags := videoFlags or SDL_RESIZABLE;       // Enable window resizing

  // This checks to see if surfaces can be stored in memory
  if ( videoInfo^.hw_available <> 0 ) then
    videoFlags := videoFlags or SDL_HWSURFACE
  else
    videoFlags := videoFlags or SDL_SWSURFACE;

  // This checks if hardware blits can be done
  if ( videoInfo^.blit_hw <> 0 ) then
    videoFlags := videoFlags or SDL_HWACCEL;

  // Sets up OpenGL double buffering
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  // get a SDL surface
  surface := SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                               videoFlags );

  // Verify there is a surface
  if ( surface = nil ) then
  begin
    writeln( ErrOutput, 'Video mode set failed: ' + SDL_GetError() );
    Quit( 1 );
  end;

  // initialize OpenGL
  initGL();

  // resize the initial window
  resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

  // wait for events
  while ( not done ) do
  begin
    { handle the events in the queue }

    while ( SDL_PollEvent( @event ) <> 0 ) do
    begin
      case( event.type_ ) of
        SDL_ACTIVEEVENT:
        begin
          // Something's happend with our focus
          // If we are iconified, we shouldn't draw the screen
          if ( (event.active.state and SDL_APPACTIVE) <> 0 ) then
          begin
            if (  event.active.gain = 0 ) then
              isActive := false
            else
              isActive := true;
          end;
        end;
        SDL_VIDEORESIZE:
        begin
          // handle resize event
          {$IFDEF UNIX}
          surface := SDL_SetVideoMode( event.resize.w,
              event.resize.h,
              16, videoFlags );
          if ( surface = nil ) then
          begin
            writeln( ErrOutput, 'Could not get a surface after resize: ' + SDL_GetError( ) );
            Quit( 1 );
          end;
          {$ENDIF}
          resizeWindow( event.resize.w, event.resize.h );
        end;
        SDL_KEYDOWN:
        begin
          // handle key presses
          handleKeyPress( @event.key.keysym );
        end;
        SDL_QUITEV:
        begin
          // handle quit requests
          done := true;
        end;
      end;
    end;

    // draw the scene
    if ( isActive ) then
      drawGLScene( );
  end;

  // clean ourselves up and exit
  Quit( 0 );
end.
