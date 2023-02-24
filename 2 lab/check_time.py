from idaapi import *
from idautils import *
from idc import *
import string

for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (startea, endea) in Chunks(funcea):
			for head in Heads(startea, endea):
				if is_code(get_flags(head)):
					if "rdtsc" in print_insn_mnem(head) or "GetTickCount" in print_insn_mnem(head):
						print(hex(head)," : ",GetDisasm(head))