from m5.params import *
from MemObject import MemObject

class TestMemDevice(MemObject):
	type = 'TestMemDevice'
	cxx_header = "cpu/pred/TestMemDevice.hh"
	# TestMemDevice contains a master port
	port = MasterPort('Master port to Memory')