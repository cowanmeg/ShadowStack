#ifndef __TEST_HH__
#define __TEST_HH__

#include "mem/mem_object.hh"
#include "params/TestMemDevice.hh"

class TestMemDevice : public MemObject {
protected:
    class ShadowStackPort : public MasterPort
    {
      public:

		ShadowStackPort(const std::string &_name, TestMemDevice *_owner)
		: MasterPort(_name, _owner)
		{ }

      protected:

		virtual bool recvTimingResp(PacketPtr pkt) {return false;}
		virtual void recvTimingSnoopReq(PacketPtr pkt) { }

		virtual void recvReqRetry() { }
    };

    /** Instance of master port, facing the memory side */
    ShadowStackPort *port; 


public:
	// Convenience typedef.
	typedef TestMemDeviceParams Params;

	TestMemDevice(const Params *p);
	~TestMemDevice();

	virtual BaseMasterPort& getMasterPort(const std::string &if_name, 
		PortID idx = InvalidPortID);

};

#endif //__TEST_HH__