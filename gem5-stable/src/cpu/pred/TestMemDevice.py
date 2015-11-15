from m5.params import *
from m5.proxy import *
from MemObject import MemObject

class TestMemDevice(MemObject):
	type = 'TestMemDevice'
	cxx_header = "cpu/pred/TestMemDevice.hh"
	# TestMemDevice contains a master port
	port = MasterPort('Master port to Memory')
	#system = Param.System(Parent.any, "System we belong to")