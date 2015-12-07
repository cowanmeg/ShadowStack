#include "cpu/pred/TestMemDevice.hh"
#include "mem/packet.hh"
#include "mem/request.hh"
#include "debug/Ras.hh"

#define STARTADDR 512000000

TestMemDevice::TestMemDevice(const Params *p) 
	: MemObject(p) {
	port = new ShadowStackPort(name() + ".port", this);
	overflowPaddr = STARTADDR; // End of DRAM addr range
	busy = false; // Don't know if this is necessary
}

TestMemDevice::~TestMemDevice() {
	delete port;
}

BaseMasterPort& 
TestMemDevice::getMasterPort(const std::string &if_name, PortID idx) {
	if (if_name == "port")
		return *port;
	return MemObject::getMasterPort(if_name, idx);
}

bool
TestMemDevice::recvTimingResp(PacketPtr pkt) {
	// TODO: FIX temp
	if (pkt->hasData()) { // Read response
		ReturnAddrStack::RASEntry data;
		pkt->writeDataToBlock((uint8_t*) &data, PCSTATE_SIZE);

		DPRINTF(Ras, "Read back data from overflow stack %s count %d\n", data.addr, data.count);
		RAS->restoreAddr(data);
	}

	delete pkt;
	busy = false;
	return true;
}

bool 
TestMemDevice::isConnected() {
	return port->isConnected();
}

bool
TestMemDevice::writeReq(uint8_t *data) { 
	//create a request packet
	// TODO incrememnt overflowPaddr
	if (busy)
		return false;
	Request::Flags flags;
	flags.set(Request::UNCACHEABLE);
  	Request *req = new Request(overflowPaddr, PCSTATE_SIZE, flags, 0);

  	Packet *pkt = new Packet(req, MemCmd::WriteReq);
	pkt->dataStatic(data);

	//DPRINTF(Ras, "Writing data to overflow stack %s\n", addr);
	port->sendTimingReq(pkt);
	busy = true;
	overflowPaddr += PCSTATE_SIZE*8;
	return true;
} 

bool
TestMemDevice::readReq() {
	if (busy)
		return false;
	
	overflowPaddr -= PCSTATE_SIZE*8;
	Request::Flags flags;
	flags.set(Request::UNCACHEABLE);
  	Request *req = new Request(overflowPaddr, PCSTATE_SIZE, flags, 0);

	Packet *pkt = new Packet(req, MemCmd::ReadReq);
	uint8_t *newData = new uint8_t[PCSTATE_SIZE];
	pkt->dataDynamic(newData);

	port->sendTimingReq(pkt);
	busy = true;
	return true;
}

void 
TestMemDevice::resetAddr(unsigned numEntries) {
	overflowPaddr = STARTADDR + PCSTATE_SIZE * numEntries;
}

TestMemDevice*
TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}

