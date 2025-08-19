/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:25:27 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/16 19:26:01 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*safe_malloc(size_t size) {
	void	*ptr;
	
	ptr = malloc(size);
	if (!ptr) {
		perror("malloc failed");
		exit(1);
	}
	return (ptr);
}
