// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "P00File.h"
#include "FSDevice.h"

/*
const u8
P00File::magicBytes[] = { 0x43, 0x36, 0x34, 0x46, 0x69, 0x6C, 0x65 };
*/

bool
P00File::isCompatibleName(const std::string &name)
{
    auto s = suffix(name);
    return s == "p00" || s == "P00";
}

bool
P00File::isCompatibleStream(std::istream &stream)
{
    const u8 magicBytes[] = { 0x43, 0x36, 0x34, 0x46, 0x69, 0x6C, 0x65 };

    if (streamLength(stream) < 0x1A) return false;
    return matchingStreamHeader(stream, magicBytes, sizeof(magicBytes));
}

P00File *
P00File::makeWithFileSystem(FSDevice *fs, int item)
{
    assert(fs);

    debug(FILE_DEBUG, "Creating P00 archive...\n");

    // Only proceed if the requested file exists
    if (fs->numFiles() <= (u64)item) return nullptr;
        
    // Create new archive
    usize fileSize = fs->fileSize(item);
    usize p00Size = fileSize + 8 + 17 + 1;
    P00File *p00 = new P00File(p00Size);
        
    debug(FILE_DEBUG, "File size = %zu\n", fileSize);
    
    // Write magic bytes (8 bytes)
    u8 *p = p00->data;
    strcpy((char *)p, "C64File");
    p += 8;
    
    // Write name in PET format (16 bytes)
    fs->fileName(item).write(p);
    p += 16;
    
    // Always 0 (1 byte)
    *p++ = 0;

    // Record size (applies to REL files, only) (1 byte)
    *p++ = 0;
        
    // Add data
    fs->copyFile(item, p, fileSize);
        
    return p00;
}
    
PETName<16>
P00File::getName()
{
    return PETName<16>(data + 8, 0x00);
}

PETName<16>
P00File::collectionName()
{
    return PETName<16>(data + 8, 0x00);
}

u64
P00File::collectionCount()
{
    return 1;
}

PETName<16>
P00File::itemName(unsigned nr)
{
    assert(nr == 0);
    u8 padChar = 0x00;
    return PETName<16>(data + 0x08, padChar);
}

u64
P00File::itemSize(unsigned nr)
{
    assert(nr == 0);
    return size - 0x1A;
}

u8
P00File::readByte(unsigned nr, u64 pos)
{
    assert(nr == 0);
    assert(pos < itemSize(nr));
    return data[0x1A + pos];
}
