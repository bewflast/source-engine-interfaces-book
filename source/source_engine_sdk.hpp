#ifndef     SOURCE_ENGINE_SDK_HPP
# define    SOURCE_ENGINE_SDK_HPP

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