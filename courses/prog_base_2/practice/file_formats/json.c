
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void parse(char * text)
{
	char * out;
	cJSON * json;
	
	json = cJSON_Parse(text);
	if (!json) {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	else
	{
		cJSON * jname = cJSON_GetObjectItem(json, "name");
		int value = jname->valueint;
		printf("Name is %i\n", value);
		out = cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n", out);
		free(out);
	}
}

// gcc main.c cJSON.c -lm
int main() {
	char * string = "{\"name\" : 1}";
	parse(string);
	return 0;
}
