#ifndef COMMON_H
#define COMMON_H

#include "util/build_defs.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 2

static const unsigned char completeVersion[] =
{
  'V',
  VERSION_MAJOR_INIT,
  '.',
  VERSION_MINOR_INIT,
  '-',
  BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
  '-',
  BUILD_MONTH_CH0, BUILD_MONTH_CH1,
  '-',
  BUILD_DAY_CH0, BUILD_DAY_CH1,
  '-',
  BUILD_HOUR_CH0, BUILD_HOUR_CH1,
  ':',
  BUILD_MIN_CH0, BUILD_MIN_CH1,
  ':',
  BUILD_SEC_CH0, BUILD_SEC_CH1,
  '\0'
};

// static char *ultoa(unsigned long val, char *s)
// {
//   char *p = s + 13;
//   *p = '\0';
//   do
//   {
//     if ((p - s) % 4 == 2)
//       *--p = ',';
//     *--p = '0' + val % 10;
//     val /= 10;
//   } while (val);
//   return p;
// }

#endif /* COMMON_H */
