#include "utest.h"
#include <xtra/string.h>

/*struct TestFixture {
		char *str;
};

UTEST_F_SETUP(TestFixture)
{
		utest_fixture->str = "Hello World";

		ASSERT_EQ("Hello World", utest_fixture->str);
		EXPECT_TRUE(true);
}*/


UTEST(chrcat, xtra_string) {
		char str[13] = "Hello World";
		ASSERT_STREQ("Hello World!", chrcat(str, '!'));

		char *str2 = (char*)malloc(sizeof(char) * 3);
		strcpy(str2, "H");
		ASSERT_STREQ(str2, chrcat(str2, 'i'));

		char chrset[8] = "elWd";
		
		ASSERT_STREQ("Hello World!", strcov(str, chrset));

		free(str2);
		
}

UTEST(init_slice, xtra_string) {
		
		char str[13] = "Hello Sailor";

		slice x = (slice)CSTRING_SLICE_INIT(str, strlen(str));
}

// ----

#include <xtra/io.h>


UTEST(fpeek, xtra_io) {
		FILE *fp = fopen("test.txt", "w");
		ASSERT_TRUE(fp);
		fclose(fp);

}

UTEST_MAIN();
