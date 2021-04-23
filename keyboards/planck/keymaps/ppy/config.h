#pragma once

#define NO_DEBUG
#define NO_PRINT
#define AUDIO_CLICKY
#define NO_USB_STARTUP_CHECK

#define MK_VARIANT MK_TYPE_KINETIC

#define MK_KINETIC_MOUSE_MAXS 120
#define MK_KINETIC_MOUSE_ACCN 12
#define MK_KINETIC_MOUSE_FRIC 18
#define MK_KINETIC_MOUSE_DRAG 2

#define MK_KINETIC_WHEEL_MAXS 2
#define MK_KINETIC_WHEEL_ACCN 6

#ifdef AUDIO_ENABLE
  #define STARTUP_SONG SONG(S__NOTE(_E4))
  #define GOODBYE_SONG SONG(S__NOTE(_C4))
#endif

// for sanwa keyboard emulation KVM
#undef NKRO_ENABLE

