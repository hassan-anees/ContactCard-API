//VcardUtilies holds Vcardparser that has all the includes
#include "VCardParser.h"
#include "VCardUtilities.h"
#include "LinkedListAPI.h"
#include <ctype.h>


PropertyType getPropertyType(char *propName){
    /*Citation from here*/
    if(mystrcmp(propName, "BEGIN") == 0){
		return BEGIN;
	}else if(mystrcmp(propName, "END") == 0){
        return END;
    }else if(mystrcmp(propName, "SOURCE") == 0){
        return SOURCE;
    }else if(mystrcmp(propName, "KIND") == 0){
        return KIND;
    }else if(mystrcmp(propName, "XML") == 0){
        return XML;
    }else if(mystrcmp(propName, "FN") == 0){
        return FN;
    }else if(mystrcmp(propName, "N") == 0){
        return N;
    }else if(mystrcmp(propName, "NICKNAME") == 0){
        return NICKNAME;
    }else if(mystrcmp(propName, "PHOTO") == 0){
        return PHOTO;
    }else if(mystrcmp(propName, "BDAY") == 0){
        return BDAY;
    }else if(mystrcmp(propName, "ANNIVERSARY") == 0){
        return ANNIVERSARY;
    }else if(mystrcmp(propName, "GENDER") == 0){
        return GENDER;
    }else if(mystrcmp(propName, "ADR") == 0){
        return ADR;
    }else if(mystrcmp(propName, "TEL") == 0){
        return TEL;
    }else if(mystrcmp(propName, "EMAIL") == 0){
        return EMAIL;
    }else if(mystrcmp(propName, "IMPP") == 0){
        return END;
    }else if(mystrcmp(propName, "LANG") == 0){
        return LANG;
    }else if(mystrcmp(propName, "TZ") == 0){
        return TZ;
    }else if(mystrcmp(propName, "GEO") == 0){
        return GEO;
    }else if(mystrcmp(propName, "TITLE") == 0){
        return TITLE;
    }else if(mystrcmp(propName, "ROLE") == 0){
        return ROLE;
    }else if(mystrcmp(propName, "LOGO") == 0){
        return LOGO;
    }else if(mystrcmp(propName, "ORG") == 0){
        return ORG;
    }else if(mystrcmp(propName, "MEMBER") == 0){
        return MEMBER;
    }else if(mystrcmp(propName, "RELATED") == 0){
        return RELATED;
    }else if(mystrcmp(propName, "CATEGORIES") == 0){
        return CATEGORIES;
    }else if(mystrcmp(propName, "NOTE") == 0){
        return NOTE;
    }else if(mystrcmp(propName, "PRODID") == 0){
        return PRODID;
    }else if(mystrcmp(propName, "REV") == 0){
        return REV;
    }else if(mystrcmp(propName, "SOUND") == 0){
        return SOUND;
    }else if(mystrcmp(propName, "UID") == 0){
        return UID;
    }else if(mystrcmp(propName, "CLIENTPIDMAP") == 0){
        return CLIENTPIDMAP;
    }else if(mystrcmp(propName, "URL") == 0){
        return URL;
    }else if(mystrcmp(propName, "VERSION") == 0){
        return VERSION;
    }else if(mystrcmp(propName, "KEY") == 0){
        return KEY;
    }else if(mystrcmp(propName, "FBURL") == 0){
        return FBURL;
    }else if(mystrcmp(propName, "CALADRURI") == 0){
        return CALADRURI;
    }else if(mystrcmp(propName, "CALURI") == 0){
        return CALURI;
    }else{
        return INVALID;
    }
    /*till here*/
    return INVALID;

}

/*Citation from here
original from: https://code.woboq.org/userspace/glibc/string/strcmp.c.html
Source code from String library*/
int mystrcmp(const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do{
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      
      if (c1 == '\0')
        return c1 - c2;
    }while (toupper(c1) == toupper(c2));

  return c1 - c2;
}


bool checkWriteName(const char *fileName){
    printf("THE FILE NAME IS %s\n", fileName);
    int fileLen = strlen(fileName);
    // atleast 4 characters a.vcf
    if(fileLen >= 4){
        if(strcmp(fileName + fileLen - 4, ".vcf") == 0){
            printf("Has .vcf\n");
            return true;
        }else if(fileLen >=6 && strcmp(fileName + fileLen - 6, ".vcard") == 0){
            printf("has .vcard\n");
            return true;
        }else{
            return false;
        }
    }
    return false;
}

char* writeProperty(void* toBePrinted){
    char* tmpStr;
	Property* tempProperty;
	int len;
	
	if (toBePrinted == NULL){
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "It's NULL");
		return tmpStr;
	}
	
	tempProperty = (Property*)toBePrinted;
	
	char *stringValues = toString(tempProperty->values);
    //remove the end ;
    int lastCharValue = strlen(stringValues) - 1;
    if(stringValues[lastCharValue] == ';'){
        stringValues[lastCharValue] = '\0';
    }
    //printf(then end with a line terminator)
    strcat(stringValues, "\r\n\0");

    char *stringParameters = toString(tempProperty->parameters);
    //printf("The string para: %s\n", stringParameters);
    int lastCharPara = strlen(stringParameters);
    if(lastCharPara > 3){
        if(stringParameters[lastCharPara - 1] == ';'){
            stringParameters[lastCharPara - 1] = '\0';
        }

    }
   
    int paraListLen = tempProperty->parameters->length;

	len = strlen(tempProperty->group)+strlen(tempProperty->name)+ strlen(stringParameters) + strlen(stringValues) + 250;
	tmpStr = (char*)malloc(sizeof(char)*len);

    /*there might be 4 cases
    1. group name parameter value
    2. group name value
    3. name parameter value
    4. name value*/
    if(strlen(tempProperty->group) != 0 && paraListLen != 0){
        sprintf(tmpStr, "%s.%s;%s:%s", tempProperty->group ,tempProperty->name, stringParameters, stringValues);
    }else if(strlen(tempProperty->group) != 0 ){
        sprintf(tmpStr, "%s.%s:%s", tempProperty->group ,tempProperty->name,  stringValues);
    }else if(paraListLen != 0){ //aka not empty
        sprintf(tmpStr, "%s;%s:%s", tempProperty->name, stringParameters, stringValues);
    }else{
        sprintf(tmpStr, "%s:%s", tempProperty->name, stringValues);
    }

    //printf("The tmpStr is : %s", tmpStr);
   
	free(stringValues);
	free(stringParameters);
	
	return tmpStr;
}


//////////////////////////////////////////OLD functions
void removeCRLF(char *line){
 int i = 0;
 for(i = 0; i < strlen(line); i++){
  if (line[i] == '\r' || line[i] == '\n'){
   line[i] =  '\0';
  }
 }
}

/*Citation from here
original from: http://mirror.fsf.org/pmon2000/3.x/src/lib/libc/fgets.c
Source code from String library*/
char * rfgets(char *dst, int max, FILE *fp, bool *newLine, bool *carReturn)
{
	int c;
	char *p;

	/* get max bytes or upto a newline */

	for (p = dst, max--; max > 0; max--) {
		if ((c = fgetc (fp)) == EOF)
			break;
		*p++ = c;
		if (c == '\n' || c == '\r'){
			if(c == '\r'){
				*carReturn = true;
			}else{
				*carReturn = false;
			}
			
            if(fgetc(fp) == '\n'){
                *newLine = true;
            }else{
                *newLine = false;
            }
            break;
        }
	}
	*p = 0;
	if (p == dst || c == EOF)
		return NULL;
	return (p);
}

bool checkFileName(char *fileName){
	printf("THE FILE NAME IS %s\n", fileName);
    int fileLen = strlen(fileName);
    // atleast 4 characters a.vcf
    if(fileLen >= 4){
        if(strcmp(fileName + fileLen - 4, ".vcf") == 0){
            printf("Has .vcf\n");
            return true;
        }else if(fileLen >=6 && strcmp(fileName + fileLen - 6, ".vcard") == 0){
            printf("has .vcard\n");
            return true;
        }else{
            return false;
        }
    }
    return false;
}

void convertToUpper(char *string){
    for(int i = 0; i < strlen(string); i++){
        string[i] = toupper(string[i]);
    }
}

/*Fix this up so that it take account for case insensitive 
Begin, version, fn and end tags

Also you need to check that the version tag is immeadiately after the
begin tag*/
VCardErrorCode basicFileCheck(FILE *fp0, FILE *fp, FILE *fileP2){

    char string[1000];
    bool newLine = false;
    bool carReturn = false;
    bool oneAhead = false;

    //check firstLine
    /*Citation from here
    original from: http://mirror.fsf.org/pmon2000/3.x/src/lib/libc/fgets.c*/
    rfgets(string, 1000, fp0, &newLine, &carReturn);
    /*till here*/

    printf("\nBEFORE The string %s\n\n", string);
    printf("okok %lu\n",strlen(string));
    if(!newLine && !carReturn){
        return INV_PROP;
    }
    //making it uppercase
    char *builtString;
    builtString = lineMaker(fp, fileP2, &oneAhead);
    if(builtString == NULL){
        return OTHER_ERROR;
    }
    strcpy(string, builtString);
    convertToUpper(string);
    printf("\nThe string %s\n\n", string);
    if(strcmp(string, "BEGIN:VCARD") != 0){
        printf("it is different***\n");
        free(builtString);
        return INV_CARD;
    }

    free(builtString);

    //check second line
    /*Citation from here
    original from: http://mirror.fsf.org/pmon2000/3.x/src/lib/libc/fgets.c*/
    rfgets(string, 1000, fp0, &newLine, &carReturn);
    /*till here*/
    printf("\nBEFORE The string %s\n\n", string);
    printf("okok %lu\n",strlen(string));
    if(!newLine && !carReturn){
        return INV_PROP;
    }
    //making it uppercase
    builtString = lineMaker(fp, fileP2, &oneAhead);
    if(builtString == NULL){
        return OTHER_ERROR;
    }
    strcpy(string, builtString);
    convertToUpper(string);
    printf("\nThe string %s\n\n", string);
    if(strcmp(string, "VERSION:4.0") != 0){
        printf("it is different*----\n");
        free(builtString);
        return INV_CARD;
    }

    free(builtString);

    /*Citation from here
    original from: http://mirror.fsf.org/pmon2000/3.x/src/lib/libc/fgets.c*/
    while(rfgets(string, 1000, fp0, &newLine, &carReturn)!= NULL){
     /*till here*/

		//printf("string IS-> %s\n", string);
		if(strlen(string) < 2){ //aka an empty string
			printf("emptyline---\n");
			return INV_PROP;
		}
		if(string[0] == ':' || string[0] == ';' || string[0] == '.' || string[0] == '='){
			printf("No property name or group name---\n"); //what if group.: and no prop name?
			return INV_PROP;
		}
		
        //line should stop at \r, and so next has to be \n, if not then invalid
        if(!newLine && !carReturn){ //perhaps want this after you analyze the line itself
            return INV_PROP;
            break;
        }
    }

	//set the fp back to the start of the file 
	fseek(fp, 0, SEEK_SET);
    fseek(fileP2, 0, SEEK_SET);
	return OK;
    
}

char* lineMaker(FILE *fp, FILE *fileP2, bool* oneAhead){
    int finalLen = 0;
    char* finalString = malloc(sizeof(char)*1000);
    if(finalString == NULL){
        return NULL;
    }
    //making it empty
    strcpy(finalString, "\0");
    char string2[1000];
    char string3[1000];
    int count;
    if(!*oneAhead){
        fgets(string3, 1000, fileP2);
        removeCRLF(string3);
        *oneAhead = true;
    }
    while(fgets(string2, 1000, fp) != NULL){
        removeCRLF(string2);
        finalLen += strlen(string2);
        printf("The len --- %d", finalLen);

        strcpy(finalString, string2); //initial line that does not have space or tab
        while(fgets(string3, 1000, fileP2) != NULL && (string3[0]==' ' || string3[0]=='\t')){
            count = 0;
            fgets(string2, 1000, fp);
            for(int i = 0; i<5; i++){
                if(string3[i]==' '){
                    count++;
                }
            }
            int string3Len = strlen(string3);
            finalLen += string3Len;
            printf("The len --- %d", finalLen);
            if(count<4){
                for(int i = 0; i<string3Len-1; i++){
                    string2[i] = string3[i+1]; //removing the space
                }
            } else{
                for(int i = 0; i<string3Len-4; i++){
                    string2[i] = string3[i+4]; //removing the tab
                }
            }
            removeCRLF(string2); //dont you mean to remove crlf for 3?
            printf("The len --- %d", finalLen);
            strcat(finalString, string2);
        }
        break;
    }
    printf("\n\nString 1:%s\n\n", finalString);
    return finalString;
}

VCardErrorCode parseFile(FILE* fp, FILE* fileP2, Card** newCardObject ){
    char string[1000]; //holds the current line, never alters 
    //a string that gets chopped at delimeters Lang;Pref=1 -> Pref=1
    //This aids to find the next delimeter 
    char *reducedString; 
    char tokenString[1000]; // the string that gets tokenized and destroyed  
    char *token; //holds the token aka part of a string from tokenString
    char tempString[1000];
    char key[] = ":;="; //holds the possible delimeters
    char *delim; //holds the most recent delimeter
    int count;//holds the number of character till the next delimeter ex. FN: Simon, count is 2 

    //FLAGS
    bool colonFlag = false; // a flag to indicate if a colon has appeared in a line 
    bool fnAdded = false; // a flag for the first fnProperty
    bool parameterFlag = false;
    bool endFlag = false;
    bool fnFlag = false;
    bool oneAhead = false;
    bool emptyFlag = false;
    bool bdayFlag = false;
    bool anniFlag = false;
    //accessng one line
    while(1){
        if(feof(fp)){
            break;
        } else {
            char *finalString = lineMaker(fp, fileP2, &oneAhead);
            if(finalString == NULL){
                return OTHER_ERROR;
            }
            strcpy(string, finalString);
            free(finalString);
            //check the size of the line, if greater than 1000 then send away
            emptyFlag = false;
        }
        if(strlen(string)==0){
            break;
        }
        if(string[strlen(string)-1]==';'){
            emptyFlag = true;
        } else if(string[strlen(string)-1]==':' || string[strlen(string)-1]=='='){
            return INV_PROP;
        }
        //checking for folded lines
        printf("YOOO");
        if(!colonFlag){
            strcpy(key, ":;=");
        }
        removeCRLF(string);  
        strcpy(tokenString, string);
        reducedString = string; //reducedString points to the string
        delim =  strpbrk(string, key); //gets the first delim in the string
        printf("\n");
        //tokenizing a dummy string (tokenString)
        token = strtok(tokenString, key);
        Property *newProperty = NULL;
        Parameter *newPara = NULL; 
        printf("%s\n", token);
        strcpy(tempString, token);
        convertToUpper(tempString);
        if(!(strcmp(tempString, "BEGIN") == 0 || strcmp(tempString, "VERSION") == 0 || strcmp(tempString, "END") == 0)){
            printf("Token(%s)\n", token);
            //Also add Bday and Anni
            //Putting first FN value in the card
            if(strcmp(tempString, "FN") == 0 && !fnAdded){
                int countDelim = 0;
                for(int i =0; i < strlen(string); i++){
                    if(strchr(key,string[i])){
                        countDelim++;
                    }
                }
                if(countDelim > 1){
                    printf("too many values for fn\n");
                    return INV_PROP;
                }
                if(*delim==':'){
                    colonFlag = true;
                    strcpy((*newCardObject)->fn->name, token);
                    printf("Token IN FN(%s)\n", token);
                    token = strtok(NULL, key);
                    printf("Token IN FN(%s)\n", token);
                    //if no value for fn 
                    if(token == NULL){
                        return INV_PROP;
                    }
                    char *tempToken = malloc(sizeof(char)*strlen(token) + 100);
                    if(tempToken == NULL){
                        return OTHER_ERROR;
                    }
                    strcpy(tempToken, token);
                    insertBack((*newCardObject)->fn->values, tempToken);
                    fnAdded = true;
                }
                token = NULL;
            }else if(strcmp(tempString, "BDAY") == 0){ //PROBLEM, if there is no bday then it seg faults
				if(bdayFlag){
                    printf("multiple bdays");
                    return INV_CARD; //having more than one in a file is an error
                }
                int tFlag = 0;
				int zFlag = 0;
                printf("do u noooo de way\n");
				for(int i = 0; i <strlen(string); i++){
					if(string[i] == 'T'){
						tFlag = 1;
					}
					if(string[i] == 'Z'){
						zFlag = 1;
					}
				}
                (*newCardObject)->birthday = createDateTime();
                if((*newCardObject)->birthday == NULL){
                    return OTHER_ERROR;
                }
                if(zFlag){
					(*newCardObject)->birthday->UTC = true;
				}

                printf("token before if %s\n", token);
                token = strtok(NULL, ";:");
                //if there is no value infront bday
                if(token == NULL){
                    return INV_PROP;
                }
                printf("bday token: %s\n", token);
                //Only time in the string
                char tmpString[100];
                strcpy(tmpString, token);
                convertToUpper(tmpString);
                if(strstr(string, "VALUE") || strstr(string, "TEXT") || strstr(string, "=")){
                    if(strstr(tmpString, "VALUE=TEXT")){
                        printf("LOLOLLOL");
                        (*newCardObject)->birthday->isText = true;
                        token = strtok(NULL, ":");
                        if(token){
                            strcpy((*newCardObject)->birthday->text, token);
                        } else {
                            return INV_PROP;
                        }
                    } else {
                        return INV_PROP;
                    }
                } else {
					if(token[0] == 'T'){
						printf("There is a t in the start\n");
						char *tempToken = strtok(token, "TZ");
						printf("Date token: %s\n", tempToken);
                        bool hifenFlag = false;
                        for(int i = 0; i<strlen(tempToken); i++){
                            if(tempToken[i] == '-'){
                                hifenFlag = true;
                            }
                        }
                        if(hifenFlag){
                            char *timeOnlyToken = strtok(tempToken, "-"); 
                            if(strlen(timeOnlyToken) > 6){
                                return INV_PROP;
                            }
                            strcpy((*newCardObject)->birthday->time, timeOnlyToken);
                        }else{
                            if(strlen(tempToken) > 6){
                                return INV_PROP;
                            }
                            strcpy((*newCardObject)->birthday->time, tempToken);
                        }
					}else{
						if(tFlag){
							char *tempToken = strtok(token,":;TZ");
							printf("date part bday token: %s\n", tempToken);
                            if(strlen(tempToken)>8){
                                return INV_PROP;
                            }
							strcpy((*newCardObject)->birthday->date, tempToken);

							tempToken = strtok(NULL,":;TZ");
                            if(tempToken == NULL){
                                printf("no value after T\n");
                                return INV_PROP;
                            }
							printf("bday token: %s\n", tempToken);
                            bool hifenFlag = false;
                            for(int i = 0; i<strlen(tempToken); i++){
                                if(tempToken[i] == '-'){
                                    hifenFlag = true;
                                }
                            }
                            if(hifenFlag){
                                char *timeOnlyToken = strtok(tempToken, "-"); 
                                if(strlen(timeOnlyToken) > 6){
                                    return INV_PROP;
                                }
                                strcpy((*newCardObject)->birthday->time, timeOnlyToken);
                            }else{
                                if(strlen(tempToken) > 6){
                                    return INV_PROP;
                                }
                                strcpy((*newCardObject)->birthday->time, tempToken);
                            }
						}else{
							char *tempToken = strtok(token,":;TZ");
							printf("bday date token: %s\n", tempToken);
                            if(strlen(tempToken)>8){ //size is too much
                                return INV_PROP;
                            }
							strcpy((*newCardObject)->birthday->date, tempToken);
						}	
					}
				}
                bdayFlag = true;
				token = NULL; 
                //birthdayFlag = true;
            }else if(strcmp(tempString, "ANNIVERSARY") == 0){ //
                if(anniFlag){
                    printf("multiple anni's");
                    return INV_CARD; //having more than one in a file is an error
                }
                int tFlag = 0;
				int zFlag = 0;
                printf("do u noooo de way\n");
				for(int i = 0; i <strlen(string); i++){
					if(string[i] == 'T'){
						tFlag = 1;
					}
					if(string[i] == 'Z'){
						zFlag = 1;
					}
				}
                (*newCardObject)->anniversary = createDateTime();
                if((*newCardObject)->anniversary == NULL){
                    return OTHER_ERROR;
                }
                if(zFlag){
					(*newCardObject)->anniversary->UTC = true;
				}

                printf("token before if %s\n", token);
                token = strtok(NULL, ";:");
                //if there is no value infront bday
                if(token == NULL){
                    return INV_PROP;
                }
                printf("bday token: %s\n", token);
                //Only time in the string
                char tmpString[100];
                strcpy(tmpString, token);
                convertToUpper(tmpString);
                if(strstr(string, "VALUE") || strstr(string, "TEXT") || strstr(string, "=")){
                    if(strstr(tmpString, "VALUE=TEXT")){
                        printf("LOLOLLOL");
                        (*newCardObject)->anniversary->isText = true;
                        token = strtok(NULL, ":");
                        if(token){
                            strcpy((*newCardObject)->anniversary->text, token);
                        } else {
                            return INV_PROP;
                        }
                    } else {
                        return INV_PROP;
                    }
                } else {
					if(token[0] == 'T'){
						printf("There is a t in the start\n");
						char *tempToken = strtok(token, "TZ");
						printf("anni date token:%s\n", tempToken);
                        //checking if extra stuff
                        bool hifenFlag = false;
                        for(int i = 0; i<strlen(tempToken); i++){
                            if(tempToken[i] == '-'){
                                hifenFlag = true;
                            }
                        }
                        if(hifenFlag){
                            char *timeOnlyToken = strtok(tempToken, "-"); 
                            if(strlen(timeOnlyToken) > 6){
                                return INV_PROP;
                            }
                            strcpy((*newCardObject)->anniversary->time, timeOnlyToken);
                        }else{
                            if(strlen(tempToken) > 6){
                                return INV_PROP;
                            }
                            strcpy((*newCardObject)->anniversary->time, tempToken);
                        }
					}else{
						if(tFlag){
							char *tempToken = strtok(token,":;TZ");
							printf("date part anni token: %s\n", tempToken);
                            if(strlen(tempToken)>8){
                                return INV_PROP;
                            }
							strcpy((*newCardObject)->anniversary->date, tempToken);

							tempToken = strtok(NULL,":;TZ");
                            if(tempToken == NULL){
                                printf("no value after T in anni\n");
                                return INV_PROP;
                            }
							printf("anni token: %s\n", tempToken);
                            bool hifenFlag = false;
                            for(int i = 0; i<strlen(tempToken); i++){
                                if(tempToken[i] == '-'){
                                    hifenFlag = true;
                                }
                            }
                            if(hifenFlag){
                                char *timeOnlyToken = strtok(tempToken, "-"); 
                                if(strlen(timeOnlyToken) > 6){
                                    return INV_PROP;
                                }
                                strcpy((*newCardObject)->anniversary->time, timeOnlyToken);
                            }else{
                                if(strlen(tempToken) > 6){
                                    return INV_PROP;
                                }
                                strcpy((*newCardObject)->anniversary->time, tempToken);
                            }
						}else{
							char *tempToken = strtok(token,":;TZ");
							printf("anni date token: %s\n", tempToken);
                            if(strlen(tempToken)>8){ //size is too much
                                return INV_PROP;
                            }
							strcpy((*newCardObject)->anniversary->date, tempToken);
						}	
					}
				}
                anniFlag = true;
				token = NULL; 
            }else { //if theres no space in start of string
                colonFlag = false; //start of string, so no colon yet
                //finding the index of the delimeter 
                for(int i = 0; i<strlen(reducedString); i++){
                    if(strrchr(key, reducedString[i])){  
                        count = i;
                        break;
                    }
                }
                reducedString+=count; //ADR;Value = 1 -> turns to ;Value(Reduction process)
                newProperty=createProperty();
                if(newProperty == NULL){
                    deleteProperty(newProperty);
                    return OTHER_ERROR;
                }
                //////////Possible that the first token is a group. so then next token would be name
                if(delim){
                    printf("\nTHUS THE delim->[%c] token[%s]\n", *delim, token);
                    
                    if(strstr(token, ".") && strlen(newProperty->name)==0){ //if there is no property and a . in the first token
                        char *group = newProperty->group;
                        char *name = newProperty->name;
                        
                        int i = 0;
                        for(i = 0; i<strlen(token); i++){
                            group[i] = token[i];
                            if(token[i+1]=='.'){
                                i++;
                                if(strchr(key, token[i+1])){
                                    deleteProperty(newProperty);
                                    return INV_PROP;
                                } else{
                                    for(int x = 1; x<strlen(token)+i; x++){
                                        name[x-1]= token[i+x];
                                    }
                                }
                                break;
                            }
                        }
                        group[i]='\0'; //we can't do strlen when we dont its not null terminateed yet
                        name[strlen(group)]='\0';
                    } else{
                        strcpy(newProperty->name, token);
                    }
                }
            }
            // Parsing the rest of the line, tokenizing it 
            while(token != NULL){ 
                //maybe make this a function
                if(!delim){
                    break;
                }
                //printf("\nAHASGJHGSJ[%c]", string[strlen(string)-1]);
                for(int i = 0; i<strlen(reducedString); i++){
                    if(strrchr(key, reducedString[i])){
                        count = i+1; // +1 removes the delimeter
                        break;
                    }
                }
                reducedString+=count;  // cuts string till delimeter
                if(delim && ((strchr(":;=", *delim) && reducedString[0]=='=') || (*delim=='=' && strchr(";:= ", reducedString[0])))){
                    deleteProperty(newProperty);
                    return INV_PROP;
                }
                //printf("LOL%lu" , strlen(reducedString));
                while(reducedString[0]==';'){ //a list of 
                    if(!colonFlag && !(*delim==':')){
                        newPara = createParameter();
                        if(newPara == NULL){
                            deleteProperty(newProperty);
                            return OTHER_ERROR;
                        }
                        strcpy(newPara->name, "\0");
                        insertBack(newProperty->parameters, newPara);
                    } else {
                        char *tempToken = malloc(sizeof(char)*strlen(token) + 100);
                        if(tempToken == NULL){
                            deleteProperty(newProperty);
                            return OTHER_ERROR;
                        }
                        strcpy(tempToken, "\0");
                        insertBack(newProperty->values, tempToken);
                    }
                    reducedString++; //move over by one?
                }
                //getting the new token//////
                token = strtok(NULL, key);
                ////////////////////////
                //If delim exists, and the string is not empty
                if(parameterFlag && (*delim!='=' || strchr(";:", reducedString[0]))){
                    deleteProperty(newProperty);
                    return INV_PROP;
                }
                if(delim && strlen(reducedString)!=0){
                    printf("the delim->%c\n", *delim);
                    if(colonFlag && *delim==':'){
                        char *tempToken = malloc(sizeof(char)*strlen(token) + 600);
                        if(tempToken == NULL){
                            deleteProperty(newProperty);
                            return OTHER_ERROR;
                        }
                        strcpy(tempToken, (":"));
                        int tempLen = 0;
                        for(int i = 0; i<strlen(token); i++){
							tempLen++;
                            tempToken[i+1]=token[i];
                        }
                        tempToken[tempLen+1]='\0'; //+1 for the = sign
                        printf("UUUUUUUUU%s", tempToken);
                        char *prev = getFromBack(newProperty->values);
                        strcat(prev, tempToken);
                        free(tempToken);
                    } else if(*delim==':'){
                        char *tempToken = malloc(sizeof(char)*strlen(token) + 100);
                        if(tempToken == NULL){
                            deleteProperty(newProperty);
                            return OTHER_ERROR;
                        }
                        strcpy(tempToken,token);
                        printf("TEMP TOKEN: %s\n", tempToken);
                        insertBack(newProperty->values, tempToken);
                        colonFlag = true;
                    } else if(*delim==';'){
                        if(!colonFlag){
                            printf("INSERTING %s", token);
                            newPara = createParameter();
                            if(newPara == NULL){
                                deleteProperty(newProperty);
                                return OTHER_ERROR;
                            }
                            strcpy(newPara->name, token);
                            insertBack(newProperty->parameters, newPara);
                            parameterFlag = true;
                        } else {//yoyoyo 
                            char *tempToken = malloc(sizeof(char)*strlen(token) + 600);
                            if(tempToken == NULL){
                                deleteProperty(newProperty);
                                return OTHER_ERROR;
                            }
                            strcpy(tempToken, token);
                            insertBack(newProperty->values, tempToken);
                        }
                    } else if(!colonFlag && *delim=='='){
                        strcpy(newPara->value, token);
                        parameterFlag = false;
                    } else if(*delim=='='){
                        char *tempToken = malloc(sizeof(char)*strlen(token) + 600);
                        if(tempToken == NULL){
                            deleteProperty(newProperty);
                            return OTHER_ERROR;
                        }
                        strcpy(tempToken, ("="));
                        int tempLen = 0;
                        for(int i = 0; i<strlen(token); i++){
							tempLen++;
                            tempToken[i+1]=token[i];
                        }
                        tempToken[tempLen+1]='\0'; //+1 for the = sign
                        printf("UUUUUUUUU%s", tempToken);
                        char *prev = getFromBack(newProperty->values);
                        strcat(prev, tempToken);
                        parameterFlag = false;
                        free(tempToken);
                    }
                }
                //printf("YO%s and %d", reducedString, count);
                delim =  strpbrk(reducedString, key); // at end get next delimeter
                printf("Token(%s)\n", token);
            }
            if(!colonFlag){// accounts if no colon
                deleteProperty(newProperty);
                return INV_PROP;
            } else{
                if(emptyFlag){
                    char *tempToken = malloc(sizeof(char) * 10);
                    if(tempToken == NULL){
                        deleteProperty(newProperty);
                        return OTHER_ERROR;
                    }
                    strcpy(tempToken, "\0");
                    insertBack(newProperty->values, tempToken);
                }
                insertBack((*newCardObject)->optionalProperties, newProperty);
                printf("\n");
            }
        }
        convertToUpper(string);
        if(strcmp(string, "END:VCARD") == 0){
            endFlag = true;
        }
        if(strcmp(tempString, "FN") == 0){
            fnFlag = true;
        }
    }
    if(!fnFlag || !endFlag){
        return INV_CARD;
    }
    return OK;
}

Property* createJsonProperty(){
	Property *newProperty = (Property *)malloc(sizeof(Property));

    if(newProperty == NULL){
        return NULL;
	}

	//intialize list for each also mallocs for me
	newProperty->parameters = initializeList(&printParameter, &deleteParameter, &compareParameters);
	newProperty->values = NULL;

	//malloc for each name and group
	newProperty->name = malloc(sizeof(char)* 1000);
    if(newProperty->name == NULL){
        return NULL;
	}

	newProperty->group = malloc(sizeof(char)* 1000);
    if(newProperty->group == NULL){
        return NULL;
	}

	//initialize the name and group to be empty 
	strcpy(newProperty->name, "\0");
	strcpy(newProperty->group, "\0");


	return newProperty;
}


Property* createProperty(){
	Property *newProperty = (Property *)malloc(sizeof(Property));

    if(newProperty == NULL){
        return NULL;
	}

	//intialize list for each also mallocs for me
	newProperty->parameters = initializeList(&printParameter, &deleteParameter, &compareParameters);
	newProperty->values = initializeList(&printValue, &deleteValue, &compareValues);

	//malloc for each name and group
	newProperty->name = malloc(sizeof(char)* 1000);
    if(newProperty->name == NULL){
        return NULL;
	}

	newProperty->group = malloc(sizeof(char)* 1000);
    if(newProperty->group == NULL){
        return NULL;
	}

	//initialize the name and group to be empty 
	strcpy(newProperty->name, "\0");
	strcpy(newProperty->group, "\0");


	return newProperty;
}

Property* createWriteProperty(){
	Property *newProperty = (Property *)malloc(sizeof(Property));

    if(newProperty == NULL){
        return NULL;
	}

	//intialize list for each also mallocs for me
	newProperty->parameters = initializeList(&writeParameter, &deleteParameter, &compareParameters);
	newProperty->values = initializeList(&writeValue, &deleteValue, &compareValues);

	//malloc for each name and group
	newProperty->name = malloc(sizeof(char)* 1000);
    if(newProperty->name == NULL){
        return NULL;
	}

	newProperty->group = malloc(sizeof(char)* 1000);
    if(newProperty->group == NULL){
        return NULL;
	}

	//initialize the name and group to be empty 
	strcpy(newProperty->name, "\0");
	strcpy(newProperty->group, "\0");


	return newProperty;
}

char* writeValue(void* toBePrinted){
	char* tempString;
	char* returnString;
	int len;
	
	if (toBePrinted == NULL){
		tempString = malloc(sizeof(char)*20);
		strcpy(tempString, "It's NULL");
		return tempString;
	}
	
	tempString = (char*)toBePrinted;
	
	len = strlen(tempString) + 20;
	returnString = (char*)malloc(sizeof(char)*len);
	
	sprintf(returnString, "%s;", tempString);
	
	return returnString;
}

char* writeParameter(void* toBePrinted){
	char* tmpStr;
	Parameter* tmpName;
	int len;
	
	if (toBePrinted == NULL){
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "It's NULL");
		return tmpStr;
	}
	
	tmpName = (Parameter*)toBePrinted; //casting it so type parameter

	len = strlen(tmpName->name)+strlen(tmpName->value) + 50;
	tmpStr = (char*)malloc(sizeof(char)*len);
	sprintf(tmpStr, "%s=%s;", tmpName->name, tmpName->value);
	//freeing the string values
	return tmpStr;
	
}

Parameter* createParameter(){
	Parameter *newParameter = (Parameter *)malloc(sizeof(Parameter) + 1000 *sizeof(char));

    if(newParameter == NULL){
        return NULL;
	}
	//initialize the name and group to be empty 
	strcpy(newParameter->name, "\0");
	strcpy(newParameter->value, "\0");


	return newParameter;
}



DateTime* createDateTime(){
	DateTime *newDateTime = (DateTime *)malloc(sizeof(DateTime) + 1000 *sizeof(char));

    if(newDateTime == NULL){
        return NULL;
	}

    newDateTime->UTC = false;
    newDateTime->isText = false;
    strcpy(newDateTime->date, "\0");
    strcpy(newDateTime->time, "\0");
    //this one is a flexible array which is done in the inital malloc above ... + ... 
    strcpy(newDateTime->text, "\0");

	return newDateTime;
}
