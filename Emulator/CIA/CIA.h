// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "CIATypes.h"
#include "SubComponent.h"
#include "TOD.h"

class CIA : public SubComponent {
    
    friend class TOD;
    friend class ParCable;

    //
    // Action flags
    //
    
    // Decrements timer A
    static constexpr u64 CIACountA0 =   (1ULL << 0);
    static constexpr u64 CIACountA1 =   (1ULL << 1);
    static constexpr u64 CIACountA2 =   (1ULL << 2);
    static constexpr u64 CIACountA3 =   (1ULL << 3);
    
    // Decrements timer B
    static constexpr u64 CIACountB0 =   (1ULL << 4);
    static constexpr u64 CIACountB1 =   (1ULL << 5);
    static constexpr u64 CIACountB2 =   (1ULL << 6);
    static constexpr u64 CIACountB3 =   (1ULL << 7);
    
    // Loads timer A
    static constexpr u64 CIALoadA0 =    (1ULL << 8);
    static constexpr u64 CIALoadA1 =    (1ULL << 9);
    static constexpr u64 CIALoadA2 =    (1ULL << 10);
    
    // Loads timer B
    static constexpr u64 CIALoadB0 =    (1ULL << 11);
    static constexpr u64 CIALoadB1 =    (1ULL << 12);
    static constexpr u64 CIALoadB2 =    (1ULL << 13);
    
    // Sets pin PB6 low
    static constexpr u64 CIAPB6Low0 =   (1ULL << 14);
    static constexpr u64 CIAPB6Low1 =   (1ULL << 15);
    
    // Sets pin PB7 low
    static constexpr u64 CIAPB7Low0 =   (1ULL << 16);
    static constexpr u64 CIAPB7Low1 =   (1ULL << 17);
    
    // Triggers an interrupt
    static constexpr u64 CIASetInt0 =   (1ULL << 18);
    static constexpr u64 CIASetInt1 =   (1ULL << 19);
    
    // Releases the interrupt line
    static constexpr u64 CIAClearInt0 = (1ULL << 20);
    static constexpr u64 CIAOneShotA0 = (1ULL << 21);
    static constexpr u64 CIAOneShotB0 = (1ULL << 22);
    
    // Indicates that ICR was read recently
    static constexpr u64 CIAReadIcr0 =  (1ULL << 23);
    static constexpr u64 CIAReadIcr1 =  (1ULL << 24);
    
    // Clears bit 8 in ICR register
    static constexpr u64 CIAClearIcr0 = (1ULL << 25);
    static constexpr u64 CIAClearIcr1 = (1ULL << 26);
    static constexpr u64 CIAClearIcr2 = (1ULL << 27);
    
    // Clears bit 0 - 7 in ICR register
    static constexpr u64 CIAAckIcr0 =   (1ULL << 28);
    static constexpr u64 CIAAckIcr1 =   (1ULL << 29);
    
    // Sets bit 8 in ICR register
    static constexpr u64 CIASetIcr0 =   (1ULL << 30);
    static constexpr u64 CIASetIcr1 =   (1ULL << 31);
    
    // Triggers an IRQ with TOD as source
    static constexpr u64 CIATODInt0 =   (1ULL << 32);
    
    // Triggers an IRQ with serial reg as source
    static constexpr u64 CIASerInt0 =   (1ULL << 33);
    static constexpr u64 CIASerInt1 =   (1ULL << 34);
    static constexpr u64 CIASerInt2 =   (1ULL << 35);
    
    // Loads the serial shift register
    static constexpr u64 CIASerLoad0 =  (1ULL << 36);
    static constexpr u64 CIASerLoad1 =  (1ULL << 37);
    
    // Clock signal driving the serial register
    static constexpr u64 CIASerClk0 =   (1ULL << 38);
    static constexpr u64 CIASerClk1 =   (1ULL << 39);
    static constexpr u64 CIASerClk2 =   (1ULL << 40);
    static constexpr u64 CIASerClk3 =   (1ULL << 41);
    
    static constexpr u64 CIALast =      (1ULL << 42);
    
    /* vAmiga:
     static constexpr u64 CIASdrToSsr0 = (1ULL << 36); // Move serial data reg to serial shift reg
     static constexpr u64 CIASdrToSsr1 = (1ULL << 37);
     static constexpr u64 CIASsrToSdr0 = (1ULL << 38); // Move serial shift reg to serial data reg
     static constexpr u64 CIASsrToSdr1 = (1ULL << 39);
     static constexpr u64 CIASsrToSdr2 = (1ULL << 40);
     static constexpr u64 CIASsrToSdr3 = (1ULL << 41);
     static constexpr u64 CIASerClk0 =   (1ULL << 42); // Clock signal driving the serial register
     static constexpr u64 CIASerClk1 =   (1ULL << 43);
     static constexpr u64 CIASerClk2 =   (1ULL << 44);
     static constexpr u64 CIASerClk3 =   (1ULL << 45);
     static constexpr u64 CIALast =      (1ULL << 46);
     */
    
    static constexpr u64 CIADelayMask = ~CIALast
    & ~CIACountA0 & ~CIACountB0 & ~CIALoadA0 & ~CIALoadB0 & ~CIAPB6Low0
    & ~CIAPB7Low0 & ~CIASetInt0 & ~CIAClearInt0 & ~CIAOneShotA0 & ~CIAOneShotB0
    & ~CIAReadIcr0 & ~CIAClearIcr0 & ~CIAAckIcr0 & ~CIASetIcr0 & ~CIATODInt0
    & ~CIASerInt0 & ~CIASerLoad0 & ~CIASerClk0;
            
    // Current configuration
    CIAConfig config = { };
    
    // Result of the latest inspection
    mutable CIAInfo info = { };
    
    
    //
    // Sub components
    //
    
public:
    
    TOD tod = TOD(c64, *this);
    
    
    //
    // Internals
    //
        
protected:
    
    // Timer A counter
    u16 counterA;
    
    // Timer B counter
    u16 counterB;
        
    // Timer A latch
    u16 latchA;
    
    // Timer B latch
    u16 latchB;
	    
    		
    //
	// Control
    //
    
    // Action flags
	u64 delay;
	u64 feed;
    
    // Control registers
	u8 CRA;
    u8 CRB;
    
    // Interrupt control register
	u8 icr;

    // ICR bits to be deleted when CIAAckIcr1 hits
    u8 icrAck;

    // Interrupt mask register
	u8 imr;

protected:
    
    // Bit mask for PB outputs (0 = port register, 1 = timer)
    u8 PB67TimerMode;
    
    // PB outputs bits 6 and 7 in timer mode
	u8 PB67TimerOut;
    
    // PB outputs bits 6 and 7 in toggle mode
	u8 PB67Toggle;
		
    
    //
    // Port registers
    //
    
protected:
    
    // Peripheral data registers
    u8 PRA;
    u8 PRB;
    
    // Data directon registers
    u8 DDRA;
    u8 DDRB;
    
    // Peripheral ports
    u8 PA;
    u8 PB;
	
    
    //
    // Shift register logic
    //
    
private:
    
    /* Serial data register
     * http://unusedino.de/ec64/technical/misc/cia6526/serial.html
     * "The serial port is a buffered, 8-bit synchronous shift register system.
     *  A control bit selects input or output mode. In input mode, data on the
     *  SP pin is shifted into the shift register on the rising edge of the
     *  signal applied to the CNT pin. After 8 CNT pulses, the data in the shift
     *  register is dumped into the Serial Data Register and an interrupt is
     *  generated. In the output mode, TIMER A is used for the baud rate
     *  generator. Data is shifted out on the SP pin at 1/2 the underflow rate
     *  of TIMER A. [...] Transmission will start following a write to the
     *  Serial Data Register (provided TIMER A is running and in continuous
     *  mode). The clock signal derived from TIMER A appears as an output on the
     *  CNT pin. The data in the Serial Data Register will be loaded into the
     *  shift register then shift out to the SP pin when a CNT pulse occurs.
     *  Data shifted out becomes valid on the falling edge of CNT and remains
     *  valid until the next falling edge. After 8 CNT pulses, an interrupt is
     *  generated to indicate more data can be sent. If the Serial Data Register
     *  was loaded with new information prior to this interrupt, the new data
     *  will automatically be loaded into the shift register and transmission
     *  will continue. If the microprocessor stays one byte ahead of the shift
     *  register, transmission will be continuous. If no further data is to be
     *  transmitted, after the 8th CNT pulse, CNT will return high and SP will
     *  remain at the level of the last data bit transmitted. SDR data is
     *  shifted out MSB first and serial input data should also appear in this
     *  format.
     */
    u8 sdr;
    
    // Clock signal for driving the serial register
    bool serClk;
    
    /* Shift register counter
     * The counter is set to 8 when the shift register is loaded and decremented
     * when a bit is shifted out.
     */
    u8 serCounter;
    
    //
	// Port pins
    //
        
    bool CNT;
	bool INT;

    
    //
    // TOD control
    //
    
    // Cycle nextTodTrigger;
    
    
    //
    // Sleep logic
    //
    
    /* Idle counter. When the CIA's state does not change during execution,
     * this variable is increased by one. If it exceeds a certain threshhold,
     * the chip is put into idle state via sleep().
     */
    u8 tiredness;

    // Total number of skipped cycles (used by the debugger, only)
    Cycle idleCycles;
        
public:
    
    // Indicates if the CIA is currently idle
    bool sleeping;
    
    /* The last executed cycle before the chip went idle
     * The variable is set in sleep()
     */
    Cycle sleepCycle;
    
    /* The first cycle to be executed after the chip went idle
     * The variable is set in sleep()
     */
    Cycle wakeUpCycle;
    
    
    //
    // Initializing
    //
    
public:
    
	CIA(C64 &ref);
    virtual bool isCIA1() const = 0;
    virtual bool isCIA2() const = 0;

    
    //
    // Methods from C64Object
    //

private:
    
    void _dump(dump::Category category, std::ostream& os) const override;

    
    //
    // Methods from C64Component
    //

protected:
    
    void _reset(bool hard) override;
    void _inspect() const override;
        
private:
    
    template <class T>
    void applyToPersistentItems(T& worker)
    {
        worker
        
        << config.revision
        << config.timerBBug;
    }
    
    template <class T>
    void applyToResetItems(T& worker, bool hard = true)
    {
        worker
        
        << counterA
        << counterB
        << latchA
        << latchB
        << delay
        << feed
        << CRA
        << CRB
        << icr
        << icrAck
        << imr
        << PB67TimerMode
        << PB67TimerOut
        << PB67Toggle
        << PRA
        << PRB
        << DDRA
        << DDRB
        << PA
        << PB
        << sdr
        << serClk
        << serCounter
        << CNT
        << INT
        << tiredness
        << idleCycles
        << sleeping
        << sleepCycle
        << wakeUpCycle;
    }
    
    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }
    
    
    //
    // Analyzing
    //

public:
    
    CIAInfo getInfo() const { return C64Component::getInfo(info); }

    
    //
    // Configuring
    //
    
public:
    
    static CIAConfig getDefaultConfig();
    const CIAConfig &getConfig() const { return config; }
    void resetConfig() override;

    i64 getConfigItem(Option option) const;
    void setConfigItem(Option option, i64 value);

        
    //
    // Accessing the I/O register space
    //
       
public:
    
    // Reads a value from a CIA register
    u8 peek(u16 addr);
    
    // Reads a value from a CIA register without causing side effects
    u8 spypeek(u16 addr) const;

    // Writes a value into a CIA register
    void poke(u16 addr, u8 value);
    
    
    //
    // Accessing the port registers
    //
    
public:
    
    // Returns the data registers (call updatePA() or updatePB() first)
    u8 getPA() const { return PA; }
    u8 getPB() const { return PB; }

private:
    
    // Returns the data direction register
    u8 getDDRA() const { return DDRA; }
    u8 getDDRB() const { return DDRB; }
    
    // Computes the value we currently see at port A
    virtual void updatePA() = 0;
    virtual u8 computePA() const = 0;
    
    // Returns the value driving port A from inside the chip
    virtual u8 portAinternal() const = 0;
    
    // Returns the value driving port A from outside the chip
    virtual u8 portAexternal() const = 0;
    
    // Computes the value we currently see at port B
    virtual void updatePB() = 0;
    virtual u8 computePB() const = 0;
    
    // Returns the value driving port B from inside the chip
    virtual u8 portBinternal() const = 0;
    
    // Returns the value  driving port B from outside the chip
    virtual u8 portBexternal() const = 0;
    
protected:

    // Action method for peeking the port registers
    virtual u8 peekPA() { updatePA(); return PA; }
    virtual u8 peekPB() { updatePB(); return PB; }

    // Action method for poking the port registers
    virtual void pokePRA(u8 value) { PRA = value; updatePA(); }
    virtual void pokePRB(u8 value) { PRB = value; updatePB(); }

    // Action method for poking the port direction registers
    virtual void pokeDDRA(u8 value) { DDRA = value; updatePA(); }
    virtual void pokeDDRB(u8 value) { DDRB = value; updatePB(); }

    
    //
    // Accessing the port pins
    //
    
public:
    
    // Simulates an edge on the flag pin
    void triggerRisingEdgeOnFlagPin();
    void triggerFallingEdgeOnFlagPin();
    
    // Emulates a pulse on the PC pin
    virtual void pulsePC() { };

    
    //
    // Handling interrupts
    //
    
private:

    // Requests the CPU to interrupt
    virtual void pullDownInterruptLine() = 0;
    
    // Removes the interrupt requests
    virtual void releaseInterruptLine() = 0;
    
    // Loads a latched value into timer
    void reloadTimerA(u64 *delay);
    void reloadTimerB(u64 *delay);
    
    // Triggers an interrupt (invoked inside executeOneCycle())
    void triggerTimerIrq(u64 *delay);
    void triggerTodIrq(u64 *delay);
    void triggerSerialIrq(u64 *delay);
    
public:
    
    // Handles an interrupt request from TOD
    void todInterrupt();
    
 
    //
    // Executing
    //
    
public:
    
	// Executes the CIA for one cycle
	void executeOneCycle();
        
    
    //
    // Speeding up (sleep logic)
    //
    
private:
    
    // Puts the CIA into idle state
    void sleep();
    
public:
        
    // Emulates all previously skipped cycles
    void wakeUp();
    void wakeUp(Cycle targetCycle);
    
    // Returns true if the CIA is in idle state
    bool isSleeping() const { return sleeping; }
    
    // Returns true if the CIA is awake
    bool isAwake() const { return !sleeping; }
    
    // The CIA is idle since this number of cycles
    Cycle idleSince() const;
    
    // Total number of cycles the CIA was idle
    Cycle idleTotal() const { return idleCycles; }
};


//
// CIA1
//

class CIA1 : public CIA {
	
public:

    CIA1(C64 &ref) : CIA(ref) { };
    bool isCIA1() const override { return true; }
    bool isCIA2() const override { return false; }
    const char *getDescription() const override { return "CIA1"; }
    
private:
        
    void pullDownInterruptLine() override;
    void releaseInterruptLine() override;
    
    u8 portAinternal() const override;
    u8 portAexternal() const override;
    void updatePA() override;
    u8 computePA() const override;
    
    u8 portBinternal() const override;
    u8 portBexternal() const override;
    void updatePB() override;
    u8 computePB() const override;
};
	

//
// CIA2
//

class CIA2 : public CIA {

    friend class ParCable;
    
public:

    CIA2(C64 &ref) : CIA(ref) { };
    bool isCIA1() const override { return false; }
    bool isCIA2() const override { return true; }
    const char *getDescription() const override { return "CIA2"; }

private:
        
    void pullDownInterruptLine() override;
    void releaseInterruptLine() override;
    
    u8 portAinternal() const override;
    u8 portAexternal() const override;
    
public:
    
    void updatePA() override;
    u8 computePA() const override;
    
private:
    
    u8 portBinternal() const override;
    u8 portBexternal() const override;
    void updatePB() override;
    u8 computePB() const override;
    void pokePRA(u8 value) override;
    void pokePRB(u8 value) override;
    void pokeDDRA(u8 value) override;
    void pulsePC() override;
};
