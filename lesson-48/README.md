[[<-back](../README.md)]

# Atomic Operations

[Semaphores](../lesson-47/README.md) operate at an operating system level. Atomic operations are a way to lock data at an efficient CPU level. Here we'll be locking a critical section using CPU spinlocks.

![preview](./md/preview.png)

---

### Coding

Instead of a semaphore we'll be using a spinlock to protect our data buffer.

``` C++
//  Data access spin lock
SDL_SpinLock gDataLock = 0;

//  The "data buffer"
int gData = -1;
```

Unlike semaphores, spin locks do not need to be allocated and deallocated.

``` C++
bool loadMedia()
{
    //  Loading success flag
    bool success = true;

    //  Load splash texture
    if  ( !gSplashTexture.loadFromFile( "./splash.png" ) )
    {
        printf( "Failed to load splash texture!\n" );
        success = false;
    }

    return success;
}

void close()
{
    //  Free loaded images
    gSplashTexture.free();

    //  Destroy window
    SDL_DestroyRenderer ( gRenderer );
    SDL_DestroyWindow   ( gWindow );
    gWindow     = NULL;
    gRenderer   = NULL;

    //  Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
```

Here our critical section is protected by [`SDL_AtomicLock`](http://wiki.libsdl.org/SDL_AtomicLock) and [`SDL_AtomicUnlock`](http://wiki.libsdl.org/SDL_AtomicUnlock).

In this case it may seem like semaphores and atomic locks are the same, but remember that semaphores can allow access beyond a single thread. Atomic operations are for when you want a strict locked/unlocked state.

``` C++
int worker( void* data )
{
    printf( "%s starting...\n", data );

    //  Pre thread random seeding
    srand( SDL_GetTicks() );

    //  Work 5 times
    for ( int i = 0; i < 5; ++i )
    {
        //  Wait randomly
        SDL_Delay( 16 + rand() % 32 );

        //  Lock
        SDL_AtomicLock( &gDataLock );

        //  Print pre work data
        printf( "%s gets %d\n", data, gData );

        //  "Work"
        gData = rand() % 256;

        //  Print post work data
        printf( "%s sets %d\n\n", data, gData );
        
        //  Unlock
        SDL_AtomicUnlock( &gDataLock );

        //  Wait randomly
        SDL_Delay( 16 + rand() % 640 );
    }

    printf( "%s finished!\n\n", data );

    return 0;
}
```

---

[[<-back](../README.md)]

