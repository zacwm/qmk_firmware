#pragma once

#define NO_DEBUG
#define NO_PRINT
#define AUDIO_CLICKY
#define NO_USB_STARTUP_CHECK

#ifdef AUDIO_ENABLE
  #define STARTUP_SONG SONG(S__NOTE(_E4))
  #define GOODBYE_SONG SONG(S__NOTE(_C4))
#endif

// for sanwa keyboard emulation KVM
#undef NKRO_ENABLE

