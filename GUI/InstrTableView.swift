// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

class InstrTableView: NSTableView {
    
    @IBOutlet weak var inspector: Inspector!
    var c64: C64Proxy { return inspector.parent.c64 }
    var cpu: CPUProxy { return c64.cpu }
    
    // var instructionAtRow: [Int: DisassembledInstruction] = [:]
    // var rowForAddress: [UInt16: Int] = [:]
    
    // Data caches
    var addrInRow: [Int: Int] = [:]
    var instrInRow: [Int: String] = [:]
    var dataInRow: [Int: String] = [:]
    // var bpInRow: [Int: BreakpointType] = [:]
    var rowForAddr: [Int: Int] = [:]
    
    var hex = true
    
    override func awakeFromNib() {
        
        delegate = self
        dataSource = self
        target = self
        doubleAction = #selector(doubleClickAction(_:))
    }
    
    private func cache() {
        
        if let addr = addrInRow[0] {
            cache(startAddr: addr)
        }
    }
    
    private func cache(startAddr: Int) {
        
        var addr = startAddr
        
        instrInRow = [:]
        addrInRow = [:]
        dataInRow = [:]
        // bpInRow = [:]
        rowForAddr = [:]
        
        for i in 0 ..< Int(CPUINFO_INSTR_COUNT) where addr <= 0xFFFF {
            
            var info = c64.cpu.disassemble(UInt16(addr), hex: hex)
            
            instrInRow[i] = String(cString: &info.command.0)
            addrInRow[i] = addr
            dataInRow[i] = String(cString: &info.data.0)
            /*
             if c64.cpu.breakpointIsSetAndDisabled(at: addr) {
             bpInRow[i] = BreakpointType.disabled
             } else if c64.cpu.breakpointIsSet(at: addr) {
             bpInRow[i] = BreakpointType.enabled
             } else {
             bpInRow[i] = BreakpointType.none
             }
             */
            rowForAddr[addr] = i
            addr += Int(info.size)
        }
    }
    
    func refresh(count: Int = 0, full: Bool = false, addr: Int = 0) {
        
        if full {
            for (c, f) in ["addr": fmt16] {
                let columnId = NSUserInterfaceItemIdentifier(rawValue: c)
                if let column = tableColumn(withIdentifier: columnId) {
                    if let cell = column.dataCell as? NSCell {
                        cell.formatter = f
                    }
                }
            }
            
            cache()
            reloadData()
        }
        
        if count != 0 {
            jumpTo(addr: addr)
        }
    }
    
    func jumpTo(addr: Int) {
        
        if let row = rowForAddr[addr] {
            
            // If the requested address is already displayed, we simply select
            // the corresponding row.
            reloadData()
            jumpTo(row: row)
            
        } else {
            
            // If the requested address is not displayed, we update the data
            // cache and display the address in row 0.
            cache(startAddr: addr)
            reloadData()
            jumpTo(row: 0)
        }
    }
    
    func jumpTo(row: Int) {
        
        scrollRowToVisible(row)
        selectRowIndexes([row], byExtendingSelection: false)
    }
    
    @IBAction func clickAction(_ sender: NSTableView!) {
        
        if sender.clickedColumn == 0 {
            
            clickAction(row: sender.clickedRow)
        }
    }
    
    func clickAction(row: Int) {
        
        /*
        if let addr = addrInRow[row] {
            
            if !cpu.breakpointIsSet(at: addr) {
                cpu.addBreakpoint(at: addr)
            } else if cpu.breakpointIsSetAndDisabled(at: addr) {
                cpu.breakpointSetEnable(at: addr, value: true)
            } else if cpu.breakpointIsSetAndEnabled(at: addr) {
                cpu.breakpointSetEnable(at: addr, value: false)
            }
            
            inspector.fullRefresh()
        }
        */
    }
    
    @IBAction func doubleClickAction(_ sender: NSTableView!) {
        
        if sender.clickedColumn != 0 {
            
            doubleClickAction(row: sender.clickedRow)
        }
    }
    
    func doubleClickAction(row: Int) {
        
        /*
        if let addr = addrInRow[row] {
            
            if cpu.breakpointIsSet(at: addr) {
                cpu.removeBreakpoint(at: addr)
            } else {
                cpu.addBreakpoint(at: addr)
            }
            
            inspector.fullRefresh()
        }
        */
    }
    
    func setHex(_ value: Bool) {
        
        hex = value
        refresh(full: true)
    }
}

extension InstrTableView: NSTableViewDataSource {
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return 256
    }
    
    func tableView(_ tableView: NSTableView, objectValueFor tableColumn: NSTableColumn?, row: Int) -> Any? {
        
        switch tableColumn?.identifier.rawValue {
            
            /*
             case "break" where bpInRow[row] == .enabled:
             return "\u{26D4}" // "⛔" ("\u{1F534}" // "🔴")
             case "break" where bpInRow[row] == .disabled:
             return "\u{26AA}" // "⚪" ("\u{2B55}" // "⭕")
             */
        case "addr":
            return addrInRow[row]
        case "data":
            return dataInRow[row]
        case "instr":
            return instrInRow[row]
        default:
            return ""
        }
    }
}

extension InstrTableView: NSTableViewDelegate {
    
    func tableView(_ tableView: NSTableView, willDisplayCell cell: Any, for tableColumn: NSTableColumn?, row: Int) {
        
        /*
        let cell = cell as? NSTextFieldCell
        
        if bpInRow[row] == .enabled {
            cell?.textColor = NSColor.systemRed
        } else if bpInRow[row] == .disabled {
            cell?.textColor = NSColor.disabledControlTextColor
        } else {
            cell?.textColor = NSColor.labelColor
        }
        */
    }
}
