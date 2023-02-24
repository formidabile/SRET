from idaapi import *
from idautils import *
from idc import *
import string

cnt = 0
good=0
string_tmp=""
string_tmp2=""
string_candidates=list()
for segea in Segments():
	for funcea in Functions(segea, get_segm_end(segea)):
		for (startea, endea) in Chunks(funcea):
			for head in Heads(startea, endea):
				if is_code(get_flags(head)):
					if "mov" == print_insn_mnem(head) and print_operand(head, 1) == "0" and good >3:
						string_candidates.append(string_tmp)
						good = 0
						string_tmp=""
						cnt+=1
						if "mov" == print_insn_mnem(head) and idc.get_operand_type(head,1) == 5:
							string_tmp2 += print_operand(head, 1)[:-1]
							if string_tmp2.find('offset') == -1 and string_tmp2 != '' and len(string_tmp2) < 3:
								#print(string_tmp2)
								#string_tmp+=string_tmp2
								string_tmp+=chr(int(string_tmp2,base=16) ^ 0x22)
								good+=1
								string_tmp2=""
							else:
								good = 0
								string_tmp=""
						print("Found ", cnt," xored strings")
						print(string_candidates)