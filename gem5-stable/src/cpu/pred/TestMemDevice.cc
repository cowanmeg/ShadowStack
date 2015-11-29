#include "cpu/pred/TestMemDevice.hh"
#include "mem/packet.hh"
#include "mem/request.hh"

TestMemDevice::TestMemDevice(const Params *p) 
	: MemObject(p) {
	port = new ShadowStackPort(name() + ".port", this);
	overflowPaddr = 512000000; // End of DRAM addr range
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
		uint8_t data;
		data = 0;
		pkt->writeData(&data);
		printf("Packet data %d\n", data);
	} else { // write response
		std::cout << "Packet has no data\n";
	}

	delete pkt;
	busy = false;
	return true;
}

bool 
TestMemDevice::isConnected() {
	return port->isConnected();
}

void
TestMemDevice::writeReq() { 
//TestMemDevice::sendReq(TheISA::PCState data) {
	//create a request packet
	// TODO incrememnt overflowPaddr
	if (busy)
		return;
	Request::Flags flags;
	flags.set(Request::UNCACHEABLE);
  	Request *req = new Request(overflowPaddr, 1, flags, 0);

  	Packet *pkt = new Packet(req, MemCmd::WriteReq);
	// TEMP
	uint8_t *data = new uint8_t[1];
	data[0] = 27;
	pkt->dataStatic(data);
	std::cout << "Writing packet with data 27\n";
	port->sendTimingReq(pkt);
	std::cout << "Sent write packet\n";
	busy = true;
} 

void
TestMemDevice::readReq() {
	if (busy)
		return;
	Request::Flags flags;
	flags.set(Request::UNCACHEABLE);
  	Request *req = new Request(overflowPaddr, 1, flags, 0);

	Packet *pkt = new Packet(req, MemCmd::ReadReq);
	uint8_t *newData = new uint8_t[1];
	pkt->dataDynamic(newData);
	std::cout << "Sendng read packet!\n";
	port->sendTimingReq(pkt);
	busy = true;
}

TestMemDevice*
TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}

