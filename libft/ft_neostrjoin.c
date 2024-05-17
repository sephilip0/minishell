/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_neostrjoin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:05:53 by sephilip          #+#    #+#             */
/*   Updated: 2024/05/16 14:08:34 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//REMEMBER WE CANT CHECK IF THE STRING WAS ALLOCATED OR NOT, BE CAREFUL
//IMPROVE SO WE CAN STRING JOIN WITH ONE OF THEM BEING NULL 

char	*ft_neostrjoin(char *s1, char *s2, int parfree)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	printf("neostr: %ld", len1 + len2 + 1);
	str = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcat(str, s2, len1 + len2 + 1);
	if (parfree == 1)
		free(s1);
	if (parfree == 2)
		free(s2);
	if (parfree == 3)
	{
		free(s1);
		free(s2);
	}
	return (str);
}
