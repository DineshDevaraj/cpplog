
/**
 *
 * Author   : D.Dinesh - 1800,
 *            Software Developer
 *
 * Created  : 14 Jan 2015 - Wed
 * Updated  : 14 Jan 2015 - Wed
 *
 **/

#include "Logger.h"

struct Called
{
   Called(Logger_t &log)
   {
      log.Prf("Welcome new Logger\n");
   }
};

int main(int argc, char *argv[])
{
   Logger_t log;
   
   if(argc > 1) log = Logger_t(argv[1]);
   
   // log.SetLevel(Logger::Level::Dbg);
   log.SetMaxFileSize(8 * 1024 * 1024); // 8 MB

   for(int I = 0; I < 10000000; I++)
   {
      log.Dbg("Hello world int %d and float %f\n", 10, 97.65);
      Called obj(log);
   }

   return 0;
}
