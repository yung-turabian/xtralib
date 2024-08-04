/*

// Requires a file and queue, but effectively seperates strings in a i/o file input
queue* splitString(FILE* file) {
	queue* words = createQueue(sizeof(Word));
	
	Word word;
	currentReadLine = 1;

	char scan[MAX_STRING_SIZE] = "";
	char ch;
	bool newLine = true;
	size_t currPos = 0;

	do {

		for (;;) {
			ch = fgetc(readFile);
			++currPos;
			if(ch == EOF) break;
			if(newLine) {
				newLine = false;
				int len = countLineLen(readFile);
				//printf("[spl-str_debug] line length= %d\n", len);
			}
			
			if(ch != ' ' && ch != '\n') {
				ccat(scan, ch);
			} else if(scan[0] != '\0') {
				currPos -= strlen(scan);
				//printf("[spl-str_debug @%zu:%zu] found word: %s\n", currentReadLine, currPos, scan);
				strcpy(word.value, scan);
				word.line = currentReadLine;
				word.pos = currPos;
				enqueue(words, &word);
				currPos += strlen(scan);

				memset(scan, 0, sizeof(scan));	
			} 

			if(ch == '\n') {
				++currentReadLine;
				newLine = true;
				currPos = 0;
			}
		}
	} while(!feof(readFile));
	
	printf("[spl-str] finished\n");
	return words;
}*/
