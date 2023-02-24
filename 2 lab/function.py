from idaapi import *
from idautils import *
from idc import *
import string

for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (startea, endea) in Chunks(funcea):
			for head in Heads(startea, endea):
				if is_code(get_flags(head)):
					if ('call' == print_insn_mnem(head)):
						a = print_operand(head, 0)
						#print(a)
						if a.find('guard_check_icall_fptr') > -1 or a.find('IsDebuggerPresent') > -1:
							print(hex(head)," : ",GetDisasm(head))