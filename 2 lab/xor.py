from idaapi import *
from idautils import *
from idc import *

for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (starters, endea) in Chunks(funcea):
			for head in Heads(starters, endea):
				if is_code(get_flags(head)):
					if GetDisasm(head).startswith("xor"):
						print(hex(head)," : ",GetDisasm(head))