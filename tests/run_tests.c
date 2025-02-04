#include "utest.h"

#include <signal.h>

#define REISHASH_ABBR
#include <reis.h>

#include <pthread.h>
#include <reis/raylib.h>

/*struct TestFixture {
		char *str;
};

UTEST_F_SETUP(TestFixture)
{
		utest_fixture->str = "Hello World";

		ASSERT_EQ("Hello World", utest_fixture->str);
		EXPECT_TRUE(true);
}*/

UTEST(reisString, chrcat) 
{
    char str[13] = "Hello World";
    ASSERT_STREQ("Hello World!", chrcat(str, '!'));

    char *str2 = (char*)malloc(sizeof(char) * 3);
    strcpy(str2, "H");
    ASSERT_STREQ(str2, chrcat(str2, 'i'));

    char chrset[8] = "elWd";

    ASSERT_STREQ("Hello World!", strcov(str, chrset));

    free(str2);
}

UTEST(reisString, strcov) 
{
    char str[13] = "Hello World";
    char chrset[8] = "elWd";
		
    ASSERT_STREQ("Hello World", strcov(str, chrset));
}

UTEST(reisIO, fpeek) 
{
    FILE *fp = fopen("test.txt", "w");
    ASSERT_TRUE(fp);
    fclose(fp);
}

UTEST(reisIO, filesystem_t)
{
	char path[] = "/home/henry/test.txt";

	filesystem_t *fs = FS_Create( path );

	ASSERT_STREQ( fs->path, path );

	ASSERT_STREQ(fs->filename, "test.txt");

	ASSERT_STREQ(fs->extension, ".txt");

	ASSERT_STREQ(fs->stem, "test");

	FS_Destroy( fs );
}


UTEST(reisTrie, Creation)
{
		TrieNode *root = TrieNodeInit(L'\0', 26);
		root = TrieInsert(root, L"hello");
		root = TrieInsert(root, L"hi");
		root = TrieInsert(root, L"teabag");
		root = TrieInsert(root, L"teacan");
		
		ASSERT_TRUE(TrieSearch(root, L"hello"));

		ASSERT_FALSE(TrieSearch(root, L"tea"));

		TrieNodeFree(root);
}

UTEST(reisMemory, MALLOC)
{
		int *x = (int*)MALLOC(sizeof(int));
		*x = 10;

		ASSERT_EQ(10, *x);

		FREE(x);
}

UTEST(reisMemory, CALLOC)
{
		int n = 5;
		int *array;

		array = (int*)CALLOC(n, sizeof(int));

		for (int i = 0; i < n; i++) {
				ASSERT_EQ(0, array[i]);
		}

		FREE(array);
}

UTEST(reisMemory, MGET)
{
		u32 *x = (u32*)MALLOC(sizeof(u32));

		ASSERT_EQ(4, (int)MGET(x));

		FREE(x);
}

UTEST(reisMemory, REALLOC)
{
		u32 *x = (u32*)MALLOC(sizeof(u32));

		ASSERT_EQ(4, (int)MGET(x));

		u16 *y = (u16*)REALLOC(x, sizeof(u16));

		ASSERT_EQ(2, (int)MGET(y));

		FREE(y);
}

UTEST(HashTable, reisHash)
{

    hashtable_t *tab = HT_New();
    int *x = (int*)MALLOC(sizeof(int));
    *x = 10;

    const char *ret = HT_Set(tab, "afdfhhfgh", "sdfafsafs");

    ASSERT_NE(ret, NULL);

    ret = HT_Get(tab, "afdfhhfgh");


    ASSERT_STREQ("sdfafsafs", ret);

    ret = HT_Set(tab, "afdfhhfgh", "sda");
    ret = HT_Get(tab, "afdfhhfgh");

    ASSERT_STREQ( "sda", ret );

    HT_Del( tab );

    FREE( x );
}

/*#include <reis/multimap.h>

UTEST(reisMultiMap, Creation)
{
		MultiMap *map = MultiMapInit();
		MultiMapInsert(map, L"j", L"b", sizeof(wchar_t) * 2);
		MultiMapInsert(map, L"j", L"c", sizeof(wchar_t) * 2);
		MultiMapInsert(map, L"j", L"b", sizeof(wchar_t) * 2);
		
		MultiMapInsert(map, L"asdffds", L"s", sizeof(wchar_t) * 2);

		int *y = MALLOC(sizeof(int));

		for(int i = 0; i < 9323; i++) {
				*y = i;
				MultiMapInsert(map, L"asdffds", y, sizeof(int));
		}
		

		size_t *num = (size_t*)MALLOC(sizeof(size_t));
		void **values = MultiMapGet(map, "asdffds", num);

		for(int i = 0; i < (int*)*num; i++)
				printf("%s\n", values[i]);


		MultiMapFree(map);
		FREE(num);
		FREE(values);
		//FREE(y);
		MultiMapFree(map);
}*/

UTEST(reisString, strpre)
{
		char str[5] = "bc";
		strpre(str, 'a');

		ASSERT_STREQ("abc", str);
}

UTEST(reisString, strpres)
{
		char str[5] = "cd";
		strpres(str, "ab");

		ASSERT_STREQ("abcd", str);
}

UTEST(reisString, wcspres)
{
		wchar_t str[5] = L"cd";
		wcspres(str, L"ab");

		ASSERT_EQ(0, wcscmp(L"abcd", str));
}

UTEST(reisMemory, HeapCheck)
{
		ASSERT_EQ(0, (int)reisMemGetAll());
}

UTEST(reisArray, SortIntArray) {
		int arr[5] = {2,3,5,6,3};
		int correct[5] = {2,3,3,5,6};
		reisInsertionSort( arr, 5 );
		
		for ( int i = 0; i < 5; i ++ ) {
				ASSERT_EQ(correct[i], arr[i]);
		}
}

#if defined(RAYLIB)

UTEST(DrawDashedLine, reisRaylib)
{
		SetTraceLogLevel(LOG_ERROR);
		InitWindow(600, 600, "test");
		SetTargetFPS(60);

		//while(!WindowShouldClose())
		{
				BeginDrawing();
						ClearBackground(RAYWHITE);
						DrawDashedLineEx((Vector2){10, 10}, (Vector2){400, 400}, 10, 4, BLACK);

						DrawDashedLineEx((Vector2){60, 10}, (Vector2){120, 80}, 10, 4, BLACK);
				EndDrawing();
		}

		CloseWindow();

		ASSERT_EQ(1, 1);
}


#endif


UTEST_MAIN();
