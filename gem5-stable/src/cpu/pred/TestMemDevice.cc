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
	// TODO: handlei
	// temp
	uint8_t data;
	pkt->writeData(&data);
	std::cout << "Packet data " << data << std::endl;
	
	delete pkt;
	return false;
}

bool 
TestMemDevice::isConnected() {
	return port->isConnected();
}

void
TestMemDevice::sendReq() { 
//TestMemDevice::sendReq(TheISA::PCState data) {
	//create a request packet
	// TODO incrememnt overflowPaddr
  	Request *req = new Request();
  	req->setPaddr(overflowPaddr);

  	Packet *pkt = new Packet(req, MemCmd::WriteReq);
	// TEMP
	uint8_t *data = static_cast<uint8_t *>(malloc(sizeof(uint8_t)));
	*data = 27;
	pkt->dataStatic<uint8_t>(data);
	std::cout << "Sending write packet!\n";
	port->sendTimingReq(pkt);
} 

void
TestMemDevice::readReq() {
	Request *req = new Request();
	req->setPaddr(overflowPaddr);

	Packet *pkt = new Packet(req, MemCmd::ReadReq);
	std::cout << "Sendng read packet!\n";
	port->sendTimingReq(pkt);
}

TestMemDevice*
 TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}

