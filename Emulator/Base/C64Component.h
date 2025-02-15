// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "C64ComponentTypes.h"
#include "C64Object.h"
#include "Serialization.h"
#include "Concurrency.h"

#include <vector>
#include <iostream>

#define synchronized \
for (util::AutoMutex _am(mutex); _am.active; _am.active = false)
    
class C64Component : public C64Object {
        
protected:
    
    // Sub components
    std::vector<C64Component *> subComponents;
            
    /* Mutex for implementing the 'synchronized' macro. The macro can be used
     * to prevent multiple threads to enter the same code block. It mimics the
     * behaviour of the well known Java construct 'synchronized(this) { }'.
     */
    mutable util::ReentrantMutex mutex;
    

    //
    // Initializing
    //
    
public:
    
    virtual ~C64Component() { };
    
    /* Initializes the component and it's subcomponents. The initialization
     * procedure is initiated once, in the constructor of the C64 class. By
     * default, a component enters it's initial configuration. Custom actions
     * can be performed by implementing the _initialize() delegation function.
     */
    void initialize();
    virtual void _initialize() { resetConfig(); }
    
    /* Resets the component and its subcomponent. It is mandatory for each
     * component to implement this function.
     */
    void reset(bool hard);
    virtual void _reset(bool hard) = 0;
    
    
    //
    // Configuring
    //
        
    // Initializes all configuration items with their default values
    virtual void resetConfig() { };


    //
    // Analyzing
    //
    
    /* Collects information about the component and it's subcomponents. Many
     * components contain an info variable of a class specific type (e.g.,
     * CPUInfo, MemoryInfo, ...). These variables contain the information shown
     * in the GUI's inspector window and are updated by calling this function.
     * Note: Because this function accesses the internal emulator state with
     * many non-atomic operations, it must not be called on a running emulator.
     * To carry out inspections while the emulator is running, set up an
     * inspection target via C64::setInspectionTarget().
     */
    void inspect() const;
    virtual void _inspect() const { }
    
    /* Base method for building the class specific getInfo() methods. When the
     * emulator is running, the result of the most recent inspection is
     * returned. If the emulator isn't running, the function first updates the
     * cached values in order to return up-to-date results.
     */
    template<class T> T getInfo(T &cachedValues) const {
        
        if (!isRunning()) inspect();
        
        synchronized { return cachedValues; }
        unreachable;
    }
        
    
    //
    // Serializing
    //
    
    // Returns the size of the internal state in bytes
    isize size();
    virtual isize _size() = 0;
    
    // Loads the internal state from a memory buffer
    isize load(const u8 *buf);
    virtual isize _load(const u8 *buf) = 0;
    
    // Saves the internal state to a memory buffer
    isize save(u8 *buf);
    virtual isize _save(u8 *buf) = 0;
    
    /* Delegation methods called inside load() or save(). Some components
     * override these methods to add custom behavior if not all elements can be
     * processed by the default implementation.
     */
    virtual isize willLoadFromBuffer(const u8 *buf) { return 0; }
    virtual isize didLoadFromBuffer(const u8 *buf) { return 0; }
    virtual isize willSaveToBuffer(const u8 *buf) {return 0; }
    virtual isize didSaveToBuffer(u8 *buf) { return 0; }
    
    
    //
    // Controlling the state (see Thread class for details)
    //
    
public:
    
    virtual bool isPoweredOff() const = 0;
    virtual bool isPoweredOn() const = 0;
    virtual bool isPaused() const = 0;
    virtual bool isRunning() const = 0;
    
    virtual void suspend() = 0;
    virtual void resume() = 0;

    // Throws an exception if the emulator is not ready to power on
    void isReady() const throws;

protected:
        
    void powerOn();
    void powerOff();
    void run();
    void pause();
    void halt();
    void warpOn();
    void warpOff();
    void debugOn();
    void debugOff();
    
    void powerOnOff(bool value) { value ? powerOn() : powerOff(); }
    void warpOnOff(bool value) { value ? warpOn() : warpOff(); }
    void debugOnOff(bool value) { value ? debugOn() : debugOff(); }

    virtual void _isReady() const throws { }
    virtual void _powerOn() { }
    virtual void _powerOff() { }
    virtual void _run() { }
    virtual void _pause() { }
    virtual void _halt() { }
    virtual void _warpOn() { }
    virtual void _warpOff() { }
    virtual void _debugOn() { }
    virtual void _debugOff() { }
};

//
// Standard implementations of _reset, _load, and _save
//

#define COMPUTE_SNAPSHOT_SIZE \
util::SerCounter counter; \
applyToPersistentItems(counter); \
applyToResetItems(counter); \
return counter.count;
    
#define RESET_SNAPSHOT_ITEMS(hard) \
util::SerResetter resetter; \
applyToResetItems(resetter, hard); \
debug(SNP_DEBUG, "Resetted (%s)\n", hard ? "hard" : "soft"); \
    
#define LOAD_SNAPSHOT_ITEMS \
util::SerReader reader(buffer); \
applyToPersistentItems(reader); \
applyToResetItems(reader); \
debug(SNP_DEBUG, "Recreated from %zu bytes\n", reader.ptr - buffer); \
return reader.ptr - buffer;
    
#define SAVE_SNAPSHOT_ITEMS \
util::SerWriter writer(buffer); \
applyToPersistentItems(writer); \
applyToResetItems(writer); \
debug(SNP_DEBUG, "Serialized to %zu bytes\n", writer.ptr - buffer); \
return writer.ptr - buffer;
