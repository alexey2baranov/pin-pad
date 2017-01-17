#include <windows.h>
#include <iostream>

#include "..\..\..\..\pilot_nt.h"
#include "..\..\..\..\deprtmnt.h"

#ifndef WINCE
  #define FUNC_NAME "_card_authorize9"
  #define TEST_PINPAD "_TestPinpad"
  #define DDL_NAME "pilot_nt.dll"
#else
  #define FUNC_NAME L"card_authorize9"
  #define TEST_PINPAD L"TestPinpad"
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

struct ThreadData
{
  HMODULE Dll;
  bool Stop;
  ThreadData(HMODULE dll): Dll(dll), Stop(false) {}
};

DWORD WINAPI TestPinpadInThread(CONST LPVOID lpParam) {
  ThreadData* threadData = (ThreadData*)lpParam;
  if (!threadData || !threadData->Dll)
    return 0;
 
  //TODO: Do never call terminal library w/o sinchronization! This only for test!!!
 
  typedef int (*test_pp_function)();
  test_pp_function function = (test_pp_function)GetProcAddress(threadData->Dll, TEST_PINPAD);
  printf("Start TestPinpad...\n");

  while(!threadData->Stop)
  {
    printf("TestPinpad result '%d'\n", function());
    Sleep(5000);
  }

  printf("Stop TestPinpad\n");

  ExitThread(0);
  return 0;
}

#define KEY_ESC 27

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

  ThreadData threadData(dll);

  HANDLE thread = CreateThread(NULL, 0, &TestPinpadInThread, &threadData, 0, NULL);

  try {
    dynamic_function function = (dynamic_function)GetProcAddress(dll, FUNC_NAME);
    if (!function)
    {
      std::cerr << "function " << FUNC_NAME << " in " << DDL_NAME << " not found";
      return -2;
    }

    spasibo(function);

    printf("Push ESC to exit\n");
    while(getchar() != KEY_ESC)
    {
      Sleep(1000);
    }

    printf("ESC detected\n");
    threadData.Stop = true;
    WaitForSingleObject(thread, INFINITE);

  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}