

// Reads ANSII code arrow keys
int
read_key(char* buf, int k)
{
		if(buf[k] == '\033' && buf[k + 1] == '[') {
				switch(buf[k + 2]) {
						case 'A': return 1; // up
						case 'B': return 2; // down
						case 'C': return 3; // right
						case 'D': return 4; // left
				}
		}
		return -1; //something went wrong
}
