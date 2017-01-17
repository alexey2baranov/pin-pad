#include <windows.h>
#include <iostream>

#include "..\..\..\..\pilot_nt.h"
#include "..\..\..\..\deprtmnt.h"

#ifndef WINCE
  #define FUNC_NAME "_card_authorize12"
  #define DDL_NAME "pilot_nt.dll"
#else
  #define FUNC_NAME L"card_authorize12"
  #define DDL_NAME L"pilot_nt.dll"
#endif



typedef int (*dynamic_function)(char *track2, struct auth_answer12 *auth_answer);

DWORD preauth(dynamic_function function, DWORD amount, std::string& rrn)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_PREAUTH;
  argument.ans.Amount = amount;
  std::cout << "First authorization. Amount = " << amount << std::endl;
  DWORD result = function(NULL,&argument);
  if (!result)
    rrn = std::string(argument.RRN);

  std::cout << "Operation completed with code '" << result << "'. RRN:" << rrn.c_str() << std::endl;
  return result;
}

DWORD add_auth(dynamic_function function, DWORD amount, const std::string& rrn)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_ADD_AUTH;
  argument.ans.Amount = amount;
  strcpy(argument.RRN, rrn.c_str());
  std::cout << "Second authorization. Amount = " << amount << ". RRN:" << rrn.c_str() <<std::endl;
  DWORD result = function(NULL,&argument);
  if(strcmp(rrn.c_str(), argument.RRN) != 0)
    std::cerr << "Wrong RRN!" << std::endl;

  std::cout << "Operation completed with code '" << result << "'!" << std::endl;
  return result;
}

DWORD complete_auth(dynamic_function function, DWORD amount, const std::string& rrn)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_COMPLETION;
  argument.ans.Amount = amount;
  strcpy(argument.RRN, rrn.c_str());
  std::cout << "Lets complete authorization. Amount = " << amount << ". RRN:" << rrn.c_str() << std::endl;
  DWORD result = function(NULL,&argument);

  std::cout << "Operation completed with code '" << result << "'!" << std::endl;
  return result;
}

DWORD cancel_auth(dynamic_function function, const std::string& rrn)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_COMPLETION;
  argument.ans.Amount = 0;
  strcpy(argument.RRN, rrn.c_str());
  std::cout << "Lets cancel authorization" << " RRN:" << rrn.c_str() << std::endl;
  DWORD result = function(NULL,&argument);

  std::cout << "Operation completed with code '" << result << "'!" << std::endl;
  return result;
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
    std::cerr << "pilot_nt.dll not found";
    return -1;
  }

  try {
    dynamic_function function = (dynamic_function)GetProcAddress(dll, FUNC_NAME);
    if (!function)
    {
      std::cerr << "function " << FUNC_NAME << " in " << DDL_NAME << " not found";
      return -2;
    }

    std::string rrn;
    //preauth(function, 1000, rrn);
    //add_auth(function, 2000, rrn);
    //complete_auth(function, 1000 + 2000, rrn);

    preauth(function, 3000, rrn);
    add_auth(function, 4000, rrn);
    cancel_auth(function, rrn);
  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}