#include "cpu/pred/TestMemDevice.hh"
#include "mem/packet.hh"
#include "mem/request.hh"

TestMemDevice::TestMemDevice(const Params *p) 
	: MemObject(p) {
	port = new ShadowStackPort(name() + ".port", this);
	overflowPaddr = 512000000; // End of DRAM addr range
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
	// TODO: handle
	return false;
}

bool 
TestMemDevice::isConnected() {
	return port->isConnected();
}

void 
TestMemDevice::sendReq(TheISA::PCState data) {
	//create a request packet
	// TODO incrememnt overflowPaddr
  	Request *req = new Request();
  	req->setPaddr(overflowPaddr)

  	Packet pkt = new Packet(req, MemCmd::WriteReq)
  	port->sendTimingReq(pkt);
}

TestMemDevice*
 TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}

