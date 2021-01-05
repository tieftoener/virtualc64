// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

#include "C64Constants.h"
#include "C64PublicTypes.h"

// Forward declarations of proxy classes
@class MyController;
@class C64Proxy;
@class CPUProxy;
@class GuardsProxy;
@class MemoryProxy;
@class VICProxy;
@class CIAProxy;
@class SIDProxy;
@class KeyboardProxy;
@class ControlPortProxy;
@class IECProxy;
@class ExpansionPortProxy;
@class DriveProxy;
@class VIAProxy;
@class DiskProxy;
@class DatasetteProxy;
@class MouseProxy;

@class AnyFileProxy;
@class AnyCollectionProxy;
@class AnyDiskProxy;
@class CRTFileProxy;
@class TAPFileProxy;
@class SnapshotProxy;
@class T64FileProxy;
@class PRGFileProxy;
@class P00FileProxy;
@class D64FileProxy;
@class G64FileProxy;
@class FSDeviceProxy;

// Forward declarations of wrappers for C++ classes.
// We wrap classes into normal C structs to avoid any reference to C++.

struct C64Wrapper;
struct CpuWrapper;
struct GuardsWrapper;
struct MemoryWrapper;
struct VicWrapper;
struct CiaWrapper;
struct SidBridgeWrapper;
struct KeyboardWrapper;
struct ControlPortWrapper;
struct IecWrapper;
struct ExpansionPortWrapper;
struct FSDeviceWrapper;
struct FSDeviceWrapper;
struct DriveWrapper;
struct ViaWrapper;
struct DiskWrapper;
struct DatasetteWrapper;
struct MouseWrapper;
struct AnyFileWrapper;

//
// C64 proxy
//

@interface C64Proxy : NSObject {
    
    struct C64Wrapper *wrapper;
    
    CPUProxy *cpu;
    GuardsProxy *breakpoints;
    GuardsProxy *watchpoints;
    MemoryProxy *mem;
    VICProxy *vic;
    CIAProxy *cia1;
    CIAProxy *cia2;
    SIDProxy *sid;
    KeyboardProxy *keyboard;
    ControlPortProxy *port1;
    ControlPortProxy *port2;
    IECProxy *iec;
    ExpansionPortProxy *expansionport;
    DriveProxy *drive8;
    DriveProxy *drive9;
    DatasetteProxy *datasette;
    MouseProxy *mouse;
}

@property (readonly) struct C64Wrapper *wrapper;
@property (readonly, strong) CPUProxy *cpu;
@property (readonly, strong) GuardsProxy *breakpoints;
@property (readonly, strong) GuardsProxy *watchpoints;
@property (readonly, strong) MemoryProxy *mem;
@property (readonly, strong) VICProxy *vic;
@property (readonly, strong) CIAProxy *cia1;
@property (readonly, strong) CIAProxy *cia2;
@property (readonly, strong) SIDProxy *sid;
@property (readonly, strong) KeyboardProxy *keyboard;
@property (readonly, strong) ControlPortProxy *port1;
@property (readonly, strong) ControlPortProxy *port2;
@property (readonly, strong) IECProxy *iec;
@property (readonly, strong) ExpansionPortProxy *expansionport;
@property (readonly, strong) DriveProxy *drive8;
@property (readonly, strong) DriveProxy *drive9;
@property (readonly, strong) DatasetteProxy *datasette;
@property (readonly, strong) MouseProxy *mouse;

- (DriveProxy *) drive:(DriveID)id;

- (void) dealloc;
- (void) kill;

@property (readonly, getter=isReleaseBuild) BOOL releaseBuild;
- (void) enableDebugging;
- (void) disableDebugging;
- (void) setInspectionTarget:(InspectionTarget)target;
- (void) clearInspectionTarget;
@property (readonly) BOOL debugMode;

- (BOOL) isReady:(RomErrorCode *)error;
- (BOOL) isReady;
- (void) powerOn;
- (void) powerOff;
- (void) inspect;
- (void) reset;
- (void) dump;

@property (readonly, getter=isPoweredOn) BOOL poweredOn;
@property (readonly, getter=isPoweredOff) BOOL poweredOff;
@property (readonly, getter=isRunning) BOOL running;
@property (readonly, getter=isPaused) BOOL paused;

- (void) run;
- (void) pause;
- (void) suspend;
- (void) resume;

- (void) requestAutoSnapshot;
- (void) requestUserSnapshot;
@property (readonly) SnapshotProxy *latestAutoSnapshot;
@property (readonly) SnapshotProxy *latestUserSnapshot;
- (void) loadFromSnapshot:(SnapshotProxy *)proxy;

- (C64Configuration) config;
- (NSInteger) getConfig:(Option)opt;
- (NSInteger) getConfig:(Option)opt id:(NSInteger)id;
- (NSInteger) getConfig:(Option)opt drive:(DriveID)id;
- (BOOL) configure:(Option)opt value:(NSInteger)val;
- (BOOL) configure:(Option)opt enable:(BOOL)val;
- (BOOL) configure:(Option)opt id:(NSInteger)id value:(NSInteger)val;
- (BOOL) configure:(Option)opt id:(NSInteger)id enable:(BOOL)val;
- (BOOL) configure:(Option)opt drive:(DriveID)id value:(NSInteger)val;
- (BOOL) configure:(Option)opt drive:(DriveID)id enable:(BOOL)val;
- (void) configure:(C64Model)value;
- (C64Model) model;

// Accessing the message queue
- (Message)message;
- (void) addListener:(const void *)sender function:(Callback *)func;
- (void) removeListener:(const void *)sender;

// Running the emulator
- (void) stopAndGo;
- (void) stepInto;
- (void) stepOver;

- (BOOL) warp;
- (void) warpOn;
- (void) warpOff;

// Handling ROMs
- (BOOL) hasBasicRom;
- (BOOL) hasCharRom;
- (BOOL) hasKernalRom;
- (BOOL) hasVC1541Rom;

- (BOOL) hasMega65BasicRom;
- (BOOL) hasMega65CharRom;
- (BOOL) hasMega65KernelRom;

- (BOOL) isBasicRom:(NSURL *)url;
- (BOOL) isCharRom:(NSURL *)url;
- (BOOL) isKernalRom:(NSURL *)url;
- (BOOL) isVC1541Rom:(NSURL *)url;

- (BOOL) loadRom:(RomType)type url:(NSURL *)url error:(ErrorCode *)err;
/*
- (BOOL) loadBasicRomFromFile:(NSURL *)url;
- (BOOL) loadCharRomFromFile:(NSURL *)url;
- (BOOL) loadKernalRomFromFile:(NSURL *)url;
- (BOOL) loadVC1541RomFromFile:(NSURL *)url;
*/
- (BOOL) loadRom:(RomType)type data:(NSData *)data error:(ErrorCode *)err;
/*
- (BOOL) loadBasicRomFromBuffer:(NSData *)buffer;
- (BOOL) loadCharRomFromBuffer:(NSData *)buffer;
- (BOOL) loadKernalRomFromBuffer:(NSData *)buffer;
- (BOOL) loadVC1541RomFromBuffer:(NSData *)buffer;
*/

- (BOOL) saveBasicRom:(NSURL *)url;
- (BOOL) saveCharRom:(NSURL *)url;
- (BOOL) saveKernalRom:(NSURL *)url;
- (BOOL) saveVC1541Rom:(NSURL *)url;

- (void) deleteBasicRom;
- (void) deleteKernalRom;
- (void) deleteCharRom;
- (void) deleteVC1541Rom;

- (RomIdentifier) basicRomIdentifier;
- (RomIdentifier) kernalRomIdentifier;
- (RomIdentifier) charRomIdentifier;
- (RomIdentifier) vc1541RomIdentifier;

- (NSString *) basicRomTitle;
- (NSString *) charRomTitle;
- (NSString *) kernalRomTitle;
- (NSString *) vc1541RomTitle;

- (NSString *) basicRomSubTitle;
- (NSString *) charRomSubTitle;
- (NSString *) kernalRomSubTitle;
- (NSString *) vc1541RomSubTitle;

- (NSString *) basicRomRevision;
- (NSString *) charRomRevision;
- (NSString *) kernalRomRevision;
- (NSString *) vc1541RomRevision;

- (BOOL) isCommodoreRom:(RomIdentifier)rev;
- (BOOL) isPatchedRom:(RomIdentifier)rev;

// Flashing files
- (BOOL)flash:(AnyFileProxy *)container;
- (BOOL)flash:(AnyCollectionProxy *)proxy item:(NSInteger)nr;

@end


//
// Guards (Breakpoints, Watchpoints)
//

@interface GuardsProxy : NSObject {
    
    struct GuardsWrapper *wrapper;
}

@property (readonly) NSInteger count;
- (NSInteger) addr:(NSInteger)nr;
- (BOOL) isEnabled:(NSInteger)nr;
- (BOOL) isDisabled:(NSInteger)nr;
- (void) enable:(NSInteger)nr;
- (void) disable:(NSInteger)nr;
- (void) remove:(NSInteger)nr;
- (void) replace:(NSInteger)nr addr:(NSInteger)addr;

- (BOOL) isSetAt:(NSInteger)addr;
- (BOOL) isSetAndEnabledAt:(NSInteger)addr;
- (BOOL) isSetAndDisabledAt:(NSInteger)addr;
- (void) enableAt:(NSInteger)addr;
- (void) disableAt:(NSInteger)addr;
- (void) addAt:(NSInteger)addr;
- (void) removeAt:(NSInteger)addr;

@end


//
// CPU proxy
//

@interface CPUProxy : NSObject {
    
    struct CpuWrapper *wrapper;
}

- (CPUInfo) getInfo;
- (NSInteger) loggedInstructions;
- (NSInteger) loggedPCRel:(NSInteger)nr;
- (NSInteger) loggedPCAbs:(NSInteger)nr;
- (void) clearLog;
- (void) dump;
- (bool) isJammed;

- (void) setHex;
- (void) setDec;

- (i64) cycle;
- (u16) pc;

- (NSString *) disassembleRecordedInstr:(NSInteger)i length:(NSInteger *)len;
- (NSString *) disassembleRecordedBytes:(NSInteger)i;
- (NSString *) disassembleRecordedFlags:(NSInteger)i;
- (NSString *) disassembleRecordedPC:(NSInteger)i;

- (NSString *) disassembleInstr:(NSInteger)addr length:(NSInteger *)len;
- (NSString *) disassembleBytes:(NSInteger)addr;
- (NSString *) disassembleAddr:(NSInteger)addr;

@end

//
// Memory proxy
//

@interface MemoryProxy : NSObject {
    
    struct MemoryWrapper *wrapper;
}

- (MemInfo) getInfo;

- (void) dump;

- (MemoryType) peekSource:(u16)addr;
- (MemoryType) pokeTarget:(u16)addr;

- (u8) spypeek:(u16)addr source:(MemoryType)source;
- (u8) spypeek:(u16)addr;
- (u8) spypeekIO:(u16)addr;
- (u8) spypeekColor:(u16)addr;

- (void) poke:(u16)addr value:(u8)value target:(MemoryType)target;
- (void) poke:(u16)addr value:(u8)value;
- (void) pokeIO:(u16)addr value:(u8)value;

- (NSString *)memdump:(NSInteger)addr num:(NSInteger)num hex:(BOOL)hex src:(MemoryType)src;
- (NSString *)txtdump:(NSInteger)addr num:(NSInteger)num src:(MemoryType)src;

@end


//
// CIA proxy
//

@interface CIAProxy : NSObject {
    
    struct CiaWrapper *wrapper;
}

- (CIAInfo) getInfo;
- (void) dump;
- (void) poke:(u16)addr value:(u8)value;

@end


//
// VICII proxy
//


@interface VICProxy : NSObject {
    
	struct VicWrapper *wrapper;
}

// - (NSInteger) videoPalette;
// - (void) setVideoPalette:(NSInteger)value;
- (BOOL) isPAL;

- (void *) stableEmuTexture;
- (NSColor *) color:(NSInteger)nr;
- (UInt32) rgbaColor:(NSInteger)nr palette:(Palette)palette;
- (double)brightness;
- (void)setBrightness:(double)value;
- (double)contrast;
- (void)setContrast:(double)value;
- (double)saturation;
- (void)setSaturation:(double)value;

- (VICIIInfo) getInfo;
- (SpriteInfo) getSpriteInfo:(NSInteger)sprite;
- (void) dump;

/*
- (BOOL) hideSprites;
- (void) setHideSprites:(BOOL)b;
- (BOOL) showIrqLines;
- (void) setShowIrqLines:(BOOL)b;
- (BOOL) showDmaLines;
- (void) setShowDmaLines:(BOOL)b;
*/

- (u32 *) noise;

@end


//
// SID proxy
//

@interface SIDProxy : NSObject {
    
    struct SidBridgeWrapper *wrapper;
}

- (SIDInfo) getInfo:(NSInteger)nr;
- (VoiceInfo) getVoiceInfo:(NSInteger)nr voice:(NSInteger)voice;
- (void) dump;

- (double) sampleRate;
- (void) setSampleRate:(double)rate;

- (NSInteger) ringbufferSize;
- (void) ringbufferData:(NSInteger)offset left:(float *)l right:(float *)r;
- (double) fillLevel;
- (NSInteger) bufferUnderflows;
- (NSInteger) bufferOverflows;

- (void) copyMono:(float *)target size:(NSInteger)n;
- (void) copyStereo:(float *)target1 buffer2:(float *)target2 size:(NSInteger)n;
- (void) copyInterleaved:(float *)target size:(NSInteger)n;

- (void) rampUp;
- (void) rampUpFromZero;
- (void) rampDown;

@end


//
// Keyboard proxy
//

@interface KeyboardProxy : NSObject {
    
    struct KeyboardWrapper *wrapper;
}

- (void) dump;

- (BOOL) keyIsPressed:(NSInteger)nr;
- (BOOL) keyIsPressedAtRow:(NSInteger)row col:(NSInteger)col;
- (BOOL) controlIsPressed;
- (BOOL) commodoreIsPressed;
- (BOOL) leftShiftIsPressed;
- (BOOL) rightShiftIsPressed;
- (BOOL) shiftLockIsPressed;

- (void) pressKey:(NSInteger)nr;
- (void) pressKeyAtRow:(NSInteger)row col:(NSInteger)col;
- (void) pressShiftLock;

- (void) releaseKey:(NSInteger)nr;
- (void) releaseKeyAtRow:(NSInteger)row col:(NSInteger)col;
- (void) releaseShiftLock;
- (void) releaseAll;

- (void) toggleKey:(NSInteger)nr;
- (void) toggleKeyAtRow:(NSInteger)row col:(NSInteger)col;
- (void) toggleShiftLock;

- (void) scheduleKeyPress:(NSInteger)nr delay:(NSInteger)delay;
- (void) scheduleKeyPressAtRow:(NSInteger)row col:(NSInteger)col delay:(NSInteger)delay;
- (void) scheduleKeyRelease:(NSInteger)nr delay:(NSInteger)delay;
- (void) scheduleKeyReleaseAtRow:(NSInteger)row col:(NSInteger)col delay:(NSInteger)delay;

@end


//
// Control port proxy
//

@interface ControlPortProxy : NSObject {
    
    struct ControlPortWrapper *wrapper;
}

- (void) dump;

- (void) trigger:(GamePadAction)event;
- (BOOL) autofire;
- (void) setAutofire:(BOOL)value;
- (NSInteger) autofireBullets;
- (void) setAutofireBullets:(NSInteger)value;
- (float) autofireFrequency;
- (void) setAutofireFrequency:(float)value;

@end


//
// Expansion port proxy
//

@interface ExpansionPortProxy : NSObject {
    
    struct ExpansionPortWrapper *wrapper;
}

- (void) dump;

- (BOOL) cartridgeAttached;
- (CartridgeType) cartridgeType;
- (BOOL) attachCartridgeAndReset:(CRTFileProxy *)c;
- (void) attachGeoRamCartridge:(NSInteger)capacity;
- (void) attachIsepicCartridge;
- (void) detachCartridgeAndReset;

- (NSInteger) numButtons;
- (NSString *) getButtonTitle:(NSInteger)nr;
- (void) pressButton:(NSInteger)nr;
- (void) releaseButton:(NSInteger)nr;

- (BOOL) hasSwitch;
- (NSInteger) switchPosition;
- (NSString *) switchDescription:(NSInteger)pos;
- (NSString *) currentSwitchDescription;
- (BOOL) validSwitchPosition:(NSInteger)pos;
- (BOOL) switchIsNeutral;
- (BOOL) switchIsLeft;
- (BOOL) switchIsRight;
- (void) setSwitchPosition:(NSInteger)pos;

- (BOOL) hasLed;
- (BOOL) led;
- (void) setLed:(BOOL)value;

- (BOOL) hasBattery;
- (void) setBattery:(BOOL)value;

@end


//
// IEC bus proxy
//

@interface IECProxy : NSObject {
    
    struct IecWrapper *wrapper;
}

- (void) dump;
- (BOOL) busy;

@end


//
// Drive proxy
//

@interface DriveProxy : NSObject {
    
    struct DriveWrapper *wrapper;
    
    // Sub proxys
    VIAProxy *via1;
    VIAProxy *via2;
    DiskProxy *disk;
}

@property (readonly) struct DriveWrapper *wrapper;
@property (readonly) VIAProxy *via1;
@property (readonly) VIAProxy *via2;
@property (readonly) DiskProxy *disk;

- (VIAProxy *) via:(NSInteger)num;

- (DriveConfig) getConfig;

- (void) dump;

- (BOOL) isConnected;
- (BOOL) isSwitchedOn;

- (BOOL) readMode;
- (BOOL) writeMode;

- (BOOL) redLED;
- (BOOL) hasDisk;
- (BOOL) hasModifiedDisk;
- (void) setModifiedDisk:(BOOL)b;
- (void) insertNewDisk:(DOSType)fstype;
- (void) insertD64:(D64FileProxy *)disk;
- (void) insertFileSystem:(FSDeviceProxy *)proxy;
- (void) insertCollection:(AnyCollectionProxy *)disk;
- (void) ejectDisk;
- (BOOL) writeProtected;
- (void) setWriteProtection:(BOOL)b;
- (BOOL) hasWriteProtectedDisk;

- (Track) track;
- (Halftrack) halftrack;
- (u16) sizeOfHalftrack:(Halftrack)ht;
- (u16) sizeOfCurrentHalftrack;
- (u16) offset;
- (u8) readBitFromHead;

- (BOOL) isRotating;

@end


//
// VIA proxy
//

@interface VIAProxy : NSObject {
    
    struct ViaWrapper *wrapper;
}

- (void) dump;

@end


//
// Disk proxy
//

@interface DiskProxy : NSObject {
    
    struct DiskWrapper *wrapper;
}

@property (readonly) struct DiskWrapper *wrapper;

- (void) dump;
- (BOOL)writeProtected;
- (void)setWriteProtection:(BOOL)b;
- (void)toggleWriteProtection;
- (NSInteger)nonemptyHalftracks;
- (void)analyzeTrack:(Track)t;
- (void)analyzeHalftrack:(Halftrack)ht;
- (NSInteger)numErrors;
- (NSString *)errorMessage:(NSInteger)nr;
- (NSInteger)firstErroneousBit:(NSInteger)nr;
- (NSInteger)lastErroneousBit:(NSInteger)nr;
- (SectorInfo)sectorInfo:(Sector)s;
- (const char *)diskNameAsString;
- (const char *)trackBitsAsString;
- (const char *)sectorHeaderBytesAsString:(Sector)nr hex:(BOOL)hex;
- (const char *)sectorDataBytesAsString:(Sector)nr hex:(BOOL)hex;
@end

//
// Datasette proxy
//

@interface DatasetteProxy : NSObject {
    
    struct DatasetteWrapper *wrapper;
}

- (void) dump;

- (BOOL) hasTape;

- (void) pressPlay;
- (void) pressStop;
- (void) rewind;
- (void) ejectTape;
- (BOOL) insertTape:(TAPFileProxy *)tape;
- (NSInteger) getType; 
- (long) durationInCycles;
- (int) durationInSeconds;
- (NSInteger) head;
- (NSInteger) headInCycles;
- (int) headInSeconds;
- (void) setHeadInCycles:(long)value;
- (BOOL) motor;
- (BOOL) playKey;

@end

//
// Mouse proxy
//

@interface MouseProxy : NSObject {
    
    struct MouseWrapper *wrapper;
}

- (MouseModel) model;
- (void) setModel:(MouseModel)model;
- (NSInteger) port;
- (void) connect:(NSInteger)toPort;
- (void) disconnect;
- (void) setXY:(NSPoint)pos;
- (void) setLeftButton:(BOOL)pressed;
- (void) setRightButton:(BOOL)pressed;

@end

//
// F I L E   T Y P E   P R O X Y S
//

@protocol Makeable

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;

@end

//
// AnyFile proxy
//

@interface AnyFileProxy : NSObject {
    
    struct AnyFileWrapper *wrapper;
}

- (struct AnyFileWrapper *)wrapper;

- (FileType)type;
- (void)setPath:(NSString *)path;
- (NSString *)name;
- (NSInteger)sizeOnDisk;
- (u64) fnv;
- (NSInteger)writeToFile:(NSString *)path error:(ErrorCode *)err;

@end

//
// AnyCollection proxy
//

@interface AnyCollectionProxy : AnyFileProxy {
}

- (NSInteger)itemSize:(NSInteger)nr;

@end

//
// Snapshot proxy
//

@interface SnapshotProxy : AnyFileProxy <Makeable> {

   NSImage *preview;
}

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype)makeWithC64:(C64Proxy *)c64proxy;

@property (readonly, strong) NSImage *previewImage;
@property (readonly) time_t timeStamp;
@property (readonly, copy) NSData *data;

@end

//
// CRTFile proxy
//

@interface CRTFileProxy : AnyFileProxy <Makeable> {
}

+ (BOOL)isCRTFile:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
 
@property (readonly) CartridgeType cartridgeType;
@property (readonly) BOOL isSupported;
@property (readonly) NSInteger initialExromLine;
@property (readonly) NSInteger initialGameLine;
@property (readonly) NSInteger chipCount;

@end

//
// TAPFile proxy
//

@interface TAPFileProxy : AnyFileProxy <Makeable> {
}

+ (BOOL)isTAPFile:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;

@property (readonly) TAPVersion version;

@end

//
// T64File proxy
//

@interface T64FileProxy : AnyCollectionProxy <Makeable> {
}
+ (BOOL)isT64File:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype)makeWithFileSystem:(FSDeviceProxy *)proxy;

@end

//
// PRGFile proxy
//

@interface PRGFileProxy : AnyCollectionProxy <Makeable> {
}
+ (BOOL)isPRGFile:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype)makeWithFileSystem:(FSDeviceProxy *)proxy;

@end

//
// Folder proxy
//

@interface FolderProxy : AnyCollectionProxy {
}

+ (instancetype)makeWithFolder:(NSString *)path error:(ErrorCode *)err;

@property (readonly) FSDeviceProxy *fileSystem;

@end

//
// P00File proxy
//

@interface P00FileProxy : AnyCollectionProxy <Makeable> {
}
+ (BOOL)isP00File:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype)makeWithFileSystem:(FSDeviceProxy *)proxy;

@end

//
// AnyDisk proxy
//

@interface AnyDiskProxy : AnyCollectionProxy {
}

+ (instancetype)make;

@property (readonly) NSInteger numTracks;
@property (readonly) NSInteger numHalftracks;
- (void)selectHalftrack:(NSInteger)ht;
- (NSInteger)sizeOfHalftrack;
- (void)seekHalftrack:(NSInteger)offset;
- (NSString *)readHalftrackHex:(NSInteger)num;

@end

//
// D64File proxy
//

@interface D64FileProxy : AnyFileProxy <Makeable> {
}
+ (BOOL)isD64File:(NSString *)path;
+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype)makeWithDisk:(DiskProxy *)proxy;
+ (instancetype)makeWithDrive:(DriveProxy *)proxy;
+ (instancetype)makeWithVolume:(FSDeviceProxy *)proxy error:(FSError *)error;

@property (readonly) NSInteger numTracks;
@property (readonly) NSInteger numHalftracks;

@end

//
// G64File proxy
//

@interface G64FileProxy : AnyDiskProxy <Makeable> {
}
+ (BOOL)isG64File:(NSString *)path;
+ (instancetype) makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype) makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
+ (instancetype) makeWithDisk:(DiskProxy *)diskProxy;

@end

//
// FSDevice
//

@interface FSDeviceProxy : NSObject {
    
    struct FSDeviceWrapper *wrapper;
}

+ (instancetype)makeWithD64:(D64FileProxy *)proxy;
+ (instancetype)makeWithDisk:(DiskProxy *)proxy;
+ (instancetype)makeWithCollection:(AnyCollectionProxy *)proxy;

@property (readonly) struct FSDeviceWrapper *wrapper;
@property (readonly) DOSType dos;
@property (readonly) NSInteger numCyls;
@property (readonly) NSInteger numHeads;
@property (readonly) NSInteger numTracks;
- (NSInteger)numSectors:(NSInteger)track;
@property (readonly) NSInteger numBlocks;

@property (readonly) NSInteger numFreeBlocks;
@property (readonly) NSInteger numUsedBlocks;
@property (readonly) NSInteger numFiles;

- (NSInteger)cylNr:(NSInteger)t;
- (NSInteger)headNr:(NSInteger)t;
- (NSInteger)trackNr:(NSInteger)c head:(NSInteger)h;
- (TSLink)tsLink:(NSInteger)b;
- (NSInteger)trackNr:(NSInteger)b;
- (NSInteger)sectorNr:(NSInteger)b;
- (NSInteger)blockNr:(TSLink)ts;
- (NSInteger)blockNr:(NSInteger)t sector:(NSInteger)s;
- (NSInteger)blockNr:(NSInteger)c head:(NSInteger)h sector:(NSInteger)s;

- (FSBlockType)blockType:(NSInteger)blockNr;
- (FSUsage)itemType:(NSInteger)blockNr pos:(NSInteger)pos;
- (FSErrorReport)check:(BOOL)strict;
- (FSError)check:(NSInteger)nr pos:(NSInteger)pos expected:(unsigned char *)exp strict:(BOOL)strict;
- (BOOL)isCorrupted:(NSInteger)blockNr;
- (NSInteger)getCorrupted:(NSInteger)blockNr;
- (NSInteger)nextCorrupted:(NSInteger)blockNr;
- (NSInteger)prevCorrupted:(NSInteger)blockNr;
- (void)printDirectory;

- (NSInteger)readByte:(NSInteger)block offset:(NSInteger)offset;
- (BOOL)exportDirectory:(NSString *)path error:(FSError *)err;

- (void)dump;
- (void)info;

- (NSString *)fileName:(NSInteger)nr;
- (FSFileType)fileType:(NSInteger)nr;
- (NSInteger)fileSize:(NSInteger)nr;
- (NSInteger)fileBlocks:(NSInteger)nr;

@end
