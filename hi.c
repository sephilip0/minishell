#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int	pipito()
{
	int piper[2];	

	if (pipe(piper) != -1)
		printf("ERROR\n");
	printf("piper[0]: %d\n", piper[0]);
	printf("piper[1]: %d\n", piper[1]);
	write(piper[1], "hello", 5);
	close(piper[1]);
	return (piper[0]);
}

int	main()
{
	int out;
	char *str;
/*
	str = (char *)calloc(sizeof(char), 6);	
	out = pipito();
	printf("out: %d\n", out);
	read(out, str, 5);
	printf("read: %s\n", str);
	close(out);
	free(str);*/
	return (0);
}
