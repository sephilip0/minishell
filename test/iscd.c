# include <stdio.h>
# include <string.h>

int ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
	|| c == '\v' || c == '\f' || c == '\r');
}

int	is_cd(char *line)
{
	while(*line && ft_isspace(*line))
		line++;
	if (strncmp(line, "cd", 2) != 0 || (!ft_isspace(line[2]) && line[2] && line[2] != '\0')) //Invalid: "da asd"
		return (0);
	line += 2;
	while(*line && ft_isspace(*line))// Espaçamento entre cd e argumento
		line++;
	if (*line == '\0') // Valid: "cd"
		return (1);
	while (*line && !ft_isspace(*line)) // Encontrar o fim do argumento
		line++;
	while (*line && ft_isspace(*line)) // Espaçamento depois do 1º agrumento
		line++;
	if (*line == '\0') //valid: cd param
		return (1);
	return (0); //invalid: cd param param
}

int	main(void)
{
	printf("result: %i\n", is_cd("cd       print       l"));
}

