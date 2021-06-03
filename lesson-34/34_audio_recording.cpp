/*  This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.    */

//  Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

//  Screen dimension constants
const int SCREEN_WIDTH	= 640;
const int SCREEN_HEIGHT = 480;

//  Maximum number of supported recording devices
const int MAX_RECORDING_DEVICES = 10;

//  Maximum recording time
const int MAX_RECORDING_SECONDS = 5;

//  Maximum recording time plus padding
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

//  The various recording actions we can take
enum RecordingState
{
	SELECTING_DEVICE    ,
	STOPPED             ,
	RECORDING           ,
	RECORDED,
	PLAYBACK,
	ERROR
};

//  Texture wrapper class
class LTexture
{
	public:
		//  Initializes variables
		LTexture();

		//  Deallocates memory
		~LTexture();

		//  Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//  Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//  Deallocates texture
		void free();

		//  Set color modulation
		void setColor       ( Uint8 red, Uint8 green, Uint8 blue );

		//  Set blending
		void setBlendMode   ( SDL_BlendMode blending );

		//  Set alpha modulation
		void setAlpha       ( Uint8 alpha );
		
		//  Renders texture at given point
		void render         ( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//  Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//  The actual hardware texture
		SDL_Texture*    mTexture;

		//  Image dimensions
		int mWidth;
		int mHeight;
};

//  Starts up SDL and creates window
bool init();

//  Loads media
bool loadMedia();

//  Frees media and shuts down SDL
void close();

//  Recording/playback callbacks
void audioRecordingCallback( void* userdata, Uint8* stream, int len );
void  audioPlaybackCallback( void* userdata, Uint8* stream, int len );

//  The window we'll be rendering to
SDL_Window*     gWindow     = NULL;

//  The window renderer
SDL_Renderer*   gRenderer   = NULL;

//  Globally used font
TTF_Font*       gFont       = NULL;
SDL_Color       gTextColor  = { 0, 0, 0, 0xFF };

//  Prompt texture
LTexture gPromptTexture;

//  The text textures that specify recording device names
LTexture gDeviceTextures[ MAX_RECORDING_DEVICES ];

//  Number of available devices
int gRecordingDeviceCount = 0;

//  Recieved audio spec
SDL_AudioSpec   gReceivedRecordingSpec;
SDL_AudioSpec   gReceivedPlaybackSpec;

//  Recording data buffer
Uint8*  gRecordingBuffer        = NULL;

//  Size of data buffer
Uint32  gBufferByteSize         = 0;

//  Position in data buffer
Uint32  gBufferBytePosition     = 0;

//  Maximum position in data buffer for recording
Uint32  gBufferByteMaxPosition  = 0;

LTexture::LTexture()
{
	//  Initialize
	mTexture    = NULL;
	mWidth      = 0;
	mHeight     = 0;
}

LTexture::~LTexture()
{
	//  Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//  Get rid of preexisting texture
	free();

	//  The final texture
	SDL_Texture*    newTexture  = NULL;

	//  Load image at specified path
	SDL_Surface*    loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//  Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//  Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if  ( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//  Get image dimensions
			mWidth  = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//  Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//  Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//  Get rid of preexisting texture
	free();

	//  Render text surface
	SDL_Surface*    textSurface =
        TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );

	if  ( textSurface != NULL )
	{
		//  Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if  ( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//  Get image dimensions
			mWidth  = textSurface->w;
			mHeight = textSurface->h;
		}

		//  Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//  Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//  Free texture if it exists
	if  ( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture    = NULL;
		mWidth      = 0;
		mHeight     = 0;
	}
}

void LTexture::setColor     ( Uint8 red, Uint8 green, Uint8 blue )
{
	//  Modulate texture rgb
	SDL_SetTextureColorMod  ( mTexture, red, green, blue );
}

void LTexture::setBlendMode ( SDL_BlendMode blending )
{
	//  Set blending function
	SDL_SetTextureBlendMode ( mTexture, blending );
}
		
void LTexture::setAlpha     ( Uint8 alpha )
{
	//  Modulate texture alpha
	SDL_SetTextureAlphaMod  ( mTexture, alpha );
}

void LTexture::render       ( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//  Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//  Set clip rendering dimensions
	if  ( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//  Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	//  Initialization flag
	bool success = true;

	//  Initialize SDL
	if  ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//  Set texture filtering to linear
		if  ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//  Create window
		gWindow =
            SDL_CreateWindow(
                "SDL Tutorial"          ,
                SDL_WINDOWPOS_UNDEFINED ,
                SDL_WINDOWPOS_UNDEFINED ,
                SCREEN_WIDTH            ,
                SCREEN_HEIGHT           ,
                SDL_WINDOW_SHOWN
            );

		if  ( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//  Create vsynced renderer for window
			gRenderer =
                SDL_CreateRenderer(
                    gWindow             ,
                    -1                  ,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                );

			if  ( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//  Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//  Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if  ( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//  Initialize SDL_ttf
				if  ( TTF_Init() == -1 )
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//  Loading success flag
	bool success = true;

	//  Open the font
	gFont = TTF_OpenFont( "./lazy.ttf", 28 );

	if  ( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//  Set starting prompt 
		gPromptTexture.loadFromRenderedText( "Select your recording device:", gTextColor );

		//  Get capture device count
		gRecordingDeviceCount = SDL_GetNumAudioDevices( SDL_TRUE );

		//  No recording devices
		if  ( gRecordingDeviceCount < 1 )
		{
			printf( "Unable to get audio capture device! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//  At least one device connected
		else
		{
			//  Cap recording device count
			if  ( gRecordingDeviceCount > MAX_RECORDING_DEVICES )
			{
				gRecordingDeviceCount = MAX_RECORDING_DEVICES;
			}

			//  Render device names
			std::stringstream promptText;
			for ( int i = 0; i < gRecordingDeviceCount; ++i )
			{
				//  Get capture device name
				promptText.str( "" );
				promptText << i << ": " << SDL_GetAudioDeviceName( i, SDL_TRUE );

				//  Set texture from name
				gDeviceTextures[ i ].loadFromRenderedText( promptText.str().c_str(), gTextColor );
			}
		}
	}

	return success;
}

void close()
{
	//  Free textures
	gPromptTexture.free();
	for ( int i = 0; i < MAX_RECORDING_DEVICES; ++i )
	{
		gDeviceTextures[ i ].free();
	}

	//  Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//  Destroy window	
	SDL_DestroyRenderer ( gRenderer );
	SDL_DestroyWindow   ( gWindow );
	gWindow     = NULL;
	gRenderer   = NULL;

	//  Free playback audio
	if  ( gRecordingBuffer != NULL )
	{
		delete[] gRecordingBuffer;
		gRecordingBuffer = NULL;
	}

	//  Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void audioRecordingCallback( void* /*userdata*/, Uint8* stream, int len )
{
	//  Copy audio from stream
	memcpy( &gRecordingBuffer[ gBufferBytePosition ], stream, len );

	//  Move along buffer
	gBufferBytePosition += len;
}

void audioPlaybackCallback( void* /*userdata*/, Uint8* stream, int len )
{
	//  Copy audio to stream
	memcpy( stream, &gRecordingBuffer[ gBufferBytePosition ], len );

	//  Move along buffer
	gBufferBytePosition += len;
}

int main( int /*argc*/, char* /*argv*/[] )
{
	//  Start up SDL and create window
	if  ( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//  Load media
		if  ( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//  Main loop flag
			bool quit = false;

			//  Event handler
			SDL_Event e;

			//  Set the default recording state
			RecordingState      currentState        = SELECTING_DEVICE;

			//  Audio device IDs
			SDL_AudioDeviceID   recordingDeviceId   = 0;
			SDL_AudioDeviceID   playbackDeviceId    = 0;

			//  While application is running
			while( !quit )
			{
				//  Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//  User requests quit
					if  ( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//  Do current state event handling
					switch  ( currentState )
					{
						//  User is selecting recording device
						case    SELECTING_DEVICE:

							//  On key press
							if  ( e.type == SDL_KEYDOWN )
							{
								//  Handle key press from 0 to 9 
								if  ( e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9 )
								{
									//  Get selection index
									int index = e.key.keysym.sym - SDLK_0;

									//  Index is valid
									if  ( index < gRecordingDeviceCount )
									{
										//  Default audio spec
										SDL_AudioSpec desiredRecordingSpec;
										SDL_zero(desiredRecordingSpec);
										desiredRecordingSpec.freq       = 44100;
										desiredRecordingSpec.format     = AUDIO_F32;
										desiredRecordingSpec.channels   = 2;
										desiredRecordingSpec.samples    = 4096;
										desiredRecordingSpec.callback   = audioRecordingCallback;

										//  Open recording device
										recordingDeviceId =
                                            SDL_OpenAudioDevice(
                                                SDL_GetAudioDeviceName( index, SDL_TRUE )       ,
                                                SDL_TRUE                                        ,
                                                  &desiredRecordingSpec                         ,
                                                &gReceivedRecordingSpec                         ,
                                                SDL_AUDIO_ALLOW_FORMAT_CHANGE
                                            );
										
										//  Device failed to open
										if  ( recordingDeviceId == 0 )
										{
											//  Report error
											printf( "Failed to open recording device! SDL Error: %s", SDL_GetError() );
											gPromptTexture.loadFromRenderedText( "Failed to open recording device!", gTextColor );
											currentState = ERROR;
										}
										//  Device opened successfully
										else
										{
											//  Default audio spec
											SDL_AudioSpec desiredPlaybackSpec;
											SDL_zero(desiredPlaybackSpec);
											desiredPlaybackSpec.freq        = 44100;
											desiredPlaybackSpec.format      = AUDIO_F32;
											desiredPlaybackSpec.channels    = 2;
											desiredPlaybackSpec.samples     = 4096;
											desiredPlaybackSpec.callback    = audioPlaybackCallback;

											//  Open playback device
											playbackDeviceId =
                                                SDL_OpenAudioDevice(
                                                    NULL                        ,
                                                    SDL_FALSE                   ,
                                                      &desiredPlaybackSpec      ,
                                                    &gReceivedPlaybackSpec      ,
                                                    SDL_AUDIO_ALLOW_FORMAT_CHANGE
                                                );

											//  Device failed to open
											if  ( playbackDeviceId == 0 )
											{
												//  Report error
												printf( "Failed to open playback device! SDL Error: %s", SDL_GetError() );
												gPromptTexture.loadFromRenderedText( "Failed to open playback device!", gTextColor );
												currentState = ERROR;
											}
											//  Device opened successfully
											else
											{
												//  Calculate per sample bytes
												int bytesPerSample  =
                                                    gReceivedRecordingSpec.channels *
                                                    ( SDL_AUDIO_BITSIZE( gReceivedRecordingSpec.format ) / 8 );

												//  Calculate bytes per second
												int bytesPerSecond  =
                                                    gReceivedRecordingSpec.freq     *
                                                    bytesPerSample;

												//  Calculate buffer size
												gBufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;

												//  Calculate max buffer use
												gBufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;

												//  Allocate and initialize byte buffer
												gRecordingBuffer = new Uint8[ gBufferByteSize ];
												memset( gRecordingBuffer, 0, gBufferByteSize );

												//  Go on to next state
												gPromptTexture.loadFromRenderedText("Press 1 to record for 5 seconds.", gTextColor);
												currentState = STOPPED;
											}
										}
									}
								}
							}
							break;	

						//  User getting ready to record
						case    STOPPED:

							//  On key press
							if  ( e.type == SDL_KEYDOWN )
							{
								//  Start recording
								if  ( e.key.keysym.sym == SDLK_1 )
								{
									//  Go back to beginning of buffer
									gBufferBytePosition = 0;

									//  Start recording
									SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

									//  Go on to next state
									gPromptTexture.loadFromRenderedText( "Recording...", gTextColor );
									currentState = RECORDING;
								}
							}
							break;	

						//  User has finished recording
						case    RECORDED:

							//  On key press
							if  ( e.type == SDL_KEYDOWN )
							{
								//  Start playback
								if  ( e.key.keysym.sym == SDLK_1 )
								{
									//  Go back to beginning of buffer
									gBufferBytePosition = 0;

									//  Start playback
									SDL_PauseAudioDevice( playbackDeviceId, SDL_FALSE );

									//  Go on to next state
									gPromptTexture.loadFromRenderedText( "Playing...", gTextColor );
									currentState = PLAYBACK;
								}
								//  Record again
								if  ( e.key.keysym.sym == SDLK_2 )
								{
									//  Reset the buffer
									gBufferBytePosition = 0;
									memset( gRecordingBuffer, 0, gBufferByteSize );

									//  Start recording
									SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

									//  Go on to next state
									gPromptTexture.loadFromRenderedText( "Recording...", gTextColor );
									currentState = RECORDING;
								}
							}
							break;

                        case    PLAYBACK:
                        case    RECORDING:
                        case    ERROR:
                                break;
					}
				}

				//  Updating recording
				if  ( currentState == RECORDING )
				{
					//  Lock callback
					SDL_LockAudioDevice( recordingDeviceId );

					//  Finished recording
					if  ( gBufferBytePosition > gBufferByteMaxPosition )
					{
						//  Stop recording audio
						SDL_PauseAudioDevice( recordingDeviceId, SDL_TRUE );

						//  Go on to next state
						gPromptTexture.loadFromRenderedText( "Press 1 to play back. Press 2 to record again.", gTextColor );
						currentState = RECORDED;
					}

					//  Unlock callback
					SDL_UnlockAudioDevice( recordingDeviceId );
				}
				//  Updating playback
				else if( currentState == PLAYBACK )
				{
					//  Lock callback
					SDL_LockAudioDevice( playbackDeviceId );

					//  Finished playback
					if  ( gBufferBytePosition > gBufferByteMaxPosition )
					{
						//  Stop playing audio
						SDL_PauseAudioDevice( playbackDeviceId, SDL_TRUE );

						//  Go on to next state
						gPromptTexture.loadFromRenderedText( "Press 1 to play back. Press 2 to record again.", gTextColor );
						currentState = RECORDED;
					}

					//  Unlock callback
					SDL_UnlockAudioDevice( playbackDeviceId );
				}

				//  Clear screen
				SDL_SetRenderDrawColor  ( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear         ( gRenderer );

				//  Render prompt centered at the top of the screen
				gPromptTexture.render   ( ( SCREEN_WIDTH - gPromptTexture.getWidth() ) / 2, 0 );

				//  User is selecting 
				if  ( currentState == SELECTING_DEVICE )
				{
					//  Render device names
					int yOffset = gPromptTexture.getHeight() * 2;
					for ( int i = 0; i < gRecordingDeviceCount; ++i )
					{
						gDeviceTextures[ i ].render( 0, yOffset );
						yOffset += gDeviceTextures[ i ].getHeight() + 1;
					}
				}

				//  Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//  Free resources and close SDL
	close();

	return 0;
}