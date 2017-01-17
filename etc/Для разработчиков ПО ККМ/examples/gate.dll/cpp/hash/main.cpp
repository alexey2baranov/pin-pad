#include <windows.h>
#include <iostream>

#include "..\..\..\..\krnltype.h"
#include "..\..\..\..\deprtmnt.h"

#ifndef WINCE
  const char* CALL_SB_KERNEL_NAME = "_call_sb_kernel";
#else
  const char* CALL_SB_KERNEL_NAME = "call_sb_kernel";
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

DWORD read_card_hash(Gate_Call_Sb_Kernel function, std::string& hash)
{
  InArg in;
  OutArg out;
  struct_in_cl5 m_in;
  struct_out_cl5 m_out;
  struct_in_reserved5  m_res;
  struct_out_reserved5 out_res;

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
  std::cout << "Let's read hash!" << std::endl;
  if (DWORD result = function(5002,&in,&out))
  {
    std::cerr << "Error '" << result << "'"<< std::endl;
  }

  hash.resize(40);
  hex_2_ascii(out_res.Hash, (BYTE*)&hash[0], sizeof(out_res.Hash));
  std::cout << "Hash: '" << hash.c_str() << "'"<< std::endl;
  return out.ErrorCode;
}

DWORD pay_with_hash(Gate_Call_Sb_Kernel function, DWORD amount, const std::string& hash)
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

  std::cout << "Let's pay with hash '" << hash.c_str() <<  "'!" << std::endl;
  m_res.Department = DEPARTMENT_NOT_SPECIFIED;
  m_in.Amount = amount;
  strcpy(m_in.Track2, hash.c_str());
  DWORD result = function(4000,&in,&out);

  return out.ErrorCode;
}

int main (int argc, char** argv)
{
  HMODULE dll = LoadLibrary("gate.dll");
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

    std::string hash;
    const DWORD STO_RUBLEY = 10000;
    read_card_hash(gate_Call_Sb_Kernel, hash);
    pay_with_hash(gate_Call_Sb_Kernel, STO_RUBLEY, hash);
  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}
