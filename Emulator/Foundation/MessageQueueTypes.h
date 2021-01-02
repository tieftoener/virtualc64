// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

// This file must conform to standard ANSI-C to be compatible with Swift.

#ifndef _MESSAGE_QUEUE_T_H
#define _MESSAGE_QUEUE_T_H

enum_long(MSGType)
{
    MSG_NONE = 0,
    
    // Emulator state
    MSG_CONFIG,
    MSG_POWER_ON,
    MSG_POWER_OFF,
    MSG_RUN,
    MSG_PAUSE,
    MSG_RESET,
    MSG_WARP_ON,
    MSG_WARP_OFF,
    MSG_MUTE_ON,
    MSG_MUTE_OFF,

    // ROMs
    MSG_BASIC_ROM_LOADED,
    MSG_CHAR_ROM_LOADED,
    MSG_KERNAL_ROM_LOADED,
    MSG_DRIVE_ROM_LOADED,
    MSG_ROM_MISSING,

    // CPU related messages
    MSG_CPU_OK,
    MSG_CPU_JAMMED,
    MSG_BREAKPOINT_REACHED,
    MSG_WATCHPOINT_REACHED,

    // VIC related messages
    MSG_PAL,
    MSG_NTSC,

    // IEC Bus
    MSG_IEC_BUS_BUSY,
    MSG_IEC_BUS_IDLE,
    
    // Floppy drives
    MSG_DRIVE_CONNECT,
    MSG_DRIVE_DISCONNECT,
    MSG_DRIVE_POWER_ON,
    MSG_DRIVE_POWER_OFF,
    MSG_DRIVE_ACTIVE,
    MSG_DRIVE_INACTIVE,
    MSG_DRIVE_READ,
    MSG_DRIVE_WRITE,
    MSG_DRIVE_LED_ON,
    MSG_DRIVE_LED_OFF,
    MSG_DRIVE_MOTOR_ON,
    MSG_DRIVE_MOTOR_OFF,
    MSG_DRIVE_HEAD,
    MSG_DISK_INSERTED,
    MSG_DISK_EJECTED,
    MSG_DISK_SAVED,
    MSG_DISK_UNSAVED,
    MSG_DISK_PROTECT,
    MSG_FILE_FLASHED,

    // Peripherals (Datasette)
    MSG_VC1530_TAPE,
    MSG_VC1530_NO_TAPE,
    MSG_VC1530_PROGRESS,

    // Peripherals (Expansion port)
    MSG_CARTRIDGE,
    MSG_NO_CARTRIDGE,
    MSG_CART_SWITCH,

    // Peripherals (Keyboard)
    MSG_KB_AUTO_RELEASE,
    
    // Snapshots
    MSG_AUTO_SNAPSHOT_TAKEN,
    MSG_USER_SNAPSHOT_TAKEN,
    MSG_SNAPSHOT_RESTORED
};
typedef MSGType MsgType;

inline bool isMsgType(long value)
{
    return (unsigned long)value <= MSG_SNAPSHOT_RESTORED;
}

inline const char *MsgTypeName(MsgType value)
{
    switch (value) {
                        
        case MSG_NONE:                 return "NONE";
        case MSG_CONFIG:               return "CONFIG";
        case MSG_POWER_ON:             return "POWER_ON";
        case MSG_POWER_OFF:            return "POWER_OFF";
        case MSG_RUN:                  return "RUN";
        case MSG_PAUSE:                return "PAUSE";
        case MSG_RESET:                return "RESET";
        case MSG_WARP_ON:              return "WARP_ON";
        case MSG_WARP_OFF:             return "WARP_OFF";
        case MSG_MUTE_ON:              return "MUTE_ON";
        case MSG_MUTE_OFF:             return "MUTE_OFF";

        case MSG_BASIC_ROM_LOADED:     return "BASIC_ROM_LOADED";
        case MSG_CHAR_ROM_LOADED:      return "CHAR_ROM_LOADED";
        case MSG_KERNAL_ROM_LOADED:    return "KERNAL_ROM_LOADED";
        case MSG_DRIVE_ROM_LOADED:     return "DRIVE_ROM_LOADED";
        case MSG_ROM_MISSING:          return "ROM_MISSING";

        case MSG_CPU_OK:               return "CPU_OK";
        case MSG_CPU_JAMMED:           return "CPU_JAMMED";
        case MSG_BREAKPOINT_REACHED:   return "BREAKPOINT_REACHED";
        case MSG_WATCHPOINT_REACHED:   return "WATCHPOINT_REACHED";

        case MSG_PAL:                  return "PAL";
        case MSG_NTSC:                 return "NTSC";

        case MSG_IEC_BUS_BUSY:         return "IEC_BUS_BUSY";
        case MSG_IEC_BUS_IDLE:         return "IEC_BUS_IDLE";
            
        case MSG_DRIVE_CONNECT:        return "DRIVE_CONNECT";
        case MSG_DRIVE_DISCONNECT:     return "DRIVE_DISCONNECT";
        case MSG_DRIVE_POWER_ON:       return "DRIVE_POWER_ON";
        case MSG_DRIVE_POWER_OFF:      return "DRIVE_POWER_OFF";
        case MSG_DRIVE_ACTIVE:         return "DRIVE_ACTIVE";
        case MSG_DRIVE_INACTIVE:       return "DRIVE_INACTIVE";
        case MSG_DRIVE_READ:           return "DRIVE_READ";
        case MSG_DRIVE_WRITE:          return "DRIVE_WRITE";
        case MSG_DRIVE_LED_ON:         return "DRIVE_LED_ON";
        case MSG_DRIVE_LED_OFF:        return "DRIVE_LED_OFF";
        case MSG_DRIVE_MOTOR_ON:       return "DRIVE_MOTOR_ON";
        case MSG_DRIVE_MOTOR_OFF:      return "DRIVE_MOTOR_OFF";
        case MSG_DRIVE_HEAD:           return "DRIVE_HEAD";
        case MSG_DISK_INSERTED:        return "DISK_INSERTED";
        case MSG_DISK_EJECTED:         return "DISK_EJECTED";
        case MSG_DISK_SAVED:           return "DISK_SAVED";
        case MSG_DISK_UNSAVED:         return "DISK_UNSAVED";
        case MSG_DISK_PROTECT:         return "DISK_PROTECT";
        case MSG_FILE_FLASHED:         return "FILE_FLASHED";

        case MSG_VC1530_TAPE:          return "VC1530_TAPE";
        case MSG_VC1530_NO_TAPE:       return "VC1530_NO_TAPE";
        case MSG_VC1530_PROGRESS:      return "VC1530_PROGRESS";

        case MSG_CARTRIDGE:            return "CARTRIDGE";
        case MSG_NO_CARTRIDGE:         return "NO_CARTRIDGE";
        case MSG_CART_SWITCH:          return "CART_SWITCH";

        case MSG_KB_AUTO_RELEASE:      return "KB_AUTO_RELEASE";
            
        case MSG_AUTO_SNAPSHOT_TAKEN:  return "AUTO_SNAPSHOT_TAKEN";
        case MSG_USER_SNAPSHOT_TAKEN:  return "USER_SNAPSHOT_TAKEN";
        case MSG_SNAPSHOT_RESTORED:    return "SNAPSHOT_RESTORED";
    }
    return "???";
}

typedef struct
{
    MsgType type;
    long data;
}
Message;

// Callback function signature
typedef void Callback(const void *, long, long);

#endif 
