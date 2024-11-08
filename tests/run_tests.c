#include "utest.h"

#include <signal.h>

#include <xtra/string.h>

#include <xtra/memory.h>

#include <xtra/trie.h>
#include <xtra/io.h>
#include <pthread.h>
#include <xtra/hash.h>
#include <xtra/raylib.h>

/*struct TestFixture {
		char *str;
};

UTEST_F_SETUP(TestFixture)
{
		utest_fixture->str = "Hello World";

		ASSERT_EQ("Hello World", utest_fixture->str);
		EXPECT_TRUE(true);
}*/

UTEST(XtraString, chrcat) 
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

UTEST(XtraString, strcov) 
{
		char str[13] = "Hello World";
		char chrset[8] = "elWd";
		
		ASSERT_STREQ("Hello World", strcov(str, chrset));
}

UTEST(XtraIO, fpeek) 
{
		FILE *fp = fopen("test.txt", "w");
		ASSERT_TRUE(fp);
		fclose(fp);

}

UTEST(XtraTrie, Creation)
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

UTEST(XtraMemory, MALLOC)
{
		int *x = (int*)MALLOC(sizeof(int));
		*x = 10;

		ASSERT_EQ(10, *x);

		FREE(x);
}

UTEST(XtraMemory, CALLOC)
{
		int n = 5;
		int *array;

		array = (int*)CALLOC(n, sizeof(int));

		for (int i = 0; i < n; i++) {
				ASSERT_EQ(0, array[i]);
		}

		FREE(array);
}

UTEST(XtraMemory, MGET)
{
		u32 *x = (u32*)MALLOC(sizeof(u32));

		ASSERT_EQ(4, (int)MGET(x));

		FREE(x);
}

UTEST(XtraMemory, REALLOC)
{
		u32 *x = (u32*)MALLOC(sizeof(u32));

		ASSERT_EQ(4, (int)MGET(x));

		u16 *y = (u16*)REALLOC(x, sizeof(u16));

		ASSERT_EQ(2, (int)MGET(y));

		FREE(y);
}

UTEST(HashTable, XtraHash)
{
		HashTable *tab = HashTableInit();
		int *x = (int*)MALLOC(sizeof(int));
		*x = 10;

		const char *ret = HashTableSet(tab, "afdfhhfgh", "sdfafsafs");

		ASSERT_NE(ret, NULL);

		ret = HashTableGet(tab, "afdfhhfgh");


		ASSERT_STREQ("sdfafsafs", ret);

		ret = HashTableSet(tab, "afdfhhfgh", "sda");
		ret = HashTableGet(tab, "afdfhhfgh");

		ASSERT_STREQ("sda", ret);

		HashTableTerminate(tab);

		FREE(x);
}

#include <xtra/multimap.h>

UTEST(XtraMultiMap, Creation)
{
		MultiMap *map = MultiMapInit();
		MultiMapInsert(map, L"j", L"b", sizeof(wchar_t) * 2);
		MultiMapInsert(map, L"j", L"c", sizeof(wchar_t) * 2);
		MultiMapInsert(map, L"j", L"b", sizeof(wchar_t) * 2);
		
		MultiMapInsert(map, L"asdffds", L"s", sizeof(wchar_t) * 2);

		/*int *y = MALLOC(sizeof(int));

		for(int i = 0; i < 9323; i++) {
				*y = i;
				MultiMapInsert(map, L"asdffds", y, sizeof(int));
		}*/
		
		/*
		size_t *num = (size_t*)MALLOC(sizeof(size_t));
		void **values = MultiMapGet(map, "asdffds", num);

		for(int i = 0; i < (int*)*num; i++)
				printf("%s\n", values[i]);


		MultiMapFree(map);
		FREE(num);
		FREE(values);*/
		//FREE(y);
		MultiMapFree(map);
}

#include <xtra/pair.h>

UTEST(XtraPair, Creation)
{
		Pair *par = PairCreate(L"a", L"b");
		
		PairFree(par);
}

UTEST(XtraString, strpre)
{
		char str[5] = "bc";
		strpre(str, 'a');

		ASSERT_STREQ("abc", str);
}

UTEST(XtraString, strpres)
{
		char str[5] = "cd";
		strpres(str, "ab");

		ASSERT_STREQ("abcd", str);
}

UTEST(XtraString, wcspres)
{
		wchar_t str[5] = L"cd";
		wcspres(str, L"ab");

		ASSERT_EQ(0, wcscmp(L"abcd", str));
}

UTEST(XtraMemory, HeapCheck)
{
		ASSERT_EQ(0, (int)mget_all());
}

#if defined(RAYLIB)

UTEST(DrawDashedLine, XtraRaylib)
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
