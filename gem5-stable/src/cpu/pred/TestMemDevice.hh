#ifndef __TEST_HH__
#define __TEST_HH__

class ReturnAddrStack;

#include "mem/mem_object.hh"
#include "params/TestMemDevice.hh"
#include "cpu/pred/ras.hh"

class TestMemDevice : public MemObject {
protected:
    class ShadowStackPort : public MasterPort
    {
      public:

		ShadowStackPort(const std::string &_name, TestMemDevice *_owner)
		: MasterPort(_name, _owner), owner(_owner)
		{ }

      protected:

		virtual bool recvTimingResp(PacketPtr pkt) {
			return owner->recvTimingResp(pkt);
		}
		virtual void recvTimingSnoopReq(PacketPtr pkt) { }

		virtual void recvReqRetry() { }

		TestMemDevice *owner;
    };

    /** Instance of master port, facing the memory side */
    ShadowStackPort *port; 

    bool recvTimingResp(PacketPtr pkt);

public:
	// Convenience typedef.
	typedef TestMemDeviceParams Params;

	TestMemDevice(const Params *p);
	~TestMemDevice();

	void assignRas(ReturnAddrStack *_RAS) {
		RAS = _RAS;
	}

	bool isConnected();
 	bool writeReq(TheISA::PCState addr);
	bool readReq();	

	virtual BaseMasterPort& getMasterPort(const std::string &if_name, 
		PortID idx = InvalidPortID);

private:
	Addr overflowPaddr;
	bool busy;
	ReturnAddrStack *RAS;

};

#endif //__TEST_HH__
