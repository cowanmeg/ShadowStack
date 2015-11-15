#include "cpu/pred/TestMemDevice.hh"

TestMemDevice::TestMemDevice(const Params *p) 
	: MemObject(p) {
	port = new ShadowStackPort(name() + "-port", this);
	std::cout << name() << std::endl;
    connected();
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


TestMemDevice*
 TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}
