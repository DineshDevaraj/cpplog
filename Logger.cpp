
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

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <unistd.h>

#define PVT_LOG_LEVEL     (Logger::Level::Msg) 

#define PVT_MIN_FILE_SIZE (        1 * 1024)    /*  1 KB */
#define PVT_MAX_FILE_SIZE (32 * 1024 * 1024)    /* 32 MB */

#define fdup(fh, mode) fdopen(dup(fileno(fh)), mode)

#if !LOCAL_FUNCTIONS

static void MakePath(const char *dir) 
{
   char tmp[256];
   char *p = NULL;
   size_t len;

   snprintf(tmp, sizeof(tmp),"%s",dir);
   len = strlen(tmp);
   if(tmp[len - 1] == '/')
      tmp[len - 1] = 0;
   for(p = tmp + 1; *p; p++)
      if(*p == '/') {
         *p = 0;
         mkdir(tmp, S_IRWXU);
         *p = '/';
      }
   mkdir(tmp, S_IRWXU);
}

#endif

Logger_t::Logger_t(FILE *fh)
{
   fc = 1;
   cfs = 0;
   level = PVT_LOG_LEVEL;
   mfs = PVT_MAX_FILE_SIZE;
   this->fh = fdup(fh, "w");
   pt = Logger::PathType::File;
}

Logger_t::Logger_t(const char *path)
{
   SetPath(path);
}

Logger_t::Logger_t(const Logger_t &rval)
{
   pt = rval.pt;
   cfs = rval.cfs;
   mfs = rval.mfs;
   path = rval.path;
   level = rval.level;
   prefix = rval.prefix;
   format = rval.format;
   fh = fdup(rval.fh, "w");
}

Logger_t & Logger_t::operator = (const Logger_t &rval)
{
   fclose(fh);
   pt = rval.pt;
   cfs = rval.cfs;
   mfs = rval.mfs;
   path = rval.path;
   level = rval.level;
   prefix = rval.prefix;
   format = rval.format;
   fh = fdup(rval.fh, "w");
}

int Logger_t::SetPath(const char *path)
{
   fc = 1;
   cfs = 0;
   struct stat st;
   this->path = path;

   if(-1 == stat(path, &st))
   {
      MakePath(path);
      pt = Logger::PathType::Dir;
   }
   else
   {
      if(S_ISDIR(st.st_mode))
         pt = Logger::PathType::Dir;
      else
         pt = Logger::PathType::File;
   }

   string lpath;
   format = "log";
   prefix = "Logger";
   
   if(pt == Logger::PathType::Dir)
   {
      lpath = path;
      lpath += "/" + GetFileName();
      path = lpath.data();
   }

   fh = fopen(path, "w");
   level = PVT_LOG_LEVEL;
   mfs = PVT_MAX_FILE_SIZE;
   
   return 0;
}

int Logger_t::SetLevel(Logger::Level_t level)
{
   int cur = this->level;
   this->level = level;
   return cur;
}

int Logger_t::SetMaxFileSize(int size)
{
   int cur = -1;
   if(PVT_MIN_FILE_SIZE < size && size < PVT_MAX_FILE_SIZE)
   {
      cur = mfs;
      mfs = size;
   }
   return cur;
}

string Logger_t::GetFileName()
{
   struct tm tm;        
   time_t tick = time(NULL);

   localtime_r(&tick, &tm);
   tm.tm_mon += 1;
   tm.tm_year += 1900;
   
   char name[64] = {};
   sprintf(name, "%s_%02d%02d%02d_%02d%02d%02d_%03d.%s", prefix.data(), 
         tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, 
         tm.tm_min, tm.tm_sec, fc, format.data());

   return name;
}


int Logger_t::wrt(FILE *fh, Logger::Level_t level, const char *file, 
      const char *func, int line, const char *fmt, va_list vargs)
{
   int len = 0;
   static const char LevelAlpha[] = "@MPDWE";
   len += fprintf(fh, "%s %s %d [%c] ", file, func, line, LevelAlpha[level]);
   len += vfprintf(fh, fmt, vargs);
   cfs += len;
   if(pt == Logger::PathType::Dir && cfs >= mfs)
   {
      fclose(fh);
      string file;
      struct stat st;
      do
      {
         file = path + "/" + GetFileName();
      }while(-1 != stat(file.data(), &st) and fc++);
      fh = fopen(file.data(), "w");
      cfs = 0;
      fc = 1;
   }
   return len;
}

int Logger_t::err(const char *file, const char *func, int line, const char *fmt, ...)
{
   int len = -1;
   if(level <= Logger::Level::Err)
   {
      va_list vargs;
      va_start(vargs, fmt);
      len = wrt(fh, Logger::Level::Err, file, func, line, fmt, vargs);
   }
   return len;
}

int Logger_t::wrn(const char *file, const char *func, int line, const char *fmt, ...)
{
   int len = -1;
   if(level <= Logger::Level::Wrn)
   {
      va_list vargs;
      va_start(vargs, fmt);
      len = wrt(fh, Logger::Level::Wrn, file, func, line, fmt, vargs);
   }
   return len;
}

int Logger_t::dbg(const char *file, const char *func, int line, const char *fmt, ...)
{
   int len = -1;
   if(level <= Logger::Level::Dbg)
   {
      va_list vargs;
      va_start(vargs, fmt);
      len = wrt(fh, Logger::Level::Dbg, file, func, line, fmt, vargs);
   }
   return len;
}

int Logger_t::prf(const char *file, const char *func, int line, const char *fmt, ...)
{
   int len = -1;
   if(level <= Logger::Level::Prf)
   {
      va_list vargs;
      va_start(vargs, fmt);
      len = wrt(fh, Logger::Level::Prf, file, func, line, fmt, vargs);
   }
   return len;
}

int Logger_t::msg(const char *file, const char *func, int line, const char *fmt, ...)
{
   int len = -1;
   if(level <= Logger::Level::Msg)
   {
      va_list vargs;
      va_start(vargs, fmt);
      len = wrt(fh, Logger::Level::Msg, file, func, line, fmt, vargs);
   }
   return len;
}

Logger_t::~Logger_t()
{
   fclose(fh);
}
