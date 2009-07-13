#include <stdio.h>
#include "EWFaccess.h"

void main(int argc, _TCHAR* argv[])
{
  EWFaccess* ptr = NULL;

  ptr = new EWFaccess();

  printf(" EFWaccess return address %08X\n", ptr );

  delete ptr;
}
//---------------------------------------------------------------------------
