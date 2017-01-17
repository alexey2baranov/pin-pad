#include <windows.h>
#include <iostream>

#include "..\..\..\..\pilot_nt.h"
#include "..\..\..\..\deprtmnt.h"

#ifndef WINCE
  #define FUNC_NAME "_card_authorize9"
  #define DDL_NAME "pilot_nt.dll"
#else
  #define FUNC_NAME L"card_authorize9"
  #define DDL_NAME L"pilot_nt.dll"
#endif



typedef int (*dynamic_function)(char *track2, struct auth_answer9 *auth_answer);

DWORD spasibo(dynamic_function function)
{
  auth_answer9 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_PURCHASE;
  argument.ans.Amount = 100000;
  std::cout << "Let's try to pay!" << std::endl;
  DWORD result = function(NULL,&argument);
  // after call 'argument' contains card hash which required for second call
  if (result == 4353)
  {
    std::cout << "Sberbank Spasibo detected!" << std::endl;
    // KKM code can change amount (payment by spasibo)
    argument.ans.Amount = 90000;
    result = function(NULL,&argument);
  }
  
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

    spasibo(function);
  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}