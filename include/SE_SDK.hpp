#ifndef     SE_SDK_HPP
# define    SE_SDK_HPP

using CreateInterfaceFn 		=  void* (*)(const char* ,int*);
using InstantiateInterfaceFn 	=  void* (*)();

class InterfaceReg
{
	
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char *pName);

public:
	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;

	InterfaceReg			*m_pNext;
	static InterfaceReg		*s_pInterfaceRegs;

};

#endif