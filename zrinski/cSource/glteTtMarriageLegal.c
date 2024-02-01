/* GenealogyLite "TableTranslation" program for table: [marriageLegal]
   ====================================================================
   See the preamble to glteTtWoman.c program source code and
   the #define LABELS_LINE statement below.

 */
#define PGM_LAST_EDIT_DATE "2024.028"
#define LABELS_LINE "wife,husband,pEst,startDate,startPlace,endDate,note,infoUrl\n"

#include "glteScullions.h"
#define IN_LINE_MAX  1024           /* maximum length of input text file line */
static const char *progName;    /* for error logging by this source file only */
/* ========================================================================== */
int main (int argc,
          const char *argv[],
          const char *envr[]) {
   int i, n, iRc, nSpouse;
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
                                     "unexpegted labels line: %s", inLn);

   pa = fgets(inLn, IN_LINE_MAX, stdin);                   /* first table row */
   n = 1;
   nSpouse = 0;                                /* was at least one specified? */
   while (pa) {
      pnl = strrchr(inLn, '\n');                    /* for simpler parsing... */
      if (pnl) *pnl = '\0';        /* ...strip ending newline from input line */
/*    fprintf(stderr, "line %3d:[%s]\n", n, inLn); */

      item = glte_GetCsvItem(inLn);            /* first line item is wife */
/*    fprintf(stderr, "wife:[%s]\n", item); */
      iRc = glte_WriteId(item);
      if (iRc > 0) glte_Abort(progName, __LINE__, "Invalid wife id:[%s], line %d\n", n);
      if (iRc == 0) {
         i = atoi(item);
         if (i % 2) glte_Abort(progName, __LINE__,
                              "Wife id:[%s] not an even number, line %d\n", item, n);
         nSpouse++;                                           /* spouse given */
         }

      item = glte_GetCsvItem(NULL);           /* second  line item is husband */
/*    fprintf(stderr, "husband:[%s]\n", item); */
      iRc = glte_WriteId(item);
      if (iRc > 0) glte_Abort(progName, __LINE__, "Invalid husband id:[%s], line %d\n", item, n);
      if (iRc == 0) {
         i = atoi(item);
         if ((i % 2) == 0) glte_Abort(progName, __LINE__,
                                      "Husband id:[%s] not an odd number, line %d %d\n", item, n, i);
         nSpouse++;                                           /* spouse given */
         }

      item = glte_GetCsvItem(NULL); /* third item is period estimate indicator */
/*    fprintf(stderr, "pEst:[%s]\n", item); */
      iRc = glte_WriteInt(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid period estimate indicator:[%s], line %d\n", item, n);

      item = glte_GetCsvItem(NULL);            /* 4th line item is start date */
/*    fprintf(stderr, "startDate:[%s]\n", item); */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid start date:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);          /* 5-th line item is start place */
/*    fprintf(stderr, "startPlace:[%s]\n", item); */
      iRc = glte_WriteText(item);          /* this one never returns an error */

      item = glte_GetCsvItem(NULL);             /* 6-th line item is end date */
/*    fprintf(stderr, "endDate:[%s]\n", item); */
      iRc = glte_WriteYearDay(item);
      if (iRc > 0) glte_Abort(progName, __LINE__,
                              "Invalid end date:[%s], line %d, error code:%d\n", item, n, iRc);

      item = glte_GetCsvItem(NULL);                 /* 6-th line item is note */
/*    fprintf(stderr, "note:[%s]\n", item); */
      iRc = glte_WriteText(item);          /* this one never returns an error */

      item = glte_GetCsvItem(NULL);               /* 7-th line item is URL(s) */
/*    fprintf(stderr, "URL's:[%s]\n", item); */
      iRc = glte_WriteText(item);          /* this one never returns an error */

      if (nSpouse == 0) glte_Abort(progName, __LINE__,
                                          "Neither spouse given, line %d\n", n);

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
