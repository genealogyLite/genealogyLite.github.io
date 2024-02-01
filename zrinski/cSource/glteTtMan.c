/* GenealogyLite "TableTranslation" program for table: [man]
   =========================================================

   See the preamble to glteTtWoman.c program source code and
   the #define LABELS_LINE statement below.
 */

#define PGM_LAST_EDIT_DATE "2024.028"
#define LABELS_LINE  "id,mother,father,name,surname,pEst,birthDate,birthPlace,deathDate,deathPlace,occupation,note,infoUrl\n"

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
      pnl = strrchr(inLn, '\n');                /* for simpler parsing... */
      if (pnl) *pnl = '\0';        /* ...strip ending newline from input line */
/*    fprintf(stderr, "input file line:[%s]\n", inLn); */

      item = glte_GetCsvItem(inLn);        /* first line item is woman id */
      iRc = glte_WriteId(item);
      if (iRc != 0) glte_Abort(progName, __LINE__,
                               "Missing or invalid woman id:[%s], line %d\n", item, n);
      i = atoi(item);
      if ((i % 2) == 0) glte_Abort(progName, __LINE__,
                                   "Man id must be an odd number, line %d\n", n);

      item = glte_GetCsvItem(NULL);             /* second line item is mother */
      iRc = glte_WriteId(item);
      if (iRc > 0) glte_Abort(progName, __LINE__, "Invalid mother id:[%s], line %d\n", item, n);
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

      item = glte_GetCsvItem(NULL); /* 6-th item is period estimate indicator */
      iRc = glte_WriteInt(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid period estimate indicator:[%s], line %d\n", item, n);

      item = glte_GetCsvItem(NULL);            /* 7-th line item is birthDate */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                   "Invalid birthDate:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);           /* 8-th line item is birthPlace */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);            /* 9-th line item is deathDate */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                   "Invalid deathDate:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);          /* 10-th line item is deathPlace */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);          /* 11-th line item is occupation */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);                /* 12-th line item is note */
      iRc = glte_WriteText(item);                   /* never returns an error */

      item = glte_GetCsvItem(NULL);             /* 13-th line item is infoUrl */
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
