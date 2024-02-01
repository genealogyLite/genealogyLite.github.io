/* scullion:  someone employed to do unskilled jobs in a kitchen
 */

#ifndef GLTE_H_INCLUDED
#define GLTE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

void  glte_Abort(const char *, int, const char *, ...);
int   glte_StrIsDigits(char *);
char *glte_GetCsvItem(char *);

int   glte_WriteId(char *);
int   glte_WriteInt(char *);
int   glte_WriteText(char *);
int   glte_WriteYearDay(char *);

/* Rudimentary calendar manipulation. Some abbreviations:
   Jdn: count of days since January 1, 4713 BC, proleptic Julian calendar
   Gdn: count of days since beginning of proleptic Gregorian calendar
   Ymd: Year, month, date (calendar date)
   Yd:  Day of year (where January 1 is Yd 1)
 */
int   glte_LeapYear(int);
int   glte_DateDissect(const char *, int *, int *, int *);
int   glte_YmdToYd(int, int, int);
void  glte_YdToTmd(int, int, int *, int *);

#endif
