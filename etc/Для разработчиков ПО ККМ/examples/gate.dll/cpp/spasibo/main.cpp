#include <windows.h>
#include <iostream>

#include "..\..\..\..\deprtmnt.h"
#include "..\..\..\..\krnltype.h"

#ifndef WINCE
  #define CALL_SB_KERNEL_NAME "_call_sb_kernel"
  #define DDL_NAME "gate.dll"
#else
  #define CALL_SB_KERNEL_NAME L"call_sb_kernel"
  #define DDL_NAME L"gate.dll"
#endif

void  hex_2_ascii(const BYTE *source,BYTE *destn, WORD sz)
{
  BYTE  a, c;
  WORD i,j;

  for(i=0,j=0;sz--;){
    c = source[j];
    a = (BYTE)(c>>4);
    if (a <= 9)
      destn[i++] = (BYTE)(a + '0');
    else
      destn[i++] = (BYTE)(a + '7');

    c = source[j++];
    a = (BYTE)(c&0x0f);
    if (a <= 9)
      destn[i++] = (BYTE)(a + '0');
    else
    destn[i++] = (BYTE)(a + '7');
  }
  destn[i] = '\0';
}

bool initialize(Gate_Call_Sb_Kernel function)
{
  InArg in;
  OutArg out;
  struct_in0 InDetail;
  struct_out0 OutDetail;
  memset(&in, 0, sizeof(in));
  memset(&InDetail, 0, sizeof(InDetail));
  in.in_struct = &InDetail;
  InDetail.AppVersion = 0x00030101;
  memset(&out, 0, sizeof(out));
  memset(&OutDetail, 0, sizeof(OutDetail));
  out.out_struct = &OutDetail;

  if(function && !function(0,&in,&out))
    if(!out.ErrorCode)
      return true;

  return false;
}

DWORD spasibo(Gate_Call_Sb_Kernel function)
{
  InArg in;
  OutArg out;
  struct_in_cl4 m_in;
  struct_out_cl4 m_out;
  struct_in_reserved4  m_res;
  struct_out_reserved4 out_res;

  memset(&in,0,sizeof(in));
  memset(&m_in,0,sizeof(m_in));
  memset(&m_res,0,sizeof(m_res));
  in.in_struct = &m_in;
  in.Reserved = &m_res;
  m_res.size = sizeof(m_res);

  memset(&out,0,sizeof(out));
  memset(&m_out,0,sizeof(m_out));
  memset(&out_res,0,sizeof(out_res));
  out.out_struct = &m_out;
  out.Reserved = &out_res;
  out_res.size = sizeof(out_res);

  m_res.Department = DEPARTMENT_NOT_SPECIFIED;
  m_in.Amount = 100000;
  std::cout << "Let's try to pay!" << std::endl;
  DWORD result = function(4000,&in,&out);
  if (out.ErrorCode == 4353)
  {
    std::cout << "Sberbank Spasibo detected!" << std::endl;
    m_in.Amount = 90000;
    strcpy(m_in.Track2, (const char*)out_res.Cert);
    result = function(4000,&in,&out);
  }
  
  std::cout << "Operation completed with code '" << out.ErrorCode << "'!" << std::endl;
  return out.ErrorCode;
}

#ifndef WINCE
int main (int argc, char** argv)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
#endif
{
  HMODULE dll = LoadLibrary(DDL_NAME);
  if (!dll) 
  {
    std::cerr << "gate.dll not found";
    return -1;
  }

  try {
    Gate_Call_Sb_Kernel gate_Call_Sb_Kernel = (Gate_Call_Sb_Kernel)GetProcAddress(dll, CALL_SB_KERNEL_NAME);
    if (!gate_Call_Sb_Kernel)
    {
      std::cerr << "function _call_sb_kernel in gate.dll not found";
      return -2;
    }

    if(!initialize(gate_Call_Sb_Kernel))
    {
      std::cerr << "failed to initialize gate.dll";
      return -3;
    }

    spasibo(gate_Call_Sb_Kernel);
  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}
