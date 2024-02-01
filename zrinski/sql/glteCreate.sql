CREATE TABLE woman (
   id integer primary key,
   mother integer references woman (id),
   father integer references man (id),
   name string,
   surname string,
   maidenSurname string,
   pEst integer,
   birthYear integer,
   birthDayNbr integer,
   bitrhPlace string,
   deathYear integer,
   deathDayNbr integer,
   deathPlace string,
   occupation string,
   note text,
   infoUrl text
   );

CREATE TABLE man (
   id integer primary key,
   mother integer references woman (id),
   father integer references man (id),
   name string,
   surname string,
   pEst integer,
   birthYear integer,
   birthDayNbr integer,
   bitrhPlace string,
   deathYear integer,
   deathDayNbr integer,
   deathPlace string,
   occupation string,
   note text,
   infoUrl text
   );

CREATE TABLE marriageLegal (
   wife integer references woman (id),
   husband integer references man (id),
   pEst integer,
   startYear integer,
   startDayNbr integer,
   startPlace string,
   endYear integer,
   endDayNbr integer,
   note text,
   infoUrl text
   );

CREATE TABLE marriageCommon (
   wife integer references woman (id),
   husband integer references man (id),
   pEst integer,
   startYear integer,
   startDayNbr integer,
   endYear integer,
   endDayNbr integer,
   note text,
   infoUrl text
   );
