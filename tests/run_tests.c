#include "utest.h"

#include <signal.h>

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

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
		switch(signal)
		{
				case SIGSEGV:
						printf("Caught segfault at address %p\n", si->si_addr);
						break;

				default:

						break;
		}
		
		exit(0);
}

UTEST(chrcat, xtra_string) 
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

UTEST(strcov, xtra_string) 
{
		char str[13] = "Hello World";
		char chrset[8] = "elWd";
		
		ASSERT_STREQ("Hello World", strcov(str, chrset));
}

/*
UTEST(init_slice, xtra_string) {
		
		char str[13] = "Hello Sailor";

		slice x = (slice)CSTRING_SLICE_INIT(str, strlen(str));
}*/

// ----

#include <xtra/io.h>
#include <pthread.h>


UTEST(fpeek, xtra_io) 
{
		FILE *fp = fopen("test.txt", "w");
		ASSERT_TRUE(fp);
		fclose(fp);

}

#include <xtra/trie.h>

UTEST(trie, trie)
{
		TrieNode *root = trienode_init(L'\0');
		root = trie_insert(root, L"hello");
		root = trie_insert(root, L"hi");
		root = trie_insert(root, L"teabag");
		root = trie_insert(root, L"teacan");
		
		trie_print_search(root, L"hello");

		//trie_print(root);

		trienode_free(root);
}

void *
faulty_eprintf(void *vargp)
{
		eprintf("%s\n", 12);
		return NULL;
}

UTEST(eprintf, xtra_io) 
{
		//char *capture = tee(100);
		//eprintf("%s :%c!", "This Works", ')');
		//fprintf(stderr, "\nCaptured: <%s>\n", capture);

		//struct sigaction sa;

		//memset(&sa, 0, sizeof(struct sigaction));
		//sigemptyset(&sa.sa_mask);
		//sa.sa_sigaction = segfault_sigaction;
		//sa.sa_flags     = SA_SIGINFO;

		//sigaction(SIGSEGV, &sa, NULL);

		//pthread_t thread_id;

		// causes a seg fault
		//pthread_create(&thread_id, NULL, faulty_eprintf, NULL);

		//pthread_detach(thread_id);

		//pthread_join(thread_id, NULL);
}

UTEST(cprintf, xtra_io) 
{
		cprintf(ANSI_MAGENTA, "%s", "Works!");
}

UTEST(xtee, xtra_io) 
{
		char *capture = xtee(100); // first 100 bytes
		printf("Hello, World");
		sleep(1);

		ASSERT_STREQ("Hello, World", capture);
}

UTEST_MAIN();
