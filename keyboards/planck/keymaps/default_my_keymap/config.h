#pragma once

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(PLANCK_SOUND)
    // #define STARTUP_SONG SONG(NO_SOUND)

    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND), \
                                  SONG(COLEMAK_SOUND), \
                                  SONG(DVORAK_SOUND) \
                                }
#endif

/*
 * MIDI options
 */

/* Prevent use of disabled MIDI features in the keymap */
//#define MIDI_ENABLE_STRICT 1

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4

/*************************************************************/
/* https://zenn.dev/mst_nishio/articles/e2dd64d14187b9b4de57 */
/*************************************************************/

/********************************************/
/* https://docs.qmk.fm/#/feature_mouse_keys */
/********************************************/

// Mouse Settings

// Delay between pressing a movement key and cursor movement (10)
// ... I can't see the difference though
#undef  MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 10

// Time between cursor movements in milliseconds (20)
#undef  MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 1

// Step size (8)
#undef MOUSEKEY_MOVE_DELTA
#define MOUSEKEY_MOVE_DELTA 1

// Maximum cursor speed at which acceleration stops (10)
#undef  MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 14

// Time until maximum cursor speed is reached (30)
// Set 0 to disable accleration
#undef  MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 20

// Mouse Wheel Settings

// Delay between pressing a wheel key and wheel movement (10)
#undef  MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY 10

// Time between wheel movements (80)
#undef  MOUSEKEY_WHEEL_INTERVAL
#define MOUSEKEY_WHEEL_INTERVAL 80

// Maximum number of scroll steps per scroll action (8)
#undef  MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 8

// Time until maximum scroll speed is reached (40)
// set 0 to disable accleration
#undef  MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40








