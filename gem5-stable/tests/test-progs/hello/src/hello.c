/*
 * Copyright (c) 2006 The Regents of The University of Michigan
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
 * Authors: Steve Reinhardt
 */

#include <stdio.h>

 void seventeen() {
  printf("seventeen\n");
} 

 void sixteen() {
  printf("Sixteen\n");
  seventeen();
} 

 void fifteen() {
  printf("Fifteen\n");
  sixteen();
} 

 void fourteen() {
  printf("Fourteen\n");
  fifteen();
} 

 void thirteen() {
  printf("Thirteen\n");
  fourteen();
} 

 void twelve() {
  printf("Twelve\n");
  thirteen();
} 

 void eleven() {
  printf("Eleven\n");
  twelve();
} 

 void ten() {
  printf("Ten\n");
  eleven();
} 

 void nine() {
  printf("Nine\n");
  ten();
} 


 void eight() {
  printf("Eight\n");
  nine();
} 

 void seven() {
  printf("Seven\n");
  eight();
} 

 void six() {
  printf("Six\n");
  seven();
} 

void five() {
  printf("Five\n");
  six();
} 

void four() {
  printf("Four\n");
  five();
} 

void three() {
  printf("Three\n");
  four();
} 

void two() {
  printf("Two\n");
  three();
} 

void one() {
  printf("One\n");
  two();
}

int main()
{
  one();
}

