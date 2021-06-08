#ifndef _CARDUtilities_H
#define _CARDUtilities_H

#include "VCardParser.h"
#include <ctype.h>

typedef enum propType {BEGIN, END, SOURCE, KIND, XML, FN, N, NICKNAME, PHOTO, BDAY, ANNIVERSARY, GENDER, ADR, TEL, EMAIL, IMPP, LANG, TZ, GEO, TITLE, ROLE, LOGO, ORG, MEMBER, RELATED, CATEGORIES, NOTE, PRODID, REV, SOUND, UID, CLIENTPIDMAP, URL, VERSION, KEY, FBURL, CALADRURI, CALURI, INVALID} PropertyType;

Property* createJsonProperty();

PropertyType getPropertyType(char *propName);

Property* createWriteProperty();

char* writeValue(void* toBePrinted);

char* writeParameter(void* toBePrinted);

int mystrcmp(const char *p1, const char *p2);

void removeCRLF(char *line);

VCardErrorCode basicFileCheck(FILE* fp0, FILE *fp, FILE *fileP2);

VCardErrorCode parseFile(FILE *fp, FILE* fileP2, Card** newCardObject);

Property* createProperty();

Parameter* createParameter();

DateTime* createDateTime();

char * rfgets(char *dst, int max, FILE *fp, bool *newLine, bool *carReturn);

void convertToUpper(char *string);

bool checkFileName(char *fileName);

bool checkWriteName(const char *fileName);

char* lineMaker(FILE *fp, FILE *fileP2, bool* oneAhead);

char* writeProperty(void* toBePrinted);

char* cardToJSON(char *filename);

char* getSummaryFromFile(char *filename);

char *cookieLife();



#endif
