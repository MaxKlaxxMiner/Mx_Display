#include "Mx_Display_ILI9341.h"

#define MxDisplayWidth 240
#define MxDisplayHeight 320

// --- Basic Reference Implementation ---

#define MxClassName Mx_DisplayV_ILI9341_Reference

Mx_DisplayV_ILI9341_Reference::Mx_DisplayV_ILI9341_Reference(bool rotate180)
{
  _rdPin = A0;
  _wrPin = A1;
  _cdPin = A2;
  _csPin = A3;
  _rsPin = A4;
  _data0Pin = 8;
  _data1Pin = 9;
  _data2Pin = 2;
  _data3Pin = 3;
  _data4Pin = 4;
  _data5Pin = 5;
  _data6Pin = 6;
  _data7Pin = 7;
  begin(rotate180);
}

Mx_DisplayV_ILI9341_Reference::Mx_DisplayV_ILI9341_Reference(bool rotate180, uint8_t rdPin, uint8_t wrPin, uint8_t cdPin, uint8_t csPin, uint8_t rsPin,
                                                             uint8_t data0Pin, uint8_t data1Pin, uint8_t data2Pin, uint8_t data3Pin,
                                                             uint8_t data4Pin, uint8_t data5Pin, uint8_t data6Pin, uint8_t data7Pin)
{
  _rdPin = rdPin;
  _wrPin = wrPin;
  _cdPin = cdPin;
  _csPin = csPin;
  _rsPin = rsPin;
  _data0Pin = data0Pin;
  _data1Pin = data1Pin;
  _data2Pin = data2Pin;
  _data3Pin = data3Pin;
  _data4Pin = data4Pin;
  _data5Pin = data5Pin;
  _data6Pin = data6Pin;
  _data7Pin = data7Pin;
  begin(rotate180);
}

#define MxReference
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxReference

#define MxDisplayWidth 320
#define MxDisplayHeight 240
#define MxHorizontal

#define MxClassName Mx_DisplayH_ILI9341_Reference

Mx_DisplayH_ILI9341_Reference::Mx_DisplayH_ILI9341_Reference(bool rotate180)
{
  _rdPin = A0;
  _wrPin = A1;
  _cdPin = A2;
  _csPin = A3;
  _rsPin = A4;
  _data0Pin = 8;
  _data1Pin = 9;
  _data2Pin = 2;
  _data3Pin = 3;
  _data4Pin = 4;
  _data5Pin = 5;
  _data6Pin = 6;
  _data7Pin = 7;
  begin(rotate180);
}

Mx_DisplayH_ILI9341_Reference::Mx_DisplayH_ILI9341_Reference(bool rotate180, uint8_t rdPin, uint8_t wrPin, uint8_t cdPin, uint8_t csPin, uint8_t rsPin,
                                                             uint8_t data0Pin, uint8_t data1Pin, uint8_t data2Pin, uint8_t data3Pin,
                                                             uint8_t data4Pin, uint8_t data5Pin, uint8_t data6Pin, uint8_t data7Pin)
{
  _rdPin = rdPin;
  _wrPin = wrPin;
  _cdPin = cdPin;
  _csPin = csPin;
  _rsPin = rsPin;
  _data0Pin = data0Pin;
  _data1Pin = data1Pin;
  _data2Pin = data2Pin;
  _data3Pin = data3Pin;
  _data4Pin = data4Pin;
  _data5Pin = data5Pin;
  _data6Pin = data6Pin;
  _data7Pin = data7Pin;
  begin(rotate180);
}

#define MxReference
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxReference
#undef MxHorizontal

#define MxPinFixed

// --- Config: Basic ---

#define MxDisplayWidth 240
#define MxDisplayHeight 320

Mx_DisplayV_ILI9341_CompactMinimal::Mx_DisplayV_ILI9341_CompactMinimal(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_CompactMinimal
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayV_ILI9341_Compact::Mx_DisplayV_ILI9341_Compact(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Compact
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayV_ILI9341_Fast::Mx_DisplayV_ILI9341_Fast(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fast
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayV_ILI9341_Fastest::Mx_DisplayV_ILI9341_Fastest(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fastest
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#define MxDisplayWidth 320
#define MxDisplayHeight 240
#define MxHorizontal

Mx_DisplayH_ILI9341_CompactMinimal::Mx_DisplayH_ILI9341_CompactMinimal(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_CompactMinimal
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayH_ILI9341_Compact::Mx_DisplayH_ILI9341_Compact(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Compact
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayH_ILI9341_Fast::Mx_DisplayH_ILI9341_Fast(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fast
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayH_ILI9341_Fastest::Mx_DisplayH_ILI9341_Fastest(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fastest
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#undef MxHorizontal


// --- Config: NoSerial ---

#define MxNoSerial

#define MxDisplayWidth 240
#define MxDisplayHeight 320

Mx_DisplayV_ILI9341_CompactMinimal_NoSerial::Mx_DisplayV_ILI9341_CompactMinimal_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_CompactMinimal_NoSerial
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayV_ILI9341_MOD_CompactMinimal_NoSerial::Mx_DisplayV_ILI9341_MOD_CompactMinimal_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_MOD_CompactMinimal_NoSerial
#define MxMod
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxCompact
#undef MxMinimal

Mx_DisplayV_ILI9341_Compact_NoSerial::Mx_DisplayV_ILI9341_Compact_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Compact_NoSerial
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayV_ILI9341_MOD_Compact_NoSerial::Mx_DisplayV_ILI9341_MOD_Compact_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_MOD_Compact_NoSerial
#define MxMod
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxCompact

Mx_DisplayV_ILI9341_Fast_NoSerial::Mx_DisplayV_ILI9341_Fast_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fast_NoSerial
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayV_ILI9341_MOD_Fast_NoSerial::Mx_DisplayV_ILI9341_MOD_Fast_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_MOD_Fast_NoSerial
#define MxMod
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod

Mx_DisplayV_ILI9341_Fastest_NoSerial::Mx_DisplayV_ILI9341_Fastest_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fastest_NoSerial
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial::Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial
#define MxMod
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxFast

#define MxDisplayWidth 320
#define MxDisplayHeight 240
#define MxHorizontal

Mx_DisplayH_ILI9341_CompactMinimal_NoSerial::Mx_DisplayH_ILI9341_CompactMinimal_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_CompactMinimal_NoSerial
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayH_ILI9341_MOD_CompactMinimal_NoSerial::Mx_DisplayH_ILI9341_MOD_CompactMinimal_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_MOD_CompactMinimal_NoSerial
#define MxMod
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxCompact
#undef MxMinimal

Mx_DisplayH_ILI9341_Compact_NoSerial::Mx_DisplayH_ILI9341_Compact_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Compact_NoSerial
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayH_ILI9341_MOD_Compact_NoSerial::Mx_DisplayH_ILI9341_MOD_Compact_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_MOD_Compact_NoSerial
#define MxMod
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxCompact

Mx_DisplayH_ILI9341_Fast_NoSerial::Mx_DisplayH_ILI9341_Fast_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fast_NoSerial
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayH_ILI9341_MOD_Fast_NoSerial::Mx_DisplayH_ILI9341_MOD_Fast_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_MOD_Fast_NoSerial
#define MxMod
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod

Mx_DisplayH_ILI9341_Fastest_NoSerial::Mx_DisplayH_ILI9341_Fastest_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fastest_NoSerial
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

Mx_DisplayH_ILI9341_MOD_Fastest_NoSerial::Mx_DisplayH_ILI9341_MOD_Fastest_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_MOD_Fastest_NoSerial
#define MxMod
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxMod
#undef MxFast

#undef MxHorizontal

#undef MxNoSerial


// --- Config: NoSd ---

#define MxNoSd

#define MxDisplayWidth 240
#define MxDisplayHeight 320

Mx_DisplayV_ILI9341_CompactMinimal_NoSd::Mx_DisplayV_ILI9341_CompactMinimal_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_CompactMinimal_NoSd
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayV_ILI9341_Compact_NoSd::Mx_DisplayV_ILI9341_Compact_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Compact_NoSd
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayV_ILI9341_Fast_NoSd::Mx_DisplayV_ILI9341_Fast_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fast_NoSd
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayV_ILI9341_Fastest_NoSd::Mx_DisplayV_ILI9341_Fastest_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fastest_NoSd
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#define MxDisplayWidth 320
#define MxDisplayHeight 240
#define MxHorizontal

Mx_DisplayH_ILI9341_CompactMinimal_NoSd::Mx_DisplayH_ILI9341_CompactMinimal_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_CompactMinimal_NoSd
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayH_ILI9341_Compact_NoSd::Mx_DisplayH_ILI9341_Compact_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Compact_NoSd
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayH_ILI9341_Fast_NoSd::Mx_DisplayH_ILI9341_Fast_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fast_NoSd
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayH_ILI9341_Fastest_NoSd::Mx_DisplayH_ILI9341_Fastest_NoSd(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fastest_NoSd
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#undef MxHorizontal

#undef MxNoSd


// --- Config: NoSd_NoSerial ---

#define MxNoSerial
#define MxNoSd

#define MxDisplayWidth 240
#define MxDisplayHeight 320

Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial::Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial::Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial::Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial::Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#define MxDisplayWidth 320
#define MxDisplayHeight 240
#define MxHorizontal

Mx_DisplayH_ILI9341_CompactMinimal_NoSd_NoSerial::Mx_DisplayH_ILI9341_CompactMinimal_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_CompactMinimal_NoSd_NoSerial
#define MxCompact
#define MxMinimal
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact
#undef MxMinimal

Mx_DisplayH_ILI9341_Compact_NoSd_NoSerial::Mx_DisplayH_ILI9341_Compact_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Compact_NoSd_NoSerial
#define MxCompact
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxCompact

Mx_DisplayH_ILI9341_Fast_NoSd_NoSerial::Mx_DisplayH_ILI9341_Fast_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fast_NoSd_NoSerial
#include "core/Mx_Display_ILI9341_Core.h"

Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial::Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial(bool rotate180) { begin(rotate180); }
#define MxClassName Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial
#define MxFast
#include "core/Mx_Display_ILI9341_Core.h"
#undef MxFast

#undef MxHorizontal

#undef MxNoSerial
#undef MxNoSd


#undef MxPinFixed

#undef MxClassName
#undef MxDisplayWidth
#undef MxDisplayHeight
