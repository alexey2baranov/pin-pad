/*
  Example for using Spasibo with tips by two transactions.
  After payment the example performs partial reversal from usual payment.
*/

#include <windows.h>
#include <iostream>

#include "..\..\..\..\pilot_nt.h"
#include "..\..\..\..\deprtmnt.h"

#ifndef WINCE
  const char* CALL_CARD_AUTHORIZE12_NAME = "_card_authorize12";
#else
  const char* CALL_CARD_AUTHORIZE12_NAME = "card_authorize12";
#endif

typedef int (*dynamic_card_authorize12)(char *track2, struct auth_answer12 *auth_answer);

DWORD spasibo(dynamic_card_authorize12 function, DWORD amount, DWORD spasiboAmount)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_PURCHASE;
  argument.ans.Amount = amount;
  std::cout << "Let's try to pay!" << std::endl;
  DWORD result = function(NULL,&argument);
  // after call 'argument' contains card hash which required for second call
  if (result == 4353)
  {
    std::cout << "Sberbank Spasibo detected!" << std::endl;
    // KKM code can change amount (payment by spasibo)
    argument.ans.Amount = amount - spasiboAmount;
    result = function(NULL,&argument);
  }
  
  std::cout << "Operation completed with code '" << result << "'!" << std::endl;
  return result;
}

DWORD operation_cancel(dynamic_card_authorize12 function, DWORD amount, DWORD department)
{
  auth_answer12 argument;

  memset(&argument,0,sizeof(argument));
  argument.ans.TType = OP_RETURN;
  argument.ans.Amount = amount;
  argument.Department = department;
  std::cout << "Let's try to return part of amount!" << std::endl;
  DWORD result = function(NULL,&argument);

  std::cout << "Operation completed with code '" << result << "'!" << std::endl;
  return result;
}

#define FROM_USUAL_PAYMENT 0
#define FROM_PODARI_JIZN 1

int main (int argc, char** argv)
{
  HMODULE dll = LoadLibrary("pilot_nt.dll");
  if (!dll) 
  {
    std::cerr << "pilot_nt.dll not found";
    return -1;
  }

  try {
    dynamic_card_authorize12 function = (dynamic_card_authorize12)GetProcAddress(dll, CALL_CARD_AUTHORIZE12_NAME);
    if (!function)
    {
      std::cerr << "function " << CALL_CARD_AUTHORIZE12_NAME << " in pilot_nt.dll not found";
      return -2;
    }

    /*
      initial amount - 1234.56 RUB
      payment by spasibo 200 RUB
      reversal 30 RUB
      as result customer pay 1004.56 RUB
    */
    spasibo(function, 123456, 20000);
    operation_cancel(function, 3000, FROM_USUAL_PAYMENT);

  }
  catch(...)
  {
    FreeLibrary(dll);
  }
  return 0;
}