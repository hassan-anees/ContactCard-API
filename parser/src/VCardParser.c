
#include "VCardParser.h"
#include "VCardUtilities.h"

char* getSummaryFromFile(char *filename){
	if(filename == NULL){
		return NULL;
	}

	char *tmpStr = NULL;
	int len = 0;
	int numOfAdditional = 0;
	char name[200];
	strcpy(name, "\0");

	len += strlen(filename);
	Card *newCard = NULL;
	VCardErrorCode errType = createCard(filename, &newCard);

	if(errType != OK){
		return NULL;
	}
	//checking validate card for further errors
	VCardErrorCode valError = validateCard(newCard);
	if(valError != OK){
		return NULL;
	}
	//char *cardStr = printCard(newCard);
	//checking validate card for further errors

	//VCardErrorCode valError = validateCard(newCard);

	//char *valStr = printError(valError);
	//getting the number of additional properties
	numOfAdditional = getLength(newCard->optionalProperties);
	void *valName = getFromFront(newCard->fn->values);
	tmpStr = malloc(sizeof(char) * 1000);
	sprintf(tmpStr, "{\"filename\":\"%s\",\"name\":\"%s\",\"additionalValues\":%d}", filename, (char *) valName, numOfAdditional);
	//printf("%s\n", tmpStr);
	deleteCard(newCard);

	//printf("%s\n**********cardtoJSON TESTSTTSTS in function\n", valStr);
	//printf("%skdjskldjskdjksjdklsjkdlsj\n\n", cardStr);
	return tmpStr;
}

char* cardToJSON(char *filename){
	//printf("YOOO\n");
	char *tmpStr = NULL;
	int len = 0;
	if(filename == NULL){
		return NULL;
	}

	Card *newCard = NULL;
	VCardErrorCode errType = createCard(filename, &newCard);
	if(errType != OK){
		return NULL;
	}
	//checking validate card for further errors
	VCardErrorCode valError = validateCard(newCard);
	if(valError != OK){
		return NULL;
	}
	//char *cardStr = printCard(newCard);
	//checking validate card for further errors
	//char *valStr = printError(valError);

	//printf("%s\n**********cardtoJSON TESTSTTSTS in function\n", valStr);
	//printf("%skdjskldjskdjksjdklsjkdlsj\n\n", cardStr);

	//iterator to get potential length of the cardJsonstring
	ListIterator countIter = createIterator(newCard->optionalProperties);
	void *element =  NULL;
	int propLen = 0;
	while((element = nextElement(&countIter)) != NULL){
		Property *objProp = (Property *) element;
		char *propStr = propToJSON(objProp);
		propLen = strlen(propStr);
		len += propLen + 2;
		//printf("%s\n", propStr);
		free(propStr);
	}


	ListIterator optionalIter = createIterator(newCard->optionalProperties);
	element =  NULL;
	//I should probably
	char tempString[1000];
	strcpy(tempString, "\0");
	char wholeString[len];
	strcpy(wholeString, "\0");

	//putting the fn prop to the string
	char *fnProp = propToJSON(newCard->fn);
	sprintf(tempString, "[%s],", fnProp);
	strcat(wholeString, tempString);
	free(fnProp);


	while((element = nextElement(&optionalIter)) != NULL){
		Property *objProp = (Property *) element;
		char *propStr = propToJSON(objProp);
		propLen = strlen(propStr);
		sprintf(tempString,"[%s],", propStr);
		strcat(wholeString, tempString);

		printf("%s\n", propStr);
		free(propStr);
	}
	int finalLen = strlen(wholeString);
	wholeString[finalLen -1] = '\0';
	//printf("\n%s\n", wholeString);
	tmpStr = malloc(sizeof(char) * finalLen + 200);
	sprintf(tmpStr,"[%s]", wholeString);
	printf("%s\n", tmpStr);
	deleteCard(newCard);

	return tmpStr;
}

char* strListToJSON(const List* strList){
	char *tmpStr;
	if(strList == NULL){
		printf("nullyo\n");
		return NULL;
	}
	List *tempList = (List *)strList;
	if(getLength(tempList) == 0){
		printf("its empty");
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "[]");
		return tmpStr;
	}

	ListIterator iter = createIterator(tempList);

	char *testStr = toString(tempList);
	//printf("THE testString and size %d: %s\n",strlen(testStr) ,testStr);
	int len = strlen(testStr);
	free(testStr);
	//char* str;
	//printf("\nOYYEYEYEYEYE");

	char valueLine[1000];
	char blahString[500];
	strcpy(valueLine, "");
	void *elem = NULL;
	while((elem = nextElement(&iter)) != NULL){
		sprintf(blahString,"\"%s\",", (char *)elem);
		strcat(valueLine, blahString);
	}

	valueLine[strlen(valueLine) - 1] = '\0';
	//printf("\nVALUE STRIING:%s\n", valueLine);
	tmpStr = malloc(sizeof(char) * len +100);
	sprintf(tmpStr, "[%s]", valueLine);
	//printf("\nBLADHD:%s\n", tmpStr);

	/*str = (char*)malloc(sizeof(char));
	strcpy(str, "");

	void* elem;
	while((elem = nextElement(&iter)) != NULL){

	}*/

	return tmpStr;
}

//maybe fist COPY str, dont modify it
List* JSONtoStrList(const char* str){
	if(str == NULL){
		printf("jsontostrlist str is null\n");
		return NULL;
	}
	char *originalString = (char *)str;
	int ogLen = strlen(originalString);
	//char *altString = malloc(sizeof(char) * lenTemp + 100);
	char altString[500];
	strcpy(altString, originalString);
	//printf("the JSON string issss: %s\n", altString);
	List *jsonList = initializeList(&printValue, &deleteValue, &compareValues);
	int count = 0;

	//check if first value is empty
	if(originalString[0] == '[' && originalString[1] == '\"' && originalString[2] == '\"' ){
		//printf("\tEMPTY STRING\n");
		char *tempValue = malloc(sizeof(char) * ogLen + 10);
		strcpy(tempValue, "\0");
		insertBack(jsonList, tempValue);
	}
	char * token = strtok(altString, "[\"]");
	while( token != NULL ) {
		//printf( " %s\n", token );
		if(strcmp(token, ",") != 0){
		//printf("\t%s\n", token);
		char *tempValue = malloc(sizeof(char) * ogLen + 10);
		strcpy(tempValue, token);
		insertBack(jsonList, tempValue);
		count = 0;
		}else{
		count++;
		if(count > 1){
			char *tempValue = malloc(sizeof(char) * ogLen + 10);
			strcpy(tempValue, "\0");
			insertBack(jsonList, tempValue);
			//printf("\tempty string\n");
			}
		}

		token = strtok(NULL, "[\"]");
   }
	//check if last value is empty
	if(originalString[ogLen - 1] == ']' && originalString[ogLen - 2] == '\"' && originalString[ogLen - 3] == '\"' ){
		//printf("\tend EMPTY STRING\n");
		char *tempValue = malloc(sizeof(char) * ogLen + 10);
		strcpy(tempValue, "\0");
		insertBack(jsonList, tempValue);
	}

	char *listJson = toString(jsonList);
	//printf("\nJSONLIST:%s\n\n", listJson);
	free(listJson);

	return jsonList;
}

//MUST NOT modify the string
char* propToJSON(const Property* prop){
	//printf("YOOO\n");
	char *tmpStr = NULL;
	if(prop == NULL){
		return NULL;
	}

	Property *tempProp = (Property *)prop;
	char *myPropPrint = printProperty(tempProp);
	//printf("%s\n\n", myPropPrint);
	int len = strlen(myPropPrint);

	//printf("The Group:(%s)\n", tempProp->group);
	//printf("The Name:(%s)\n", tempProp->name);
	char *jsonValues = strListToJSON(tempProp->values);
	//printf("the json string(%s)\n", jsonValues);

	tmpStr = malloc(sizeof(char) * len + 300);
	sprintf(tmpStr, "{\"group\":\"%s\",\"name\":\"%s\",\"values\":%s}",tempProp->group, tempProp->name, jsonValues);
	//printf("%s\n",tmpStr);

	free(jsonValues);
	free(myPropPrint);
	return tmpStr;
}

Property* JSONtoProp(const char* str){
	if(str == NULL){
		printf("jsontoprop str is  null\n");
		return NULL;
	}
	char *originalStr = (char *)str;
	int myLen = strlen(originalStr);
	char copyStr[1000];
	strcpy(copyStr, originalStr);
	//printf("JSONtoProp copyStr %s\n", copyStr);

	//FOLLOWING IS to obtain group and name from the jsonString
	char group[200];
	char name[200];
    char * token = strtok(copyStr, "[\"]");
	while( token != NULL ) {
		//printf( " (%s)\n", token );
		if(strcmp(token, "group") == 0){
	    	token = strtok(NULL, "[\"]");
	    	//printf( "token in group if (%s)\n", token );
	    	if(token != NULL && strcmp(token, ":") == 0){
	    	    strcpy(group, token);
	    	    token = strtok(NULL, "[\"]");
	    	    if(strcmp(token, ",") == 0){
	    	        strcpy(group, "\0");
	    	    }else{
	    	        strcpy(group, token);
	    	    }
	    	}
		}else if(strcmp(token, "name") == 0){
	    	token = strtok(NULL, "[\"]");
	    	//printf( "token in name if (%s)\n", token );
	    	if(token != NULL && strcmp(token, ":") == 0){
	    	    strcpy(name, token);
	    	    token = strtok(NULL, "[\"]");
	    	    if(strcmp(token, ",") == 0){
	    	        strcpy(name, "\0");
					printf("empty name is json to prop\n");
					return NULL;
	    	    }else{
	    	        strcpy(name, token);
	    	    }
	    	}
		}else{
		    token = strtok(NULL, "{[\"]}");
		}
   }
	//printf("THE group->(%s)\n", group);
	//printf("THE name->(%s)\n", name);

	//NOW getting the values
	char partString[400];
	char valueString[1000];
	int len = 0;
	char delim1 = ',';
	char delim2 = ':';
	int valueFlag = 0;
	for (int i = 0; i < myLen; i++){
		//printf("(%c)\n", string[i]);
		if(originalStr[i] == delim1 || originalStr[i] == delim2){
			//printf("part string: %s\n", partString);
			//clear the string
			if(strcmp(partString, "\"values\"") == 0){
				len = 0;
				strcpy(partString, "\0");
				delim1 = '}';
				delim2 = '}';
				valueFlag = 1;
			}
			if(valueFlag == 0){
				len = 0;
				strcpy(partString, "\0");
			}else{
				strcpy(valueString, partString);
			}

		}else{
			partString[len] = originalStr[i];
			partString[len+1] = '\0';
			len++;
		}

	}

	//CONSTRUCTING THE PROPERTY
	Property *jsonProperty = createJsonProperty();
	//entering the group and name
	strcpy(jsonProperty->group, group);
	strcpy(jsonProperty->name, name);

	//getting the LIST
	List *testJsonList = JSONtoStrList(valueString);
	jsonProperty->values = testJsonList;

	/*char *testJsonProp = printProperty(jsonProperty);
	printf("inside func propJson: %s\n", testJsonProp);
	free(testJsonProp);*/

	//deleteProperty(jsonProperty);
	//char *strJsonList = toString(testJsonList);
	//printf("the strJsonList is %s\n", strJsonList);

	return jsonProperty;
}

char* dtToJSON(const DateTime* prop){
	//printf("in dttoJSON prop");
	if(prop == NULL){
		printf("its dt is null\n");
		return NULL;
	}
	char *dateJson = NULL;
	DateTime *tempDate = (DateTime *)prop;
	char *myDateStr = printDate(tempDate);
	int len = strlen(myDateStr);
	free(myDateStr);
	//printf("lol date %s\n", myDateStr);

	char isTextStr[50];
	char isUTCStr[50];
	if(tempDate->isText){
		strcpy(isTextStr, "true");
	}else{
		strcpy(isTextStr, "false");
	}

	if(tempDate->UTC){
		strcpy(isUTCStr, "true");
	}else{
		strcpy(isUTCStr, "false");
	}

	dateJson = malloc(sizeof(char) * len + 100);
	sprintf(dateJson,"{\"isText\":%s,\"date\":\"%s\",\"time\":\"%s\",\"text\":\"%s\",\"isUTC\":%s}",isTextStr, tempDate->date, tempDate->time, tempDate->text, isUTCStr);
	//printf("the JSON DATE:%s\n",dateJson);

	return dateJson;
}

DateTime* JSONtoDT(const char* str){
	if(str == NULL){
		return NULL;
	}

	char copyStr[1000];
	strcpy(copyStr, str);
	char dtTime[50];
	char dtDate[50];
	char dtText[900];
	bool isText = false;
	bool isUTC = false;
    char * token = strtok(copyStr, "\"");
	//printf("hello token %s\n", token);

	while( token != NULL ) {
		//printf( " (%s)\n", token );
		if(strcmp(token, "date") == 0){
	    	token = strtok(NULL, "[\"]");
	    	//printf( "token in group if (%s)\n", token );
	    	if(token != NULL && strcmp(token, ":") == 0){
	    	    strcpy(dtDate, token);
	    	    token = strtok(NULL, "[\"]");
	    	    if(strcmp(token, ",") == 0){
	    	        strcpy(dtDate, "\0");
	    	    }else{
	    	        strcpy(dtDate, token);
	    	    }
	    	}
		}else if(strcmp(token, "time") == 0){
	    	token = strtok(NULL, "[\"]");
	    	//printf( "token in name if (%s)\n", token );
	    	if(token != NULL && strcmp(token, ":") == 0){
	    	    strcpy(dtTime, token);
	    	    token = strtok(NULL, "[\"]");
	    	    if(strcmp(token, ",") == 0){
	    	        strcpy(dtTime, "\0");
					//printf("empty name is json to prop\n");
	    	    }else{
	    	        strcpy(dtTime, token);
	    	    }
	    	}
		}else if(strcmp(token, "text") == 0){
	    	token = strtok(NULL, "[\"]");
	    	//printf( "token in name if (%s)\n", token );
	    	if(token != NULL && strcmp(token, ":") == 0){
	    	    strcpy(dtText, token);
	    	    token = strtok(NULL, "[\"]");
	    	    if(strcmp(token, ",") == 0){
	    	        strcpy(dtText, "\0");
					//printf("empty name is json to prop\n");
	    	    }else{
	    	        strcpy(dtText, token);
	    	    }
	    	}
		}else if(strcmp(token, "isText") == 0){
	    	token = strtok(NULL, ":,");
	    	//printf( "token in istext is (%s)\n", token );
	    	if(strcmp(token, "false") == 0){
				isText = false;
			}else if(strcmp(token, "true") == 0){
				isText = true;
			}
		}else if(strcmp(token, "isUTC") == 0){
	    	token = strtok(NULL, "{:,}");
	    	//printf( "token in isUTC is (%s)\n", token );
	    	if(strcmp(token, "false") == 0){
				isUTC = false;
			}else if(strcmp(token, "true") == 0){
				isUTC = true;
			}
		}else{
		    token = strtok(NULL, "{[\"]}");
		}
   }

	//Creating the DATETIME
	//printf("Date(%s), time(%s), text(%s), isText(%d), isUTC(%d)\n", dtDate, dtTime, dtText, isText, isUTC);
	DateTime *newDT = createDateTime();
	strcpy(newDT->date, dtDate);
	strcpy(newDT->time, dtTime);
	strcpy(newDT->text, dtText);
	if(isText == false){
		newDT->isText = false;
	}else{
		newDT->isText = true;
	}

	if(isUTC == false){
		newDT->UTC = false;
	}else{
		newDT->UTC = true;
	}

	return newDT;
}

Card* JSONtoCard(const char* str){
	if(str == NULL){
		printf("str null json to card\n");
		return NULL;
	}

	//creating the CARD
	Card * newCard = malloc(sizeof(Card));
	newCard->fn = createProperty();
	newCard->optionalProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	newCard->anniversary = NULL;
	newCard->birthday = NULL;

	char copyStr[1000];
	strcpy(copyStr, str);
	printf("copy (%s)\n", copyStr);

	//DO WE NEED TO CHECK IF GROUP WILL BE THERE?
	char *token = strtok(copyStr, "{:\"}");
	while(token != NULL){
		printf("token (%s)\n",token);
		token = strtok(NULL, "{:\"}");
		if(token!= NULL){
			char *tempValue = malloc(sizeof(char) * strlen(token) +100);
			strcpy(tempValue, token);
			insertBack(newCard->fn->values, tempValue);
		}
	}

	strcpy(newCard->fn->name, "FN");
	//DO we need to make a parameter?

	return newCard;
}

void addProperty(Card* card, const Property* toBeAdded){
	if(card == NULL){
		printf("card not created\n");
		return;
	}

	if(toBeAdded == NULL){
		printf("prop is null\n");
		return;
	}

	Property *tempProp = (Property *) toBeAdded;
	insertBack(card->optionalProperties, tempProp);

	return;
}



VCardErrorCode validateCard(const Card* obj){
	printf("IM in VALIDATE\n");
	//BASIC NULL
	if(obj == NULL){
		printf("\n\nInvalid card sdjksjds\n\n");
		return INV_CARD;
	}
	if(obj->fn == NULL){
		printf("Its null fn\n");
		return INV_CARD;
	}
	//CHECK FN name is actually FN
	if(mystrcmp(obj->fn->name, "FN") != 0){
		printf("it is not FN");
		return INV_CARD;
	}
	//check the value exists
	if(getLength(obj->fn->values) != 1){
		printf("too many fn values");
		return INV_CARD;
	}

	//JSON STUFF-------------------------____REMOVE LATER
		char *myJsonProp = propToJSON(obj->fn);
		printf("my jsonProp: %s\n", myJsonProp);
		Property *jsonProperty = JSONtoProp(myJsonProp);

		//char *strJsonProperty = printProperty(jsonProperty);
		//printf("the propJson: %s\n", strJsonProperty);
		//free(strJsonProperty);
		deleteProperty(jsonProperty);
		free(myJsonProp);
	////////////////////////////////////////////////////

	//list of optional properties cannot be null
	if(obj->optionalProperties == NULL){
		printf("optional properties is NULL\n");
		return INV_CARD;
	}

	if(obj->birthday != NULL){ //if bday exists check specifc parts of it
		//both cannot occur at same time
		//printf("DATE(%s), TIME(%s)\n", obj->birthday->date, obj->birthday->time);
		if(obj->birthday->isText && obj->birthday->UTC){
			printf("both utc and text on, which is invalid\n");
			return INV_DT;
		}

		int bdayDateLen = strlen(obj->birthday->date);
		if(bdayDateLen > 0){
			//checking thet each char is a number or hifen
			for(int i = 0; i < bdayDateLen; i++){
				if((obj->birthday->date[i] != '-') && (!isdigit(obj->birthday->date[i]))){
					printf("is not a digit in date\n");
					return INV_DT;
				}
				/*if(obj->birthday->date[i] == '-' || isdigit(obj->birthday->date[i])){
					printf("is digit in date\n");
				}else{
					printf("its not a digit\n");
					return INV_DT;
				}*/
			}
			if(obj->birthday->date[bdayDateLen - 1] == '-'){
				printf("hell hifen\n");
				return INV_DT;
			}
		}

		int bdayTimeLen = strlen(obj->birthday->time);
		if(bdayTimeLen > 0){
			//printf("The thing \n");
			for(int i = 0; i < bdayTimeLen; i++){
				if( (obj->birthday->time[i] != '-') && (!isdigit(obj->birthday->time[i]))){
					printf("is not digit in time\n");
					return INV_DT;
				}
				/*if(obj->birthday->time[i] == '-' || isdigit(obj->birthday->time[i])){
					printf("is digit in time\n");
				}else{
					printf("its not a digit\n");
					return INV_DT;
				}*/
			}

			if(obj->birthday->time[bdayTimeLen - 1] == '-'){
				printf("hell hifen\n");
				return INV_DT;
			}
		}
		//if there is text and no flag for it
		if(strlen(obj->birthday->text) > 0 && !obj->birthday->isText){
			return INV_DT;
		}
		//if isText on then both date and text need to be empty
		if((bdayTimeLen > 0 || bdayDateLen > 0) && obj->birthday->isText){
			printf("cant have date and time while is text bday\n");
			return INV_DT;
		}
	}

	if(obj->anniversary != NULL){ //if bday exists check specifc parts of it
		//both cannot occur at same time
		if(obj->anniversary->isText && obj->anniversary->UTC){
			printf("both utc and text on, which is invalid\n");
			return INV_DT;
		}
		int anniDateLen = strlen(obj->anniversary->date);
		if(anniDateLen > 0){
			//printf("The thing \n");
			for(int i = 0; i < anniDateLen; i++){
				if( (obj->anniversary->date[i] != '-') && (!isdigit(obj->anniversary->date[i]))){
					printf("is not digit in anni date \n");
					return INV_DT;
				}
				/*if(obj->anniversary->date[i] == '-' || isdigit(obj->anniversary->date[i])){
					printf("is digit in anni date \n");
				}else{
					printf("its not a digit\n");
					return INV_DT;
				}*/
			}
			if(obj->anniversary->date[anniDateLen - 1] == '-'){
				printf("hell hifen\n");
				return INV_DT;
			}
		}

		int anniTimeLen = strlen(obj->anniversary->time);
		if(anniTimeLen > 0){
			for(int i = 0; i < anniTimeLen; i++){
				if((obj->anniversary->time[i] != '-') && (!isdigit(obj->anniversary->time[i]))){
					printf("is digit in anni time\n");
					return INV_DT;
				}
				/*if(obj->anniversary->time[i] == '-' || isdigit(obj->anniversary->time[i])){
					printf("is digit in anni time\n");
				}else{
					printf("its not a digit\n");
					return INV_DT;
				}*/
			}
		}
		//if there is text and no flag for it
		if(strlen(obj->anniversary->text) > 0 && !obj->anniversary->isText){
			return INV_DT;
		}

		if((anniTimeLen > 0 || anniDateLen > 0) && obj->anniversary->isText){
			printf("cant have date and time while is text anni\n");
			return INV_DT;
		}
	}

	/////////////////DATE TIME JSON STUFF
	char *bdayStr = dtToJSON(obj->birthday);
	printf("Bday str:%s\n\n", bdayStr);
	//DateTime *tempDate = JSONtoDT(bdayStr);

	char *anniStr = dtToJSON(obj->anniversary);
	printf("anni str:%s\n\n", anniStr);
	free(anniStr);
	//DateTime *tempAnni = JSONtoDT(bdayStr);
	//char *tempDateStr = printDate(tempDate);
	//printf("the tempDate %s", tempDateStr);
	//free(tempDateStr);
	//deleteDate(tempDate);
	free(bdayStr);
	//

	//char *anniStr = dtToJSON(obj->anniversary);
	//printf("Anni str:%s\n", anniStr);
	//free(anniStr);



	ListIterator optionalIter = createIterator(obj->optionalProperties);
	void *element =  NULL;

	bool kindDenyMember = false;
	bool nOccurs = false;
	bool genderOccurs = false;
	bool proIdOccurs = false;
	bool revOccurs = false;
	bool uidOccurs = false;
	bool kindOccurs = false;
	bool kindSecondOccur = false;



	//I should probably
	while((element = nextElement(&optionalIter)) != NULL){
		Property *objProp = (Property *) element;
		char *propName = objProp->name;
		char *propGroup = objProp->group;
		List *propValues = objProp->values;
		List *propParas = objProp->parameters;

		if(propGroup == NULL){
			return INV_PROP;
		}

		if(propName == NULL){
			return INV_PROP;
		}

		if(propValues == NULL){
			return INV_PROP;
		}

		if(propParas == NULL){
			return INV_PROP;
		}

		PropertyType propertyNum = getPropertyType(propName);

		//------------jSON STUFf REMOVE LATER////////////////
		char *jsonString2 = strListToJSON(objProp->values);
		//printf("%s\n", jsonString2);
		List *myJsonList2 = JSONtoStrList(jsonString2);
		char *strList2 = toString(myJsonList2);
		//printf("strLIST: %s\n", strList2);
		freeList(myJsonList2);
		free(strList2);
		free(jsonString2);

		//TESTING TO OBJPROP
		char *myJsonProp = propToJSON(objProp);
		printf("my jsonProp: %s\n", myJsonProp);
		Property* jsonProperty = JSONtoProp(myJsonProp);
		char *strJsonProperty = printProperty(jsonProperty);
		printf("the propJson: %s\n", strJsonProperty);
		free(strJsonProperty);
		deleteProperty(jsonProperty);
		free(myJsonProp);
		////////////////////////////////////////////////////


		//first check basic stuff like is each prop has a value or not
		if(getLength(objProp->values) < 1){
			printf("No value \n");
			return INV_PROP;
		}
		//checking if group is empty string, if not then error
		if(objProp->group == NULL){
			printf("group was null\n");
			return INV_PROP;
		}

		if(objProp->name == NULL){
			printf("name was null\n");
			return INV_PROP;
		}

		//check if the values are valid, aka not null, do while loop here
		if(getLength(objProp->values) > 0){
			ListIterator valueIter = createIterator(objProp->values);
			void *valueElement = NULL;
			while((valueElement = nextElement(&valueIter)) != NULL){
				char *tempValue = (char *) valueElement;
				if(tempValue == NULL){
					printf("there isinvalid value, either null\n");
					return INV_PROP;
				}
			}
		}


		//if a parameter exists check if the name exists and the value
		//need to run an iterator
		if(getLength(objProp->parameters) > 0){
			ListIterator paraIter = createIterator(objProp->parameters);
			void *paraElement = NULL;
			while((paraElement = nextElement(&paraIter)) != NULL){
				Parameter *tempPara = (Parameter *) paraElement;
				if(strlen(tempPara->name) == 0 || strlen(tempPara->value) == 0){
					printf("there is no para value or para name\n");
					return INV_PROP;
				}
			}
		}

		//OR make a function that returns a number so you can put it in a switch case
		//you can make a enum thing because its more readable


		//printf("Propertey name: %s\n", propName);
		int numberOfValues = getLength(objProp->values);

		//Entering switch
		switch (propertyNum){
			case INVALID:
				return INV_PROP;
				break;
			case BEGIN:
				printf("extra begin\n");
				return INV_PROP;
				break;
			case END:
				printf("extra end");
				return INV_PROP;
				break;
			case SOURCE:
				break;
			case KIND:
				if(kindOccurs){
					printf("multiple kind\n");
					return INV_PROP;
				}
				if(numberOfValues > 1){
					printf("too many kind values\n");
					return INV_PROP;
				}
				//char *valueName = toString(objProp->values);
				void *valName = getFromFront(objProp->values);
				//printf("Value Name for kind %s\n", (char *)valName);
				if( (strcmp((char *)valName, "individual") == 0 ) || (strcmp((char *)valName, "group") == 0 ) || (strcmp((char *)valName, "org") == 0 ) || (strcmp((char *)valName, "location") == 0 )){
					//printf("correct");
					if(strcmp((char *)valName, "org") == 0 ){
						kindDenyMember = true; //IF this true then there is no member property in the vcard
					}
				}else{
					printf("not right");
					return INV_PROP;
				}
				kindOccurs = true;
				break;
			case XML:
				if(numberOfValues > 1){
					printf("too much values in xml\n");
					return INV_PROP;
				}
				break;

			//IDENTIFICATION PROPERTIES
			case FN:
				if(numberOfValues > 1){
					printf("too much values in fn\n");
					return INV_PROP;
				}
				break;
			case N: //only one can occur
				//printf("n propterty and is %d\n", nOccurs);
				if(nOccurs){
					printf("Multiple n properties");
					return INV_PROP;
				}
				if(numberOfValues != 5){
					printf("not enough values");
					return INV_PROP;
				}

				nOccurs = true;
				break;

			case NICKNAME:
				break;
			case PHOTO:
				//NOT SURE IF, MIGHT BE ALLOWED MORE
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case BDAY:
				printf("multiple bday\n");
				return INV_DT;
				break;
			case ANNIVERSARY:
				printf("multiple anni\n");
				return INV_DT;
				break;
			case GENDER:
				//printf("hello jojo\n");
				if(genderOccurs){
					printf("multiple gender");
					return INV_PROP;
				}
				if(numberOfValues > 2){
					printf("too many gender values");
					return INV_PROP;
				}
				void *genName = getFromFront(objProp->values);
				//printf("Value Name for kind %s\n", (char *)genName);
				if( (strcmp((char *)genName, "") == 0 ) || (strcmp((char *)genName, "M") == 0 ) || (strcmp((char *)genName, "F") == 0 ) || (strcmp((char *)genName, "O") == 0 ) || (strcmp((char *)genName, "N") == 0 ) || (strcmp((char *)genName, "U") == 0 ) ){
					//printf("correct");
					genderOccurs = true;
				}else{
					printf("not right");
					return INV_PROP;
				}
				genderOccurs = true;
				break;
			//ADDRESSING PROPERTIES
			case ADR:
				if(numberOfValues != 7){
					printf("not enough adr values");
					return INV_PROP;
				}
				break;
			// COMMUNITCATION PROPERTIES
			case TEL:
				break;
			case EMAIL:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case IMPP:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case LANG:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			//GEOGRAPICAL PROPERTIES
			case TZ:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case GEO:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			//ORGINZATIONAL VALUES
			case TITLE:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case ROLE:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case LOGO:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case ORG:
				break;
			case MEMBER://NOT SURE, ASK IN DISCUSSION
				if(kindDenyMember){
					printf("kind had an org");
					return INV_PROP;
				}
				if(numberOfValues > 1)
					return INV_PROP;

				ListIterator opIter = createIterator(obj->optionalProperties);
				void *opName = NULL;
				while((opName = nextElement(&opIter)) != NULL){
					Property *innerProp = (Property *) opName;
					char *innerPropName = innerProp->name;
					PropertyType innerPropertyNum = getPropertyType(innerPropName);
					if(innerPropertyNum == KIND){
						kindSecondOccur = true;
						void *kindValue = getFromFront(innerProp->values);
						if(strcmp((char *)kindValue, "group") !=  0){
							printf("kind and member conflict\n");
							return INV_PROP;
						}
					}
				}

				if(kindSecondOccur == false){
					printf("kind not found in card for member\n");
					return INV_PROP;
				}

				break;
			case RELATED:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			//Explanitory PROPERTIES
			case CATEGORIES:
				break;
			case NOTE:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case PRODID:
				if(proIdOccurs){
					printf("too many proid\n");
					return INV_PROP;
				}
				if(numberOfValues > 1)
					return INV_PROP;

				proIdOccurs = true;
				break;
			case REV:
				if(revOccurs){
					printf("too many rev\n");
					return INV_CARD;
				}
				if(numberOfValues > 1)
					return INV_PROP;

				revOccurs = true;
				break;
			case SOUND:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case UID:
				if(uidOccurs){
					printf("too many uid\n");
					return INV_PROP;
				}
				if(numberOfValues > 1)
					return INV_PROP;

				uidOccurs = true;
				break;
			case CLIENTPIDMAP:
				//can only have 2 values
				if(numberOfValues != 2){
					printf("not corrent amount pid\n");
					return INV_PROP;
				}
				//first value must be an integer that is greater than 0

				break;
			case URL:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case VERSION:
				printf("can't have version in optional prop");
				return INV_CARD;
				break;
			//SECURITY PROPERTIES
			case KEY:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			//CALENDER PROPERTIES
			case FBURL:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case CALADRURI:
				if(numberOfValues > 1)
					return INV_PROP;
				break;
			case CALURI:
				if(numberOfValues > 1)
					return INV_PROP;
				break;

			default:
				printf("invalid prop?\n");
				return INV_PROP;
				break;
		}

		/*if(mystrcmp(propName, "N") == 0){
			if(getLength(objProp->values) != 5){
				printf("not enough values");
				return INV_PROP;
			}
		}else if(mystrcmp(propName, "KIND") == 0){
			if(getLength(objProp->values) > 1){
				printf("too many kind values\n");
				return INV_PROP;
			}
			//char *valueName = toString(objProp->values);
			void *valName = getFromFront(objProp->values);
			printf("Value Name for kind %s\n", (char *)valName);
			if( (strcmp((char *)valName, "individual") == 0 ) || (strcmp((char *)valName, "group") == 0 ) || (strcmp((char *)valName, "org") == 0 ) || (strcmp((char *)valName, "location") == 0 )){
				printf("correct");
				if(strcmp((char *)valName, "org") == 0 ){
					kindOrg = true; //IF this true then there is no member property in the vcard
				}
			}else{
				printf("not right");
				return INV_PROP;
			}
		}*/

	}


	//use a list iterator


	return OK;

}

VCardErrorCode writeCard(const char* fileName, const Card* obj){

	if(fileName == NULL){
		printf("nothing in there");
		return WRITE_ERROR;
	}
	bool correctFile = checkWriteName(fileName);
	//printf("VALUE for checkfileName %d\n", correctFile);

	//checking if INV_FILE
	if(!correctFile){
		printf("\nwrong filename\n\n");
		return WRITE_ERROR;
	}

	FILE *fpw;
	fpw = fopen(fileName, "w");
	if(fpw == NULL){
		printf("file error\n");
		return WRITE_ERROR;
	}

	if(obj == NULL){
		printf("object not created");
		fclose(fpw);
		return WRITE_ERROR;
	}


	//MAKING A COPY OF LIST OF OPTIONAL PROPERTIES
    //put the the object properties into a new list with my own write property function
    List *writeOptionalProperties = initializeList(&writeProperty, &deleteProperty, &compareProperties);
    ListIterator tempCardIter = createIterator(obj->optionalProperties);
    void *element = NULL;
    while((element = nextElement(&tempCardIter)) != NULL){
		Property *objProp = (Property *) element;
		Property *tempProperty = createWriteProperty();
		if(tempProperty == NULL){
			printf("malloc not working\n\n");
			freeList(writeOptionalProperties);
			return OTHER_ERROR;
		}
		strcpy(tempProperty->group, objProp->group);
		strcpy(tempProperty->name, objProp->name);

		ListIterator tempParaIter = createIterator(objProp->parameters);
		void *paraElement = NULL;
		while((paraElement = nextElement(&tempParaIter)) != NULL){
			Parameter *objPara = (Parameter *) paraElement;
			Parameter *tempParameter = createParameter();
			if(tempParameter == NULL){
				printf("malloc not working\n\n");
				freeList(writeOptionalProperties);
				return OTHER_ERROR;
			}
			strcpy(tempParameter->name, objPara->name);
			strcpy(tempParameter->value, objPara->value);
			insertBack(tempProperty->parameters, tempParameter);
		}

		ListIterator tempValueIter = createIterator(objProp->values);
		void *valueElement = NULL;
		while((valueElement = nextElement(&tempValueIter)) != NULL){
			char *objValue = (char *) valueElement;
			char *tempValue = malloc(sizeof(char) * (strlen(objValue) +1) );
			if(tempValue == NULL){
				printf("malloc not working\n\n");
				freeList(writeOptionalProperties);
				return OTHER_ERROR;
			}
			strcpy(tempValue, objValue);
			insertBack(tempProperty->values, tempValue);
		}
        //Property *tempProperty = (Property *) element;
        insertBack(writeOptionalProperties, tempProperty);
    }

	char *stringOP = toString(writeOptionalProperties);

	//write start
    fprintf(fpw,"BEGIN:VCARD\r\n");
    fprintf(fpw,"VERSION:4.0\r\n");

    //write fn property
    void *fnValue = getFromFront(obj->fn->values);
    //printf("The valueeee for fn: %s\n", fnValue);
    fprintf(fpw,"%s:%s\r\n", obj->fn->name, (char *) fnValue);

    //write BDAY and ANNI
    if(obj->birthday != NULL){
        int bdayDateLen = strlen(obj->birthday->date);
        int bdayTimeLen = strlen(obj->birthday->time);
        if(obj->birthday->isText){
            fprintf(fpw, "BDAY;VALUE=text:%s\r\n", obj->birthday->text);
        }else{
            if(bdayDateLen != 0 && bdayTimeLen != 0 && obj->birthday->UTC){
                fprintf(fpw, "BDAY:%sT%sZ\r\n", obj->birthday->date, obj->birthday->time);
            }else if(bdayDateLen != 0 && bdayTimeLen != 0){
                fprintf(fpw, "BDAY:%sT%s\r\n", obj->birthday->date, obj->birthday->time);
            }else if(bdayDateLen == 0 && bdayTimeLen != 0 && obj->birthday->UTC){
                fprintf(fpw, "BDAY:T%sZ\r\n", obj->birthday->time);
            }else if(bdayDateLen == 0 && bdayTimeLen != 0){
                fprintf(fpw, "BDAY:T%s\r\n", obj->birthday->time);
            }else if(bdayDateLen != 0 && bdayTimeLen == 0){
                fprintf(fpw, "BDAY:%s\r\n", obj->birthday->date);
            }
        }
    }

    if(obj->anniversary != NULL){
        int anniDateLen = strlen(obj->anniversary->date);
        int anniTimeLen = strlen(obj->anniversary->time);
        if(obj->anniversary->isText){
            fprintf(fpw, "ANNIVERSARY;VALUE=text:%s\r\n", obj->anniversary->text);
        }else{
            if(anniDateLen != 0 && anniTimeLen != 0 && obj->anniversary->UTC){
                fprintf(fpw, "ANNIVERSARY:%sT%sZ\r\n", obj->anniversary->date, obj->anniversary->time);
            }else if(anniDateLen != 0 && anniTimeLen != 0){
                fprintf(fpw, "ANNIVERSARY:%sT%s\r\n", obj->anniversary->date, obj->anniversary->time);
            }else if(anniDateLen == 0 && anniTimeLen != 0 && obj->anniversary->UTC){
                fprintf(fpw, "ANNIVERSARY:T%sZ\r\n", obj->anniversary->time);
            }else if(anniDateLen == 0 && anniTimeLen != 0){
                fprintf(fpw, "ANNIVERSARY:T%s\r\n", obj->anniversary->time);
            }else if(anniDateLen != 0 && anniTimeLen == 0){
                fprintf(fpw, "ANNIVERSARY:%s\r\n", obj->anniversary->date);
            }
        }
    }


    printf("SOKDOSKdos\n\n\n\n%s\n", stringOP);
	//write optional properties
    fprintf(fpw,"%s", stringOP);

	//write the end
    fprintf(fpw,"END:VCARD\r\n");

	freeList(writeOptionalProperties);
	free(stringOP);
	fclose(fpw);

	return OK;
}

//if you want to modify something
// ************* Card parser fucntions - MUST be implemented ***************
VCardErrorCode createCard(char* fileName, Card** newCardObject){
	//first we need to check if the file is value, if it is then sent OK
	if(fileName == NULL){
		printf("nothing in there");
		return INV_FILE;
	}
	bool correctFile = checkFileName(fileName);
	//printf("VALUE for checkfileName %d\n", correctFile);

	//checking if INV_FILE
	if(!correctFile){
		printf("wrong filename");
		return INV_FILE;
	}

	FILE *fp;
	fp = fopen(fileName, "r");
	if(fp == NULL){

		printf(" for fp file error\n");
		return INV_FILE;
	}

	FILE *fileP2;
	fileP2 = fopen(fileName, "r");
	if(fileP2 == NULL){
		printf("for fileP2 file error\n");
		return INV_FILE;
	}

	FILE *fp0;
	fp0 = fopen(fileName, "r");
	if(fp0 == NULL){
		printf("for fp0 file error\n");
		return INV_FILE;
	}



	VCardErrorCode parseResult = OK;
	VCardErrorCode basicError = basicFileCheck(fp0,  fp, fileP2);
	printf("the error code it %d", basicError );


	if(basicError == INV_FILE || basicError == INV_CARD || basicError == INV_PROP){
		printf("\nerror from basicFileCheck\n\n");
		//deleteCard(*newCardObject);
		//*newCardObject = NULL;
		fclose(fp);
		fclose(fileP2);
		fclose(fp0);
		return basicError;
	}else{
		//FILE is now OK to parse, begin process by mallocing for newCardObject
		*newCardObject = malloc(sizeof(Card));
		if(*newCardObject == NULL){
			deleteCard(*newCardObject);
			*newCardObject = NULL;
			fclose(fp);
			fclose(fileP2);
			fclose(fp0);
			return OTHER_ERROR;
		}
		(*newCardObject)->fn = createProperty(); // or should it be  = malloc(sizeof(Property))
		if((*newCardObject)->fn == NULL){
			deleteCard(*newCardObject);
			*newCardObject = NULL;
			fclose(fp);
			fclose(fileP2);
			fclose(fp0);
			return OTHER_ERROR;
		}
		(*newCardObject)->optionalProperties = initializeList(&printProperty, &deleteProperty, &compareProperties);
		//if found in vCard then do createDateTime()
		(*newCardObject)->birthday = NULL;
		(*newCardObject)->anniversary = NULL;

		parseResult = parseFile(fp, fileP2, newCardObject);
		if(parseResult == INV_PROP || parseResult == OTHER_ERROR || parseResult == INV_CARD){
			deleteCard(*newCardObject);
			*newCardObject = NULL;
		}
	}


	//else its okay
	fclose(fp);
	fclose(fileP2);
	fclose(fp0);
	return parseResult;
}


void deleteCard(Card* obj){
	Card *pointerObj;
	if(obj == NULL){
		return;
	}
	//tobeDeleted is the property that is wanted to be deleted
	pointerObj = (Card *) obj;
	deleteProperty(pointerObj->fn);
	freeList(pointerObj->optionalProperties);
	//we need to free the dateTimes
	deleteDate(pointerObj->birthday);
	deleteDate(pointerObj->anniversary);
	free(pointerObj);
	pointerObj = NULL;
}

char* printCard(const Card* obj){
	char* tmpStr;
	Card* tmpName;
	int len;
	if (obj == NULL){
		//printf("nullyo");
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "It's NULL");
		return tmpStr;
	}

	tmpName = (Card *)obj;

	char *optionalProperties = toString(tmpName->optionalProperties);
	char *stringFNProperty = printProperty(tmpName->fn);
	char *stringBirthday = printDate(tmpName->birthday);
	char *stringAnni = printDate(tmpName->anniversary);


	//!!!!!!!!!!!!Need to update to include DATE MEMBER
	if(tmpName->birthday == NULL && tmpName->anniversary == NULL){
		len = strlen(optionalProperties)+strlen(stringFNProperty) + 150;
	}else if(tmpName->anniversary == NULL){
		len = strlen(optionalProperties)+strlen(stringFNProperty) + strlen(stringBirthday) + 150;
	}else if(tmpName->birthday == NULL){
		len = strlen(optionalProperties)+strlen(stringFNProperty) + strlen(stringAnni) + 150;
	}else{
		len = strlen(optionalProperties)+strlen(stringFNProperty) + strlen(stringBirthday) + strlen(stringAnni) + 150;
	}

	tmpStr = (char*)malloc(sizeof(char)*len);
	sprintf(tmpStr, "FN Property:%s\nBirthday:%s\nAnniversary:%s\n\n------OPTIONAL CARD PROPERTIES------ %s\n", stringFNProperty, stringBirthday, stringAnni ,optionalProperties);
	//freeing the string values

	free(optionalProperties);
	free(stringFNProperty);
	free(stringBirthday);
	free(stringAnni);

	return tmpStr;

}

char* printError(VCardErrorCode err){
	char* tmpStr;
	//printf("\n\nIN printerrror error type is:%d\n", err);

	tmpStr = (char*)malloc(sizeof(char)*50);
	if(tmpStr == NULL){
		strcpy(tmpStr, "NUll entered");
	}

	if(err == OK){
		strcpy(tmpStr, "OK");
	}
	if(err == INV_FILE){
		strcpy(tmpStr, "INV_FILE");
	}
	if(err == INV_CARD){
		strcpy(tmpStr, "INV_CARD");
	}
	if(err == INV_PROP){
		strcpy(tmpStr, "INV_PROP");
	}
	if(err == INV_DT){
		strcpy(tmpStr, "INV_DT");
	}
	if(err == WRITE_ERROR){
		strcpy(tmpStr, "WRITE_ERROR");
	}
	if(err == OTHER_ERROR){
		strcpy(tmpStr, "OTHER_ERROR");
	}
	//freeing the string values
	return tmpStr;
}




//*************PROPERTY FUNCTIONS*************************
void deleteProperty(void* toBeDeleted){
	Property *tempProp;
	if(toBeDeleted == NULL){
		return;
	}
	//tobeDeleted is the property that is wanted to be deleted
	tempProp = (Property *) toBeDeleted;
	free(tempProp->name);
	free(tempProp->group);
	freeList(tempProp->parameters);
	freeList(tempProp->values);
	//do we need to do free(temoProp)?
	free(tempProp);
	tempProp = NULL;

}

//YOU CAN USE THE COMPARE FUNCTION FOR THE LIST
int compareProperties(const void* first,const void* second){
	Property* tmpName1;
	Property* tmpName2;

	if (first == NULL || second == NULL){
		return 0;
	}

	tmpName1 = (Property*)first;
	tmpName2 = (Property*)second;

	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);
}
char* printProperty(void* toBePrinted){
	char* tmpStr;
	Property* tmpName;
	int len;

	if (toBePrinted == NULL){
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "It's NULL");
		return tmpStr;
	}

	tmpName = (Property*)toBePrinted;

	char *stringValues = toString(tmpName->values);
	char *stringParameters = toString(tmpName->parameters);

	len = strlen(tmpName->group)+strlen(tmpName->name)+ strlen(stringParameters) + strlen(stringValues) + 250;
	tmpStr = (char*)malloc(sizeof(char)*len);
	sprintf(tmpStr, "\n(CONTENT LINE)->Group:%s || Name:%s || LIST_PARAMETERS-%s || LIST_VALUES-%s\n", tmpName->group, tmpName->name, stringParameters, stringValues);
	//freeing the string values

	free(stringValues);
	free(stringParameters);

	return tmpStr;
}


//***********PARAMETER FUNCTIONS*************************
void deleteParameter(void* toBeDeleted){
	Parameter *tempPara;
	if(toBeDeleted == NULL){
		return;
	}
	//tobeDeleted is the parameter that is wanted to be deleted
	tempPara = (Parameter *) toBeDeleted;
	//I dont think we need to free tempPara->name[] this since it alread has 200 bytes
	free(tempPara); //free the container itself, i dont think we need to free tempPara->value sinze it was allocated with newPara
	tempPara = NULL;
}
int compareParameters(const void* first,const void* second){
	Parameter* tmpName1;
	Parameter* tmpName2;

	if (first == NULL || second == NULL){
		return 0;
	}

	tmpName1 = (Parameter*)first;
	tmpName2 = (Parameter*)second;

	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);
}

//FILL LATER
char* printParameter(void* toBePrinted){
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
	sprintf(tmpStr, "\nName(%s) ParaValue(%s)\n", tmpName->name, tmpName->value);
	//freeing the string values
	return tmpStr;

}

//***********VALUE FUNCTIONS*************************
void deleteValue(void* toBeDeleted){
	char *tempValue;
	if(toBeDeleted == NULL){
		return;
	}
	//tobeDeleted is the parameter that is wanted to be deleted
	tempValue = (char *) toBeDeleted;
	//I dont think we need to free tempPara->name[] this since it alread has 200 bytes
	free(tempValue); //free the container itself, i dont think we need to free tempPara->value sinze it was allocated with newPara
	tempValue = NULL;


}
int compareValues(const void* first,const void* second){
	char* value1;
	char* value2;

	if (first == NULL || second == NULL){
		return 0;
	}

	value1 = (char*)first;
	value2 = (char*)second;

	return strcmp(value1, value2);
}
char* printValue(void* toBePrinted){
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

	sprintf(returnString, "\nValue(%s)", tempString);

	return returnString;
}

//DATE TIMES FUNCTIONS

void deleteDate(void* toBeDeleted){
	DateTime *tempDate;
	if(toBeDeleted == NULL){
		return;
	}
	//tobeDeleted is the parameter that is wanted to be deleted
	tempDate = (DateTime *) toBeDeleted;
	//I dont think we need to free tempPara->name[] this since it alread has 200 bytes
	free(tempDate);
	tempDate = NULL;

}
int compareDates(const void* first,const void* second){
	DateTime* value1;
	DateTime* value2;

	if (first == NULL || second == NULL){
		return 0;
	}

	value1 = (DateTime*)first;
	value2 = (DateTime*)second;

	return strcmp(value1->date, value2->date);
}
char* printDate(void* toBePrinted){
	char* tmpStr;
	DateTime* tmpName;
	int len;

	if (toBePrinted == NULL){
		tmpStr = malloc(sizeof(char)*20);
		strcpy(tmpStr, "It's NULL");
		return tmpStr;
	}

	tmpName = (DateTime*)toBePrinted; //casting it so type parameter

	len = strlen(tmpName->date)+strlen(tmpName->time) +strlen(tmpName->text) + 100;
	tmpStr = (char*)malloc(sizeof(char)*len);
	sprintf(tmpStr, "DATETime: Date:%s Time:%s UTC:%d isText:%d Text:%s\n", tmpName->date, tmpName->time, tmpName->UTC, tmpName->isText, tmpName->text);
	//freeing the string values
	return tmpStr;
}
