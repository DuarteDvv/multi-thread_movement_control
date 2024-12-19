#ifndef PASSA
#define PASSA

#include <stdio.h>
#include <time.h>

/*********************************************************
 Inclua o código a seguir no seu programa, sem alterações.
 Dessa forma a saída automaticamente estará no formato esperado
 pelo sistema de correção automática.
 *********************************************************/

void passa_tempo(int tid, int sala, int decimos)
{
   struct timespec zzz, agora;
   static struct timespec inicio = {0, 0};
   int tstamp;

   if ((inicio.tv_sec == 0) && (inicio.tv_nsec == 0))
   {
      clock_gettime(CLOCK_REALTIME, &inicio);
   }

   zzz.tv_sec = decimos / 10;
   zzz.tv_nsec = (decimos % 10) * 100L * 1000000L;

   if (sala == 0)
   {
      nanosleep(&zzz, NULL);
      return;
   }

   clock_gettime(CLOCK_REALTIME, &agora);
   tstamp = (10 * agora.tv_sec + agora.tv_nsec / 100000000L) - (10 * inicio.tv_sec + inicio.tv_nsec / 100000000L);

   printf("%3d [ %2d @%2d z%4d\n", tstamp, tid, sala, decimos);

   nanosleep(&zzz, NULL);

   clock_gettime(CLOCK_REALTIME, &agora);
   tstamp = (10 * agora.tv_sec + agora.tv_nsec / 100000000L) - (10 * inicio.tv_sec + inicio.tv_nsec / 100000000L);

   printf("%3d ) %2d @%2d\n", tstamp, tid, sala);
}
/*********************** FIM DA FUNÇÃO *************************/

#endif