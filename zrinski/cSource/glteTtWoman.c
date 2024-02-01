/* GenealogyLite "TableTranslation" program for table: [woman]
   ===========================================================

   Table translation programs (glteTt...) programs transform .csv
   ("comma-separated-value") text files composed - and, if the longevity
   of the information is a principal consideration, maintained - in any
   "spread-sheet" prgram - into a variation thereof suitable to be used
   is input to a batch load of tables into SQLite database via its
   .import command. The transformations are:

   1) First ("column header") line of the input file is the checked but
      removed from the output (cf. LABELS_LINE #define statement below).

   2) Quoted UTF-8 strings in input lines  (presumably cell data with
      included ',' characters) are stripped of quotes.

   3) Single date input item is split into two separate items: year and day.

      Input date item can have any of three possible forms:
      a) Single 1-4 digit integer.  (Julian or Gregorian ordinal year number).
      b) As the above, but prefixed by character 'a'.
      c) ASCII String of the form YYYYmMMdDD - also possibly prefixed by 'a'.
         Date, Julian before October 15, 1582, Gregorian after that date. 'a',
         'm' and 'd' (for Latin "Annus", "Mensis" and "Dies", respectively)
         introduce a numeric part of the date - year, month and day. Numeric
         part consists of 4 (for year component, 2 for month and day
         components) digits. The leading 0(s) may be omitted.

      The single item input date (in any of the three formats described above)
      is transformed into two items: 1-4 digit integer year number and integer
      (from 1 to 365/366) Year Day Number.

   The input file ("comma-separated-values") uses "," as item separator,
   the output file uses the vertical bar character ("|", SQLite default).

   Input is read from "standard input" and output is written to "standard
   output"; thus the invocation of the program will typically look like this:

   ./glteTtWoman < simpsonsWoman.csv > simpsonsWoman.bsv

   (assuming we use "simpsons" (family last name) at the prefix for all
   "simpsons" genealogy project files), "csv" is the default suffix of table
   composed in a spreadsheet program and .bsv is (suggested) suffix for
   "BarSeparatedValues" files that are used to load SQLite database).

 */

#define PGM_LAST_EDIT_DATE "2024.028"
#define LABELS_LINE  "id,mother,father,name,surname,maidenSurname,pEst,birthDate,birthPlace,deathDate,deathPlace,occupation,note,infoUrl\n"

#include "glteScullions.h"
#define IN_LINE_MAX  1024           /* maximum length of input text file line */
static const char *progName;    /* for error logging by this source file only */
/* ========================================================================== */
int main (int argc,
          const char *argv[],
          const char *envr[]) {
   int i, n, iRc;
   char *item, *pnl;             /* comma separated input item, newline strip */
   char *pa;
   char inLn[IN_LINE_MAX + 2];
/* -------------------------------------------------------------------------- */
   progName = strrchr(argv[0], '/');                                 /* POSIX */
   if (progName == NULL) progName = strrchr(argv[0], '\\');         /* MS Win */
   if (progName == NULL) progName = argv[0];                      /* neither? */
   else progName += 1;                        /* strip leading path separator */
   fprintf(stderr, "\n[%s] source as of: %s\n", progName, PGM_LAST_EDIT_DATE);

/* First line are column labels; confirm and waste it */
   pa = fgets(inLn, IN_LINE_MAX, stdin);
   if (pa == NULL) glte_Abort(progName, __LINE__, "missing column labels line\n");

   if (strcmp(inLn, LABELS_LINE)) glte_Abort(progName, __LINE__,
                                             "unexpeted labels line: %s", inLn);
   pa = fgets(inLn, IN_LINE_MAX, stdin);                   /* first table row */
   n = 1;
   while (pa) {
      pnl = strrchr(inLn, '\n');                    /* for simpler parsing... */
      if (pnl) *pnl = '\0';        /* ...strip ending newline from input line */
/*    fprintf(stderr, "line:[%s]\n", inLn); */

      item = glte_GetCsvItem(inLn);            /* first line item is woman id */
      iRc = glte_WriteId(item);
      if (iRc != 0) glte_Abort(progName, __LINE__,
                               "Missing or invalid woman id:[%s], line %d\n", item, n);
      i = atoi(item);
      if (i % 2) glte_Abort(progName, __LINE__,
                            "Woman id:[%s] must be an even number, line %d\n", item, n);

      item = glte_GetCsvItem(NULL);             /* second line item is mother */
      iRc = glte_WriteId(item);
      if (iRc > 0) glte_Abort(progName, __LINE__, "Invalid mother id, line %d\n", item, n);
      if (iRc == 0) {
         i = atoi(item);
         if (i % 2) glte_Abort(progName, __LINE__,
                              "Mother id:[%s] not an even number, line %d\n", item, n);
         }

      item = glte_GetCsvItem(NULL);              /* third line item is father */
/*    fprintf(stderr, "father:[%s]\n", item); */
      iRc = glte_WriteId(item);
      if (iRc > 0) glte_Abort(progName, __LINE__, "Invalid father id:[%s], line %d\n", item, n);
      if (iRc == 0) {
         i = atoi(item);
         if ((i % 2) == 0) glte_Abort(progName, __LINE__,
                                      "Father id:[%s] not an odd number, line %d %d\n", item, n, i);
         }

      item = glte_GetCsvItem(NULL);                 /* 4-th line item is name */
      iRc = glte_WriteText(item);          /* this one never returns an error */

      item = glte_GetCsvItem(NULL);              /* 5-th line item is surname */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);        /* 6-th line item is maidenSurname */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL); /* 7-th item is period estimate indicator */
      iRc = glte_WriteInt(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid period estimate indicator:[%s], line %d\n", item, n);

      item = glte_GetCsvItem(NULL);            /* 8-th line item is birthDate */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid birthDate:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);           /* 9-th line item is birthPlace */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);           /* 10-th line item is deathDate */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid deathDate:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);          /* 11-th line item is deathPlace */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);          /* 12-th line item is occupation */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);                /* 13-th line item is note */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);             /* 14-th line item is infoUrl */
      iRc = glte_WriteText(item);                   /* never returns an error */

/*    when stdout is redirected to a file, rub-out the extra separator, to... */
      fseek(stdout, -1, SEEK_CUR); /* ...avoid SQLite .import command warning */

      putchar('\n');
      n++;       /* count lines transformed into "bar-separated-value and..." */
      pa = fgets(inLn, IN_LINE_MAX, stdin);                    /* ...get next */
      }

   fprintf(stderr, "Done, %2d lines read, %d written\n", n, n - 1);
   return(0);
   }
/* ========================================================================== */
#include "glteScullions.c"
/* ========================================================================== */
