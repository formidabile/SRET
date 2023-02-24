from idaapi import *
from idautils import *
from idc import *
import string

for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (startea, endea) in Chunks(funcea):
			for head in Heads(startea, endea):
				if is_code(get_flags(head)):
					if "mov" in print_insn_mnem(head) and ("fs:30" in print_operand(head, 1) or "fs:[30" in print_operand(head, 1)):
						print(hex(head)," : ",GetDisasm(head))