/* Miscellaneous helper functions ("scullions") used by programs performing
   batch .import of SQLite database with spreadsheet tables as source.
 */
#include "glteScullions.h"

/* year-day of the first day of month for leap year and non-leap years */
static int lpM0[] = {1, 32, 61, 92, 122, 153, 183, 214, 245, 275, 306, 336};
static int nlM0[] = {1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
/* ========================================================================== */
/* Abort program execution with source file name/line and optional diagnostic.
   This is not an application-ready way to deal with error conditions, it is
   only fit for programmer's development test/demo/scaffolding code!
 */
void glte_Abort(const char *progname,                   /* given program name */
                int line,                                     /* program line */
                const char *fmt,           /* exit message text/format string */
                ...) {                           /* diagnostic items to print */
/* -------------------------------------------------------------------------- */
   va_list p_arg;
   va_start(p_arg, fmt);
   fprintf(stderr, "Error: %s(%d): ", progname, line);
   vfprintf(stderr, fmt, p_arg);
   va_end(p_arg);
   exit(1);
   }
/* ========================================================================== */
/* Return 1 if string of all digits, 0 if not. (Somewhat arbitrary,
   it considers an empty string an "is-digits" one).
 */
int glte_StrIsDigits(char *dsOp) {
   while (*dsOp) {
      if (isdigit(*dsOp++) == 0) return(0);
      }
   return(1);
   }
/* ========================================================================== */
/* "Special-purpose strtok()" - one that only extracts using ',' as sub-string
    separators, but it ignores those inside of (double) quote-strings -
    since this is a common method for "spread-sheet" programs to output cell
    values that include comma character(s). Like strtok(), it expect the null
    at the end  of the input string - so there better be one near-by.
 */
static char *next;                               /* static, so initially NULL */
char *glte_GetCsvItem(char *s) {
   char *pr, *prQ;
/* -------------------------------------------------------------------------- */
   if (s) {      /* first call on the next input string (typically file line) */
      if (*s == ',') {                        /* string start is a delimiter? */
         *s = '\0';                                   /* make it empty string */
         next = s + 1;         /* point to where next extraction should start */
         return(s);                               /* return that empty string */
         }
      else {
         next = s;
         }
      }
   if (next == NULL) return(NULL);   /* line end reached, keep returning NULL */
   if (*next == '\0') {                 /* end reached in previous invocation */
      next = NULL;
      return(NULL);
      }

   while ((*next == ' ') || (*next == 5)) {     /* first, skip any whitespace */
      next++;
      if (*next == '\0') {     /* hitting line end while skipping whitespace? */
         next = NULL;
         return(NULL);
         }
      }

/* real work starts here: "next" points to start of what is to be extracted.  */

   if (*next == '\"') {                           /* and it is a quote string */
      pr = next;      /* we'll return this if matching end quote not found... */
      prQ = ++next;               /* ...and this if it was found and stripped */
/*    advance for matching double quote (or end of string) */
      while ((*next != '\"') && (*next != '\0')) next++;
      if (*next == '\0') {     /* hit end of string instead of ending quote!? */
         next = NULL;
         return(pr);
         }
      *next++ = '\0';                  /* end substring to be returned as prQ */
      while ((*next != ',') && (*next != '\0')) next++;
      if (*next == '\0') next = NULL;  /* line end while skipping whitespace? */
      else next++;
      return(prQ);
      }

/* "next" now points to the beginning of an ordinary string to be returned */
   pr = next;             /* we'll return this, but must find end of it first */
   while ((*next != ',') && (*next != '\0')) next++;
   if (*next == '\0') next = NULL;        /* hit end of string instead of "," */
   else *next++ = '\0';          /* terminate, set resume scanning pointer... */
   return(pr);                                 /* ...and return the substring */
   }
/* ========================================================================== */
/* Given string (table item) from input csv, write the equivalent item(s) in
   a bsv ("BarSeparatedValues") text file that can be used in SQLite .import
   Return 0 if the non-NULL item was written,
   -1 if empty (NULL) item was written,
    1 error code otherwise.
 */
int glte_WriteId(char *s) {                         /* write person id number */
   if ((s == NULL) || (strlen(s) == 0)) {     /* deal with NULL/empty strings */
      putchar('|');                                               /* no value */
      return(-1);
      }
   if (glte_StrIsDigits(s) == 0) return(1);      /* string must be all-digits */
   if (atoi(s) < 1) {                                  /* interpret 0 as NULL */
      putchar('|');
      return(-1);
      }
   fputs(s, stdout);          /* some value given, write it without change... */
   putchar('|');                                      /* ...and the separator */
   return(0);
   }
/* ========================================================================== */
/* As above, for integers. (Currently same as writing person id, but duplicated
   in order to make possible future changes to person id manipulation simpler)
 */
int glte_WriteInt(char *s) {                         /* write generic integer */
   if ((s == NULL) || (strlen(s) == 0)) {     /* deal with NULL/empty strings */
      putchar('|');                                               /* no value */
      return(-1);
      }
   if (glte_StrIsDigits(s) == 0) return(1);      /* string must be all-digits */
   if (atoi(s) < 1) {                                  /* interpret 0 as NULL */
      putchar('|');
      return(-1);
      }
   fputs(s, stdout);          /* some value given, write it without change... */
   putchar('|');                                      /* ...and the separator */
   return(0);
   }
 /* ========================================================================== */
/* As above, for text items (UTF-8 names, places, notes, etc.)
   If the string is enclosed in double quotes, strip those. No '|' character
   is allowd in input string, but any UTF-8 glyph is ok.
   Return value as above.
 */
int glte_WriteText(char *s) {                           /* write text string. */

   if ((s == NULL) || (strlen(s) == 0)) {     /* deal with NULL/empty strings */
      putchar('|');                                               /* no value */
      return(-1);
      }
   fputs(s, stdout);                       /* some value given, write text... */
   putchar('|');                                          /* ...and separator */
   return(0);
   }
/* ========================================================================== */
/* As above, input item is date. Two output items must be written, first is
   the year, second is the yearDay ("day of year").
   Error returns are as provided by date dissection function.
 */
int glte_WriteYearDay(char *s) {
   int iRc, nY, nM, nD, nYd;
   if ((s == NULL) || (strlen(s) == 0)) {     /* deal with NULL/empty strings */
      fputs("||", stdout);                   /* two NULL items: year, yearDay */
      return(-1);
      }
   iRc = glte_DateDissect(s, &nY, &nM, &nD);
/* fprintf(stderr, "[%s]:(%d)%d-%d-%d\n", s, iRc, nY, nM, nD); */
   if (iRc > 0) return(iRc);
   if (iRc < 0) {
      printf("%d||", nY);
      }
   else {            /* both year and date were given, turn date into YearDay */
      nYd = glte_YmdToYd(nY, nM, nD);
      printf("%d|%d|", nY, nYd);
      }
   return(0);
   }
/* ========================================================================== */
/* return 1 if given year is a leap year, 0 otherwise.
 */
int glte_LeapYear(int nY) {
   if (nY > 1582) {                         /* assume Gregorian calendar year */
      if (nY % 400 == 0) return(1);            /*  perfectly divisible by 400 */
      if (nY % 100 == 0) return(0);        /* divisible by 100 but not by 400 */
      if (nY % 4 == 0) return(1);  /* not divisible by 100 but divisible by 4 */
      return(0);              /* all other Gregorian years are not leap years */
      }
   else {                                             /* Julian calendar year */
      if (nY % 400 == 0) return(1);            /*  perfectly divisible by 400 */
      return(0);                 /* all other Julian years are not leap years */
      }
   }
/* ========================================================================== */
 /* Given string either pure numeric YYYY or YYYYMMDD, or alphanumeric in
   aYYYYmMMdDD form, return calendat Year, Month and Day.

   Signal ill-formed or illegal date string. Leap year rules are assumed
   to be Julian if 1582 or earlier, Gregorian otherwise.
 */
int glte_DateDissect(const char *dateString,        /* given text date string */
                     int *nA,                                /* returned year */
                     int *nM,                               /* returned month */
                     int *nD) {

   int i, l, iTestM, iTestD;
   char dsOp[16];
   char *pm, *pd;
/* -------------------------------------------------------------------------- */
   l = strlen(dateString);
   if ((l < 1) || (l > 11)) return(1);      /* input string empty or too long */
   if (*dateString == 'a') dateString++;  /* 'a' at the beginning is optional */
   strcpy(dsOp, dateString);
/* fprintf(stderr, "work string: [%s]\n", dsOp); */

   if (!isdigit(*dsOp)) return(1);

/* First deal with numeric only YYYY or YYYYMMDD */
   if (glte_StrIsDigits(dsOp)) {
/*    fprintf(stderr, "numeric only work string: [%s]\n", dsOp); */
      l = strlen(dateString);
      if (l < 5) {                                            /* number, year */
         *nA = atoi(dsOp);
         *nM = *nD = 0;
         return(-1);                       /* only the numeric year was given */
         }
      if (l == 8) {               /* assume it is YYYYMMDD (or was aYYYYMMDD) */
         *nD = atoi(dsOp + 6);
         dsOp[6] = '\0';
         *nM = atoi(dsOp + 4);
         dsOp[4] = '\0';
         *nA = atoi(dsOp);
         }
      else return(1);                        /* all numeric, but wrong length */
      }
   else {    /* there are some alpha characters, are they one 'm' and one 'd' */
/*    fprintf(stderr, "alpha-numeric work string: [%s]\n", dsOp); */
      i = -1;                 /* disqualify strings with illegal character(s) */
      iTestM = iTestD = 1;                     /* test for both month and day */
      pm = pd = NULL;                         /* assume neither will be found */
      while (dsOp[++i]) {
/*       fprintf(stderr, "test %d - %c\n", i, dsOp[i]); */
         if (isdigit(dsOp[i])) continue;
/*       so, it's not a digit? One 'm' and one 'd' in string is also okay: */
         if (iTestM && (dsOp[i] == 'm')) {                 /* first 'm' found */
            dsOp[i] = '\0';                 /* terminate preceding sub-string */
            pm = dsOp + i + 1;              /* mark month sub-string position */
            iTestM = 0;                                       /* no more 'm's */
            continue;
            }
         if (iTestD && (dsOp[i] == 'd')) {                 /* first 'd' found */
            dsOp[i] = '\0';                 /* terminate preceding sub-string */
            pd = dsOp + i + 1;                /* mark day sub-string position */
            iTestD = 0;                                       /* no more 'd's */
            continue;
            }
         return(2);
         }
/*    fprintf(stderr, "character tests passed, %p, %p\n", pm, pd); */
      if ((pd && !pm) || (pm && !pd)) return(3);             /* both or none! */

      *nA = atoi(dsOp);                                            /* n Annus */
      if (pm == NULL) return(-1);                  /* only the year was given */

      *nM = atoi(pm);                                             /* n Mensis */
      *nD = atoi(pd);                                               /* n Dies */
/*    fprintf(stderr, "a:%d m:%d d:%d\n", *nA, *nM, *nD); */

      if ((*nM < 1) || (*nM > 12)) return(4);              /* bad month value */
      if ((*nD < 1) || (*nD > 31)) return(4);                /* bad day value */
      }

/* confirm day value meets the calendar rules. Is it a leap year? */
   if (*nM == 2) {                                                /* February */
/*    fprintf(stderr, "February, leap year:%s\n", glte_LeapYear(*nA) ? "yes" : "no"); */
      if (glte_LeapYear(*nA)) {
         if (*nD > 29) return(5);               /* leap year, day <= 29 is ok */
         }
      else {
         if (*nD > 28) return(4);           /* in "normal" year, must be < 29 */
         }
      }
   if ((*nM == 4) || (*nM == 6) || (*nM == 9) || (*nM == 11)) {   /* other... */
      if (*nD > 30) return(4);                           /* ..."short months" */
      }

   return(0);
   }
/* ========================================================================== */
/* Return day-of-year for a given year, month and day. Leap-years rules are
   Julian if 1582 and earlier, Gregorian otherwise. No error checking is
   performed other than array acess protection.
 */
int glte_YmdToYd(int nY, int nM, int nD) {
   int m0;                        /* year-day of the first day of given month */
   if ((nM < 1) || (nM > 12)) return(0);    /* to protect from over/underflow */
   if (glte_LeapYear(nY)) m0 = lpM0[nM - 1];
   else m0 = nlM0[nM - 1];
   return(m0 + nD - 1);
   }
/* ========================================================================== */
/* Return month and day given the year and day-of-year.
 */
void glte_YdToTmd(int nY, int nYd, int *nM, int *nD) {
   int i;
   *nM = *nD = 0;
   if ((nY < 1) || (nYd < 1)) return;     /* from year and day-of-year 1 only */

   if (glte_LeapYear(nY)) {
      if (nYd > 366) return;
      for (i = 11; i; i--) {
         if (nYd >= lpM0[i]) break;
         }
      *nM = i + 1;
      *nD = nYd - lpM0[i] + 1;
      }
   else {
      if (nYd > 365) return;
      for (i = 11; i; i--) {
         if (nYd >= nlM0[i]) break;
         }
      *nM = i + 1;
      *nD = nYd - nlM0[i] + 1;
      }
   return;
   }
/* ========================================================================== */
