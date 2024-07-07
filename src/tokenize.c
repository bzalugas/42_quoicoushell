/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/07 02:54:12 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

/*
EASY STEPS FOR AN EASY TOKENIZATION.
1-	replace $text by the corresponding variables (unless inside single quotes).
	A variable starts with $ and can only contain alphanumerics and underscore.
2-	write a custom "ft_split" that doesnt split inside quotes and splits
	around special characters.
	(such as '|', ';', '\n', '&', '<', '>', '=', '<<', '>>').
	a/n/d/ s/h/o/u/l/d e/x/c/l/u/d/e q/u/o/t/e/s f/r/o/m t/h/e o/u/t/p/u/t/.
	should keep the quotes to differentiate operator from string (ie: | vs "|")
3-	categorize each as either command or operator
4-	remove quotes at extremities of each elements ig
*/

