#include "cpu/pred/TestMemDevice.hh"
#include "mem/packet.hh"
#include "mem/request.hh"
#include "debug/Ras.hh"
#include "debug/TMD.hh"

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

void 
TestMemDevice::recvReqRetry() {
    DPRINTF(TMD, "Received a retry\n");
    port->sendTimingReq(lastRequest);
    busy = false;
}

bool
TestMemDevice::recvTimingResp(PacketPtr pkt) {
	// TODO: FIX temp
	if (pkt->hasData()) { // Read response
		ReturnAddrStack::RASEntry data;
		pkt->writeDataToBlock((uint8_t*) &data, PCSTATE_SIZE);

		DPRINTF(TMD, "Read back data from overflow stack %s count %d\n", data.addr, data.count);
		RAS->restoreAddr(data);
		//busy = false;
	}

	delete pkt;
	
	return true;
}

bool 
TestMemDevice::isConnected() {
	return port->isConnected();
}

bool
TestMemDevice::writeReq(uint8_t *data) { 
	//create a request packet
	if (busy)
		return false;
	Request::Flags flags;
	flags.set(Request::PHYSICAL);
  	Request *req = new Request(overflowPaddr, PCSTATE_SIZE, flags, 0);

  	Packet *pkt = new Packet(req, MemCmd::WriteReq);
	pkt->dataStatic(data);

	if (port->sendTimingReq(pkt)) {
	    DPRINTF(TMD, "TestMemDevice: Write sent\n");
            overflowPaddr += PCSTATE_SIZE*8;
	    return true;
        }
        busy = true;
        lastRequest = pkt;
        return false;
} 

bool
TestMemDevice::readReq() {
	if (busy)
		return false;
	
	Request::Flags flags;
	flags.set(Request::PHYSICAL | Request::STRICT_ORDER);
  	Request *req = new Request(overflowPaddr - PCSTATE_SIZE*8, PCSTATE_SIZE, flags, 0);

	Packet *pkt = new Packet(req, MemCmd::ReadReq);
	uint8_t *newData = new uint8_t[PCSTATE_SIZE];
	pkt->dataDynamic(newData);

	if (port->sendTimingReq(pkt)) {
            overflowPaddr -= PCSTATE_SIZE * 8;
            DPRINTF(TMD, "TestMemDevice: Read sent\n");
            return true;
        }
	
        busy = true;
	lastRequest = pkt;
	return false;
}

void 
TestMemDevice::resetAddr(unsigned numEntries) {
	overflowPaddr = STARTADDR + PCSTATE_SIZE * 8 * numEntries;
}

TestMemDevice*
TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}

