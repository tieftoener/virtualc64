/*
 * Author: Dirk W. Hoffmann, 2011 - 2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#import "C64GUI.h"
#import "VirtualC64-Swift.h"

@implementation MyController {
}

@synthesize c64;

@synthesize propertiesDialog;
@synthesize hardwareDialog;
@synthesize mediaDialog;
@synthesize mountDialog;
@synthesize tapeDialog;

// Debugger
@synthesize debugPanel;
@synthesize stopAndGoButton;
@synthesize stepIntoButton;
@synthesize stepOverButton;
@synthesize hex;

// Debugger (CPU panel)
@synthesize pc;
@synthesize sp;
@synthesize a;
@synthesize x;
@synthesize y;
@synthesize Nflag;
@synthesize Zflag;
@synthesize Cflag;
@synthesize Iflag;
@synthesize Bflag;
@synthesize Dflag;
@synthesize Vflag;
@synthesize breakAt;

// Debugger (CIA panel)
@synthesize ciaSelector;

@synthesize ciaPA;
@synthesize ciaPAbinary;
@synthesize ciaPRA;
@synthesize ciaDDRA;

@synthesize ciaPB;
@synthesize ciaPBbinary;
@synthesize ciaPRB;
@synthesize ciaDDRB;

@synthesize ciaTimerA;
@synthesize ciaLatchA;
@synthesize ciaRunningA;
@synthesize ciaToggleA;
@synthesize ciaPBoutA;
@synthesize ciaOneShotA;

@synthesize ciaTimerB;
@synthesize ciaLatchB;
@synthesize ciaRunningB;
@synthesize ciaToggleB;
@synthesize ciaPBoutB;
@synthesize ciaOneShotB;

@synthesize todHours;
@synthesize todMinutes;
@synthesize todSeconds;
@synthesize todTenth;
@synthesize todIntEnable;
@synthesize alarmHours;
@synthesize alarmMinutes;
@synthesize alarmSeconds;
@synthesize alarmTenth;

@synthesize ciaIcr;
@synthesize ciaIcrBinary;
@synthesize ciaImr;
@synthesize ciaImrBinary;
@synthesize ciaIntLineLow;

// Debugger (VIC)
@synthesize VicDX;
@synthesize VicDY;
@synthesize VicSpriteX1;
@synthesize VicSpriteY1;
@synthesize VicSpriteX2;
@synthesize VicSpriteY2;
@synthesize VicSpriteX3;
@synthesize VicSpriteY3;
@synthesize VicSpriteX4;
@synthesize VicSpriteY4;
@synthesize VicSpriteX5;
@synthesize VicSpriteY5;
@synthesize VicSpriteX6;
@synthesize VicSpriteY6;
@synthesize VicSpriteX7;
@synthesize VicSpriteY7;
@synthesize VicSpriteX8;
@synthesize VicSpriteY8;
@synthesize VicRasterline;
@synthesize VicRasterInterrupt;

// Debugger (SID)
@synthesize selectedVoice;
@synthesize volume;
@synthesize potX;
@synthesize potY;

@synthesize voiceSelector;
@synthesize waveform;
@synthesize frequency;
@synthesize pulseWidth;
@synthesize attackRate;
@synthesize decayRate;
@synthesize sustainRate;
@synthesize releaseRate;
@synthesize gateBit;
@synthesize testBit;
@synthesize syncBit;
@synthesize ringBit;

@synthesize filterType;
@synthesize filterCutoff;
@synthesize filterResonance;
@synthesize filter1;
@synthesize filter2;
@synthesize filter3;

@synthesize waveformView;
@synthesize audioBufferLevel;
@synthesize audioBufferLevelText;
@synthesize bufferUnderflows;
@synthesize bufferOverflows;

// Toolbar
@synthesize controlPort1;
@synthesize controlPort2;

// Main screen
@synthesize debugger;

// Bottom bar
@synthesize greenLED;
@synthesize redLED;
@synthesize progress;
@synthesize driveIcon;
@synthesize cartridgeIcon;
@synthesize tapeIcon;
@synthesize tapeProgress;
@synthesize clockSpeed;
@synthesize clockSpeedBar;
@synthesize warpIcon;

@synthesize menuItemFinalIII;
@synthesize gamePadManager;
@synthesize modifierFlags;
@synthesize mouseXY;
@synthesize hideMouse;
@synthesize statusBar;
@synthesize gamepadSlot1;
@synthesize gamepadSlot2;
@synthesize genericDeviceImage;
@synthesize autoMount;

@synthesize keyboardcontroller;
@synthesize metalScreen;
@synthesize cpuTableView;
@synthesize memTableView;
@synthesize speedometer;
@synthesize animationCounter;

@synthesize timer;
@synthesize timerLock;

// --------------------------------------------------------------------------------
//          Refresh methods: Force all GUI items to refresh their value
// --------------------------------------------------------------------------------

/*
- (void)refresh:(NSFormatter *)byteFormatter word:(NSFormatter *)wordFormatter threedigit:(NSFormatter *)threeDigitFormatter
{		
	NSControl *ByteFormatterControls[] = { 
		// CPU panel
		sp, a, x, y,
		// CIA panel
        ciaPA, ciaPB,
		todHours, todMinutes, todSeconds, todTenth,
        alarmHours, alarmMinutes, alarmSeconds, alarmTenth,
        ciaImr, ciaIcr,
		// VIC panel
		VicSpriteY1, VicSpriteY2, VicSpriteY3, VicSpriteY4, VicSpriteY5, VicSpriteY6, VicSpriteY7, VicSpriteY8,
        // SID panel
        attackRate, decayRate, sustainRate, releaseRate,
        filterResonance,
        volume, potX, potY,
 		NULL };
	
	NSControl *WordFormatterControls[] = { 
		// CPU panel
		pc, breakAt,
		// CIA panel
		ciaTimerA, ciaLatchA, ciaTimerB, ciaLatchB,
		// VIC panel
		VicRasterline, VicRasterInterrupt,
        // SID panel
        frequency, pulseWidth, filterCutoff,
		NULL };

    NSControl *threeDigitFormatterControls[] = { 
		// VIC panel
		VicSpriteX1, VicSpriteX2, VicSpriteX3, VicSpriteX4, VicSpriteX5, VicSpriteX6, VicSpriteX7, VicSpriteX8,
		NULL };

	// Bind formatters
	for (int i = 0; ByteFormatterControls[i] != NULL; i++) {
		[ByteFormatterControls[i] abortEditing];
		[ByteFormatterControls[i] setFormatter:byteFormatter];
		[ByteFormatterControls[i] setNeedsDisplay];
	}
	
	for (int i = 0; WordFormatterControls[i] != NULL; i++) {
		[WordFormatterControls[i] abortEditing];
		[WordFormatterControls[i] setFormatter:wordFormatter];
		[WordFormatterControls[i] setNeedsDisplay];
	}

    for (int i = 0; threeDigitFormatterControls[i] != NULL; i++) {
		[threeDigitFormatterControls[i] abortEditing];
		[threeDigitFormatterControls[i] setFormatter:threeDigitFormatter];
		[threeDigitFormatterControls[i] setNeedsDisplay];
	}
    
	[[[memTableView tableColumnWithIdentifier:@"addr"] dataCell] setFormatter:wordFormatter];
	[[[memTableView tableColumnWithIdentifier:@"hex0"] dataCell] setFormatter:byteFormatter];
	[[[memTableView tableColumnWithIdentifier:@"hex1"] dataCell] setFormatter:byteFormatter];
	[[[memTableView tableColumnWithIdentifier:@"hex2"] dataCell] setFormatter:byteFormatter];
	[[[memTableView tableColumnWithIdentifier:@"hex3"] dataCell] setFormatter:byteFormatter];	
	
	[self refresh];
}

- (void)enableUserEditing:(BOOL)enabled
{
	NSControl *controls[] = {
        
		// CPU panel
		pc, sp, a, x, y, 
		Nflag, Zflag, Cflag, Iflag, Bflag, Dflag, Vflag,
		
        // SID panel
        frequency, pulseWidth, attackRate, decayRate, sustainRate, releaseRate,
        
		// VIC panel
		VicSpriteX1, VicSpriteX2, VicSpriteX3, VicSpriteX4, VicSpriteX5, VicSpriteX6, VicSpriteX7, VicSpriteX8,
        VicSpriteY1, VicSpriteY2, VicSpriteY3, VicSpriteY4, VicSpriteY5, VicSpriteY6, VicSpriteY7, VicSpriteY8,
        
        VicRasterline, VicRasterInterrupt, VicDX, VicDY,
		NULL };
	
	// Enable / disable controls
	for (int i = 0;; i++) {
		if (controls[i] == NULL) break;
		[controls[i] setEnabled:enabled];
	}
	
	// Enable / disable table columns
	[[memTableView tableColumnWithIdentifier:@"hex0"] setEditable:enabled];
	[[memTableView tableColumnWithIdentifier:@"hex1"] setEditable:enabled];
	[[memTableView tableColumnWithIdentifier:@"hex2"] setEditable:enabled];
	[[memTableView tableColumnWithIdentifier:@"hex3"] setEditable:enabled];
	
	// Change image and state of debugger control buttons
	if (![c64 isRunnable]) {
		[stopAndGoButton setImage:[NSImage imageNamed:@"play32"]];		
		[stopAndGoButton setEnabled:false];
		[stepIntoButton setEnabled:false];
		[stepOverButton setEnabled:false];
		
	} else if ([c64 isHalted]) {
		[stopAndGoButton setImage:[NSImage imageNamed:@"play32"]];		
		[stopAndGoButton setEnabled:true];
		[stepIntoButton setEnabled:true];
		[stepOverButton setEnabled:true];
	} else {
		[stopAndGoButton setImage:[NSImage imageNamed:@"pause32"]];
		[stopAndGoButton setEnabled:true];
		[stepIntoButton setEnabled:false];
		[stepOverButton setEnabled:false];		
	}		
}
*/

@end
