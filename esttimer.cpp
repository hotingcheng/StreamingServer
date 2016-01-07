
#include "es_timer.h"
#include <stdio.h>
#include <iostream.h>

int main ()
{

   ES_FlashTimer timer;
   while (getchar() != 'q')
      cout << timer.Elapsed() << endl;
   return 0;
}
