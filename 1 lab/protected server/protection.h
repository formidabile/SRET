#ifndef PROTECTION
#define PROTECTION

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>

#define FLG_HEAP_ENABLE_TAIL_CHECK (0x10)
#define FLG_HEAP_ENABLE_FREE_CHECK (0x20)
#define FLG_HEAP_VALIDATE_PARAMETERS (0x40)



// checking password
void protection1(std::string pswd);
//generate random string
std::string protection2(unsigned len, std::string alphabet);
// IsDebuggerPresent
void protection3();
//CheckRemoteDebuggerPresent
void protection4();
//NtQueryProcessInformation
void protection5();
//TrapFlag
void protection6();
//anti VM LDT
void protection7();
//anti VM STR
void protection8();
// calculating CRC
DWORD protection9(PUCHAR funcBegin, size_t size);
// generate keygen
void protection10();
// Evklid's algorythm
int protection11(int m, int n);
// decoding message by Merkle-Hellman's algorythm
std::string protection12(const int* ciphertext, const int m, const int n);
// checking equalation of current and original CRC
bool protection13();
// self-modifying code
int protection14(void* addr, int wb);


#endif
