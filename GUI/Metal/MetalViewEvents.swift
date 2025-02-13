// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

public extension MetalView {

    //
    // Keyboard events
    //
    
    override func keyDown(with event: NSEvent) {

        parent.keyboard.keyDown(with: event)
    }
    
    override func keyUp(with event: NSEvent) {

        parent.keyboard.keyUp(with: event)
    }
    
    override func flagsChanged(with event: NSEvent) {
        
        // Check for a mouse controlling key combination
        if parent.metal.checkForMouseKeys(with: event) { return }
        
        // Save modifier flags (needed by the TouchBar)
        parent.modifierFlags = event.modifierFlags
        
        parent.keyboard.flagsChanged(with: event)
    }
    
    //
    // Mouse events
    //
    
    func retainMouse() {
        
        NSCursor.hide()
        CGAssociateMouseAndMouseCursorPosition(boolean_t(truncating: false))
        gotMouse = true
    }
    
    func releaseMouse() {
        
        NSCursor.unhide()
        CGAssociateMouseAndMouseCursorPosition(boolean_t(truncating: true))
        gotMouse = false
    }
    
    override func updateTrackingAreas() {

        let options: NSTrackingArea.Options = [ .activeInKeyWindow, .mouseEnteredAndExited ]
        
        if trackingArea != nil {
            removeTrackingArea(trackingArea!)
        }
        trackingArea = NSTrackingArea(rect: self.bounds, options: options, owner: self, userInfo: nil)
        addTrackingArea(trackingArea!)
    }
    
    override func mouseEntered(with event: NSEvent) {
        
        insideTrackingArea = true
        
        // Check if we need to retain the mouse
        if prefs.retainMouseByEntering {
   
            // Only retain if the user didn't shake the mouse recently
            if DispatchTime.diffMilliSec(lastShake) > UInt64(500) {
                retainMouse()
            } else {
                track("Last shake too recent")
            }
        }
    }
    
    override func mouseExited(with event: NSEvent) {
        
        insideTrackingArea = false
        releaseMouse()
    }

    override func mouseDown(with event: NSEvent) {

        if !gotMouse {
            if prefs.retainMouseByClick && insideTrackingArea { retainMouse() }
            return
        }

        mouse!.processMouseEvents(events: [.PRESS_LEFT])
    }
    
    override func mouseUp(with event: NSEvent) {

        if !gotMouse { return }

        mouse!.processMouseEvents(events: [.RELEASE_LEFT])
    }

    override func rightMouseDown(with event: NSEvent) {

        if !gotMouse { return }
        
        mouse!.processMouseEvents(events: [.PRESS_RIGHT])
    }

    override func rightMouseUp(with event: NSEvent) {

        if !gotMouse { return }
        
        mouse!.processMouseEvents(events: [.RELEASE_RIGHT])
    }
        
    override func mouseMoved(with event: NSEvent) {
        
        if !gotMouse { return }
        
        let dx = event.deltaX
        let dy = -event.deltaY
        
        // Make coordinate independent of window size
        let scaleX = (256.0 * 400.0) / frame.width
        let scaleY = (256.0 * 300.0) / frame.height
        let dxdy = NSPoint(x: dx * scaleX, y: dy * scaleY)
                
        mouse?.processMouseEvents(delta: dxdy)
    }
    
    override func mouseDragged(with event: NSEvent) {

        mouseMoved(with: event)
    }
    
    override func rightMouseDragged(with event: NSEvent) {
        
        mouseMoved(with: event)
    }

    func checkForMouseKeys(with event: NSEvent) -> Bool {
                
        if !gotMouse && prefs.retainMouseWithKeys {
            
            switch prefs.retainMouseKeyComb {
              
            case 0 where event.modifierFlags.contains([.option, .command]),
                 1 where event.modifierFlags.contains([.option, .control]):
                
                track()
                retainMouse()
                return true
                
            default: break
            }

        } else if gotMouse && prefs.releaseMouseWithKeys {
            
            switch prefs.releaseMouseKeyComb {
                
            case 0 where event.modifierFlags.contains([.option, .command]),
                 1 where event.modifierFlags.contains([.option, .control]):
                
                track()
                releaseMouse()
                return true
                
            default: break
            }
        }
        return false
    }
}
