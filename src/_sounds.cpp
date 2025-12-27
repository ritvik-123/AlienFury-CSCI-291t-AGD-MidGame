#include "_sounds.h"

// Constructor
_sounds::_sounds()
{
    // Initialize sound engine in your actual implementation
}

// Destructor - clean up sound engine
_sounds::~_sounds()
{
    eng->drop(); // Release sound engine resources
}

// Play background music in loop
void _sounds::playMusic(char* fileName)
{
    eng->play2D(fileName, true); // true = looped playback
}

// Play sound effect once
void _sounds::PlaySoundA(char* fileName)
{
    if(!eng->isCurrentlyPlaying(fileName))
        eng->play2D(fileName, false, false); // false = no loop, no track
}

// Pause currently playing sound
void _sounds::pauseSound(char* fileName)
{
    eng->play2D(fileName, true, false); // Pause playback
}

// Initialize sound engine
int _sounds::initSound()
{
    if(!eng)
    {
        // Sound engine failed to initialize
        return 0; // Return 0 for failure
    }
    return 1; // Return 1 for success
}
