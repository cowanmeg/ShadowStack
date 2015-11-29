/*
 * Copyright (c) 2004-2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Kevin Lim
 */

#include "cpu/pred/ras.hh"
#include "debug/Ras.hh"

void
ReturnAddrStack::init(unsigned _numEntries)
{
     DPRINTF(Ras, "Initializing RAS or size %d\n", _numEntries);
     numEntries  = _numEntries;
     addrStack.resize(numEntries);
     reset();
}

void
ReturnAddrStack::assignPort(TestMemDevice *_dev)
{
     DPRINTF(Ras, "Adding test Memdevice\n");
     dev = _dev;
     dev->assignRas(this);
}

void
ReturnAddrStack::reset()
{
    usedEntries = 0;
    tos = 0;
    bos = 0;
    for (unsigned i = 0; i < numEntries; ++i)
        addrStack[i].set(0);
}

void
ReturnAddrStack::push(const TheISA::PCState &return_addr)
{
    incrTos();

    addrStack[tos] = return_addr;

    if (usedEntries != numEntries) {
        ++usedEntries;
    }
    DPRINTF(Ras, "RAS pushed %s bos=%d, tos=%d, usedEntries=%d\n", return_addr, bos, tos, usedEntries);
    
    checkOverflow();

}

void
ReturnAddrStack::pop(bool ignoreValue)
{
    TheISA::PCState popped_addr = addrStack[tos];

    if (usedEntries > 0) {
        --usedEntries;
        decrTos();
    } else if (ignoreValue && overflowEntries > 0) { // Cheating - when popping off from 
      // a bad prediction if it goes into the overflow just get rid of it.
      overflowEntries--;
      bos--;
      DPRINTF(Ras, "Trying to pop but RAS empty - pop overflow bos=%d tos=%d entries=%d overflow=%d\n",
                bos, tos, usedEntries, overflowEntries);
    } else {
      DPRINTF(Ras, "Trying to pop but RAS empty bos=%d tos=%d entries=%d overflow=%d\n",
                bos, tos, usedEntries, overflowEntries);
      print();
    }
    
    
    
    DPRINTF(Ras, "RAS popped %s, bos=%d, tos=%d, usedEntries=%d\n", popped_addr, bos, tos, usedEntries);

    checkUnderflow();
}

void
ReturnAddrStack::restore(unsigned top_entry_idx, unsigned bottom_entry_idx,
                         const TheISA::PCState &restored)
{
    tos = top_entry_idx;
    bos = bottom_entry_idx;
    usedEntries = (tos > bos) ? tos-bos-1 : numEntries-bos+tos;
    addrStack[tos] = restored;
    
    DPRINTF(Ras, "RAS restored %s bos=%d, tos=%d, usedEntries=%d\n", restored, bos, tos, usedEntries);
}

void
ReturnAddrStack::print() {
    for (int i = tos; i>=0; i--)
        DPRINTF(Ras, "\ttos=%d, %s\n", i, addrStack[i]);
}

void 
ReturnAddrStack::checkOverflow() {
    if (tos > (numEntries * 3/4)) {
      // Write the bottom entry to the overflow stack
      TheISA::PCState data = addrStack[bos+1];
      if (dev->writeReq(data)) {
        // Update RAS sate
        incrBos();
        usedEntries--;
        overflowEntries++;

        DPRINTF(Ras, "RAS wrote %s bos=%d, usedEntries=%d overflowEntries=%d\n", data, 
            bos, usedEntries, overflowEntries);
      } 
    }
}

void 
ReturnAddrStack::checkUnderflow() {
    if ( (overflowEntries > 0) && (tos < (numEntries * 1/4)) ) {
      DPRINTF(Ras, "RAS triggered underflow - restore entries\n");
      dev->readReq();
    }

}

void
ReturnAddrStack::restoreAddr(const TheISA::PCState &return_addr) {
  addrStack[bos] = return_addr;
  decrBos();

  usedEntries++;
  overflowEntries--;

  DPRINTF(Ras, "Ras returned %s to bos=%d usedEntries=%d overflowEntries=%d\n", return_addr, 
      bos, usedEntries, overflowEntries);

}
