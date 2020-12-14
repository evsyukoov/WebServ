//
// Created by Денис on 13/12/2020.
//
#include "lib.hpp"

static int	check_overlap(char *str, int sign)
{
	long overlap;
	long prev;

	overlap = 0;
	while ('0' <= *str && *str <= '9')
	{
		prev = overlap;
		overlap = overlap * 10 + (*str - '0');
		if (prev > overlap && sign == -1)
			return (0);
		if (prev > overlap && sign == 1)
			return (-1);
		str++;
	}
	return (1);
}

int			ft_atoi(char *str)
{
	int sign;
	int res;
	int i;

	sign = 1;
	res = 0;
	i = 0;
	while (*str == '\n' || *str == '\t' || *str == '\r'
		   || *str == '\v' || *str == '\f' || *str == ' ')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	if (*str == '+' && sign != -1)
		str++;
	while ('0' <= str[i] && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (check_overlap(str, sign) == 1)
		return (res * sign);
	return (check_overlap(str, sign));
}

static void		skip(char **s, char delimetr)
{
	while (**s == delimetr && **s)
		(*s)++;
}

static int		word_len(char *s, char delimetr)
{
	int len;

	len = 0;
	while (s[len] != delimetr && s[len])
		len++;
	return (len);
}

static int		word_counter(char *s, char delimetr)
{
	int words;

	words = 0;
	skip(&s, delimetr);
	while (*s)
	{
		while (*s != delimetr && *s)
			s++;
		words++;
		skip(&s, delimetr);
	}
	return (words);
}

char			**ft_split(char *s, char c)
{
	int		words;
	int		i;
	int		j;
	char	**res;

	if (!s)
		return (0);
	words = word_counter(s, c);
	if (!(res = (char**)malloc(sizeof(char*) * (words + 1))))
		return (NULL);
	i = 0;
	skip(&s, c);
	while (i < words)
	{
		if (!(res[i] = (char*)malloc(sizeof(char) * (word_len(s, c) + 1))))
			return (0);
		j = 0;
		while (*s != c && *s)
			res[i][j++] = *s++;
		res[i][j] = '\0';
		i++;
		skip(&s, c);
	}
	res[i] = NULL;
	return (res);
}

size_t 	ft_strlen(char *buff)
{
	int i = 0;
	while (buff[i])
		i++;
	return (i);
}
