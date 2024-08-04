#include "utest.h"
#include "../include/xtra/string.h"

UTEST(chrcat, xtra_string) {
		char str[13] = "Hello World";
		ASSERT_STREQ("Hello World!", chrcat(str, '!'));

		char *str2 = (char*)malloc(sizeof(char) * 3);
		strcpy(str2, "H");
		ASSERT_STREQ(str2, chrcat(str2, 'i'));
		free(str2);
		
}

UTEST_MAIN();

