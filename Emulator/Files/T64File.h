// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "AnyCollection.h"

class T64File : public AnyCollection {
     
public:

    static bool isCompatiblePath(const std::string &name);
    static bool isCompatibleStream(std::istream &stream);
    
    static T64File *makeWithFileSystem(class FSDevice &fs);

    
    //
    // Initializing
    //
    
    T64File() : AnyCollection() { }
    T64File(usize capacity) : AnyCollection(capacity) { }

    
    //
    // Methods from C64Object
    //
    
    const char *getDescription() const override { return "T64File"; }

    
    //
    // Methods from AnyFile
    //
    
    FileType type() const override { return FILETYPE_T64; }
    PETName<16> getName() const override;
        

    //
    // Methods from AnyCollection
    //

    PETName<16> collectionName() override;
    isize collectionCount() const override;
    PETName<16> itemName(isize nr) const override;
    u64 itemSize(isize nr) const override;
    u8 readByte(isize nr, u64 pos) const override;
    
private:

    u16 memStart(isize nr) const;
    u16 memEnd(isize nr) const;
    
    
    //
    // Scanning and repairing
    //
    
public:
    
    // Checks if the header contains information at the specified location
    bool directoryItemIsPresent(isize n);

    /* This methods eliminates the following inconsistencies:
     *
     *   - number of files:
     *     Some archives state falsely in their header that zero files are
     *     present. This value will be fixed.
     *
     *   - end loading address:
     *     Archives that are created with CONVC64 often contain a value of
     *     0xC3C6, which is wrong (e.g., paradrd.t64). This value will be
     *     changed such that getByte() will read until the end of the physical
     *     file.
    */
    void repair() override;
};
