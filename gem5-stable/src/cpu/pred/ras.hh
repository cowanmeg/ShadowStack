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

#ifndef __CPU_PRED_RAS_HH__
#define __CPU_PRED_RAS_HH__

#include <vector>

#include "arch/types.hh"
#include "base/types.hh"
#include "config/the_isa.hh"
#include "cpu/pred/TestMemDevice.hh"
#include "rc4.h"
#include "base/types.hh"
#include "debug/Ras.hh"


/** Return address stack class, implements a simple RAS. */
class ReturnAddrStack
{
  public:
    /** Creates a return address stack, but init() must be called prior to
     *  use.
     */
    ReturnAddrStack() {}

    /** Initializes RAS with a specified number of entries.
     *  @param numEntries Number of entries in the RAS.
     */
    void init(unsigned numEntries);

    /* Initializes RAS with TestMemDevice used for overflow */
    void assignPort(TestMemDevice *dev);

    void reset();

    /** Returns the top address on the RAS. */
    TheISA::PCState top()
    { return addrStack[tos]; }

    /** Returns the index of the top of the RAS. */
    unsigned topIdx()
    { return tos; }

    /** Returns the index of the bottom of the RAS. */
    unsigned bottomIdx()
    { return bos; }

    /** Pushes an address onto the RAS. */
    void push(const TheISA::PCState &return_addr);

    /** Pops the top address from the RAS. */
    void pop(bool ignoreValue=false);

    /** Changes index to the top of the RAS, and replaces the top address with
     *  a new target.
     *  @param top_entry_idx The index of the RAS that will now be the top.
     *  @param restored The new target address of the new top of the RAS.
     */
    void restore(unsigned top_entry_idx, unsigned bottom_entry_idx, const TheISA::PCState &restored);

     bool empty() { return usedEntries == 0; }

     bool full() { return usedEntries == numEntries; 
		 }

    /** Checks if the RAS is about to underflow and if so sends a request
    to restore entries */
    void checkUnderflow();

    /** Checks if the RAS is about to overflow and if so sends out to 
    bottom most address to the overflow stack*/
    void checkOverflow();

    /** Writes back a return address returned from overflow stack
    back into the RAS*/
    void restoreAddr(const TheISA::PCState &return_addr);

    void print();
  private:
    /** Increments the top of stack index. */
    inline void incrTos()
    { 
        if (++tos == numEntries) tos = 0;
    }

    /** Decrements the top of stack index. */
    inline void decrTos()
    { 
      tos = (usedEntries == numEntries ? numEntries - 1 : tos - 1); 

    }
    /** Increments the bottom of stack index. */
    inline void incrBos()
    { 
        if (++bos == numEntries) bos = 0;
    }

    /** Decrements the bottom of stack index. */
    inline void decrBos()
    { bos = (bos == 0 ? numEntries - 1 : bos - 1); 
    }

    /** The RAS itself. */
    std::vector<TheISA::PCState> addrStack;

    /** The number of entries in the RAS. */
    unsigned numEntries;

    /** The number of used entries in the RAS. */
    unsigned usedEntries;

    /** The number of used entries in the overflow. */
    unsigned overflowEntries;
 
    /** The top of stack index. */
    unsigned tos;
    /** The bottom of stack index. */
    unsigned bos;

    /** Pointer to device that handles overflow stack*/
    TestMemDevice *dev;
    RC4 rc;
};

#endif // __CPU_PRED_RAS_HH__
