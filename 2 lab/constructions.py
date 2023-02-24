from idaapi import *
from idautils import *
from idc import *
import string

for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (startea, endea) in Chunks(funcea):
			for head in Heads(startea, endea):
				if is_code(get_flags(head)):
					if ('int' == print_insn_mnem(head)):
						a = print_operand(head, 0)[:-1]
						#print(a)
						if a == '2D':
							print(hex(head)," : ",GetDisasm(head))
						elif 'ss' == print_insn_mnem(head):
							print(hex(head)," : ",GetDisasm(head))