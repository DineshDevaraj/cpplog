
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 14 Jan 2015 - Wed
 * Updated  : 15 Jan 2015 - Thu
 * 
 * Licence  : Refer the license file
 *
 **/

#pragma once

#include <stdio.h>
#include <iostream>

using namespace std;

struct Logger
{
   struct Level
   {
      enum Value
      {
         Msg = 1,   /* lowest level */
         Prf,
         Dbg,
         Wrn,
         Err        /* highest level */
      };
   };  

   typedef Level::Value Level_t;

   struct PathType
   {
      enum Value
      {
         Dir,
         File
      };
   };

   typedef PathType::Value PathType_t;

};

class Logger_t
{
   int fc;                 /* file count */
   int cfs;                /* curent file size  */
   int mfs;                /* maximum file size  */   
   FILE *fh;               /* latest file handle */
   string path;            /* path to targer file of folder */
   string prefix;
   string format;          /* file format eg. txt, log, ... */
   Logger::Level_t level;
   Logger::PathType_t pt;

   string GetFileName();
   int wrt(FILE *fh, Logger::Level_t level, const char *file, 
         const char *func, int line, const char *fmt, va_list vargs);

   public :
      
      /* path - path to target file or folder. Function detects      *
       * the type of the given path as file or folder. If the given  *
       * path does not exist a new folder of given path is created   */
      Logger_t(const char *path);
      
      Logger_t(FILE *fh = stdout);
      
      Logger_t(const Logger_t &rval);
      
      Logger_t & operator = (const Logger_t &rval);

      /* change the path of file or folder during the run time */
      int SetPath(const char *path);

      /* returns the previous level and set the new level */
      int SetLevel(Logger::Level_t level);
      
      int SetMaxFileSize(int size);

      /* error */
      int err(const char *file, const char *func, int line, const char *fmt, ...);

      /* warning */
      int wrn(const char *file, const char *func, int line, const char *fmt, ...);

      /* debug */
      int dbg(const char *file, const char *func, int line, const char *fmt, ...);

      /* performance */
      int prf(const char *file, const char *func, int line, const char *fmt, ...);

      /* message */
      int msg(const char *file, const char *func, int line, const char *fmt, ...);

      ~Logger_t();
};

#define LI __FILE__, __PRETTY_FUNCTION__, __LINE__ /* location information */

#define Err(fmt, ...) err(LI, fmt, ## __VA_ARGS__)
#define Wrn(fmt, ...) wrn(LI, fmt, ## __VA_ARGS__)
#define Dbg(fmt, ...) dbg(LI, fmt, ## __VA_ARGS__)
#define Prf(fmt, ...) prf(LI, fmt, ## __VA_ARGS__)
#define Msg(fmt, ...) msg(LI, fmt, ## __VA_ARGS__)
