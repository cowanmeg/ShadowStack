#include "cpu/pred/TestMemDevice.hh"

TestMemDevice::TestMemDevice(const Params *p) 
	: MemObject(p) {
	port = new ShadowStackPort(name() + "-port", this);
    std::cout << "Created TestMemDevice\n";
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

TestMemDevice*
 TestMemDeviceParams::create() {
    return new TestMemDevice(this);
}
