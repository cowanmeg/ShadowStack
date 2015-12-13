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
#include "debug/TMD.hh"

void
ReturnAddrStack::init(unsigned _numEntries)
{
     DPRINTF(Ras, "Initializing RAS or size %d\n", _numEntries);
     numEntries  = _numEntries;
     addrStack.resize(numEntries);
     reset();
     stalled = false;
     stalledWrite = false;
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
    stalled = false;
    stalledWrite = false;
    usedEntries = 0;
    tos = 0;
    bos = 0;
    for (unsigned i = 0; i < numEntries; ++i)
        addrStack[i] = (RASEntry{0,0});
}

bool
ReturnAddrStack::push(const TheISA::PCState &return_addr)
{
    checkOverflowStack();
    if (return_addr == addrStack[tos].addr) {
       addrStack[tos].count++;
       DPRINTF(Ras, "RAS pushed same value %s, incremented count to %d\n", return_addr, addrStack[tos].count);
    } else {
	
        incrTos();
        addrStack[tos] = RASEntry{return_addr, 1};
        if (usedEntries != numEntries) {
            ++usedEntries;
        } else {
          std::cout << "RAS overflowed :(\n";
        }
    }

    DPRINTF(Ras, "RAS pushed %s bos=%d, tos=%d, usedEntries=%d\n", return_addr, bos, tos, usedEntries);

    if (usedEntries == numEntries -1) {
      DPRINTF(Ras, "RAS about to overflow\n");
      std::cout << "Stall overflow\n";
      stalledWrite = true;
      return false;
    }
    return true;
}

bool
ReturnAddrStack::pop(bool ignoreValue)
{
    uint8_t count = addrStack[tos].count;
    TheISA::PCState popped_addr = addrStack[tos].addr;
    if (count > 1) {
        addrStack[tos].count--;
    } else if (addrStack[tos].count < 2 && usedEntries > 0) {
        --usedEntries;
        decrTos();
    } /*else if (ignoreValue && overflowEntries > 0) { // Cheating - when popping off from 
      // a bad prediction if it goes into the overflow just get rid of it.
      overflowEntries--;
      bos--;
      dev->resetAddr(overflowEntries);
      DPRINTF(Ras, "Trying to pop but RAS empty - pop overflow bos=%d tos=%d entries=%d overflow=%d\n",
                bos, tos, usedEntries, overflowEntries);
    } */else {
      std::cout << "RAS shouldn't reach here!!!\n";
    }
    DPRINTF(Ras, "RAS popped %s count=%d, bos=%d, tos=%d, usedEntries=%d overflowEntries=%d\n", 
      popped_addr, count, bos, tos, usedEntries, overflowEntries);

    checkOverflowStack();

    if (usedEntries <=  2 && overflowEntries > 0) {
      // premetively stall
      std::cout << "Stall underflow\n";
      
      stalled = true;
      DPRINTF(Ras, "Suspend the thread to wait for RAS to fill back up\n");
      return false;
    }
    return true;
}

void
ReturnAddrStack::restore(unsigned top_entry_idx, unsigned bottom_entry_idx,
                         const RASEntry &restored)
{
    tos = top_entry_idx;
    //bos = bottom_entry_idx;
    //dev->resetAddr(bos);
    usedEntries = (tos >= bos) ? tos-bos : numEntries-bos+tos;
    addrStack[tos] = restored;
    
    DPRINTF(Ras, "RAS restored %s count=%d, bos=%d, tos=%d, usedEntries=%d\n", restored.addr,restored.count, bos, tos, usedEntries);
}

void
ReturnAddrStack::print() {
    for (uint64_t i = numEntries-1; i >= 1; i--)
        DPRINTF(Ras, "\ttos=%d, %s count %d\n", i, addrStack[i].addr, addrStack[i].count);
    DPRINTF(Ras, "\ttos=0, %s count %d\n", addrStack[0].addr, addrStack[0].count);

}

void
ReturnAddrStack::checkOverflowStack() {
  
  if ( (overflowEntries > 0) && (usedEntries <= (numEntries * 1/4)) ) {
      DPRINTF(TMD, "RAS triggered underflow - restore entries\n");
      dev->readReq();
  } else if (usedEntries >= (numEntries * 3/4)) {
      // Write the bottom entry to the overflow stack
      unsigned bosIndex = bos+1;
      if (bos+1 == numEntries)
        bosIndex = 0;
      RASEntry entry = addrStack[bosIndex];
      //TheISA::PCState address=entry.addr;
      //uint64_t encryptedAddr=rc.encrypt64(address.pc());
      //printf("checkOverflow: before encrypted = %lx after encrypted = %lx\n",address.pc(),encryptedAddr);
      //entry.addr.pc(encryptedAddr);
      //std::cout << "size: " << sizeof(entry) << std::endl;
      uint8_t *data = new uint8_t[PCSTATE_SIZE];
      std::memcpy(data, &entry, PCSTATE_SIZE);
    if (dev->writeReq(data)) {
      // Update RAS sate
      incrBos();
      usedEntries--;
      overflowEntries++;

      DPRINTF(TMD, "RAS wrote %s bos=%d, usedEntries=%d overflowEntries=%d\n", entry.addr, 
      bos, usedEntries, overflowEntries);
      print();

      if (stalledWrite) {
        stalledWrite = false;
        tc->activate();
        DPRINTF(Ras, "Wake up thread Overflow \n");
        std::cout << "Waking up thread\n";
      }
    } 
  }
}

void
ReturnAddrStack::restoreAddr(const RASEntry &return_addr) {
  if (overflowEntries > 0) {
    RASEntry return_address = (RASEntry) return_addr;
    //uint64_t decryptedAddr = rc.encrypt64(return_address.addr.pc());
  //  printf("restoreAddr: before decrypted = %lx after decrypted = %lx\n",address.pc(),decryptedAddr);   
    //return_address.addr.pc(decryptedAddr);
    addrStack[bos] = return_address;
    decrBos();

    usedEntries++;
    overflowEntries--;

    if (stalled && (usedEntries >= 2 || overflowEntries == 0)) {
        // wake up thread
        std::cout << "Waking up thread\n";
        stalled = false;
        tc->activate();
        DPRINTF(Ras, "Wake up thread! Underflow \n");
    }
    DPRINTF(TMD, "Ras returned %s to bos=%d usedEntries=%d overflowEntries=%d\n", return_addr.addr, 
        bos, usedEntries, overflowEntries);
  } 
  checkOverflowStack();

}

bool
ReturnAddrStack::unroll(const TheISA::PCState &corrTarget) {
  /* Match the pc corrTarget with the next pc of entries in the RAS */
  DPRINTF(Ras, "Unrolling the RAS to find %s\n", corrTarget);
  uint64_t temp_usedEntries = usedEntries;
  for (int i = tos; i > bos; i--) {
    temp_usedEntries--;
    if (corrTarget.pc() == addrStack[i].addr.npc()) {
      tos = i-1;
      usedEntries = temp_usedEntries;
      DPRINTF(Ras, "Found a match - Resetting the RAS\n");
      print();
      return true;
    }
  }
  DPRINTF(Ras, "RAS actually has an incorrect value\n");
  return false;
  //std::cout << "No address match! Security Attack!\n";
  //printf("Correct Target %lx->%lx\n", ((uint64_t) corrTarget.pc()), ((uint64_t) corrTarget.npc())); 
}
