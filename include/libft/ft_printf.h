/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:39:36 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:26:00 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define SPECIFIERS "cspdiuxX%"
# define FLAGS "-0.# +*"
# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stdbool.h>

/*********************************** STRUCTS **********************************/

typedef enum e_type
{
	ERR = 0,
	LIT,
	CHAR,
	STR,
	INT,
	UINT,
	LHEX,
	UHEX,
	PTR,
	FLOAT,
	PERCENT,
	CONV
}			t_type;

/* width is used for min_width & minus & 0 flags
 * pad for dot
 *
 * if width set but minus off, width is for min_width */

typedef struct s_flags
{
	unsigned char	minus:1;
	unsigned char	zero:1;
	unsigned char	dot:1;
	unsigned char	sharp:1;
	unsigned char	space:1;
	unsigned char	plus:1;
	ssize_t			width;
	ssize_t			pad;
	int				n_star;
}				t_flags;

typedef struct s_node
{
	t_type			type;
	size_t			len;
	void			*content;
	struct s_node	*next;
}				t_node;

typedef struct s_buffer
{
	size_t	tot_len;
	t_node	*first;
}				t_buffer;

/************************** BUFFER & NODE FUNCTIONS ***************************/

t_buffer		*buff_init(void);
bool			buff_add_back(t_buffer *buf, t_node *new);
bool			buff_add_after(t_buffer *buf, t_node *node, t_node *new);
bool			buff_add_before(t_buffer *buf, t_node *node, t_node *new);
void			*buff_clear(t_buffer **buf);
size_t			buff_print(t_buffer *buf);
size_t			buff_special_print(t_buffer *buf, int count);

t_node			*node_get_next_conversion(t_node *node);
t_node			*node_new(t_type type, size_t len, void *content);

/************************* FLAGS PARSING FUNCTIONS ****************************/

bool			flags_get(t_flags *flags, char *str, int *i, va_list args);
t_flags			*flags_init(void);

/****************************** UTILS FUNCTIONS *******************************/

char			*ft_itoa_printf(int n, t_flags *f);
char			*ft_utoa_printf(unsigned int n);
char			*ft_utohex_printf(unsigned long n, bool lower);
long			ft_atol(const char *nptr);
char			*ft_ftoa_printf(double n, int precision, t_flags *f);

/********************************** HELPER ************************************/

bool			get_int_uint_adds(char **sp, char **zer, t_flags *f);
bool			get_float_adds(char **sp, t_flags *f);

/****************************** CONVERSIONS PART ******************************/

bool			convert_buffer(t_buffer *buf, va_list args);

bool			handle_char(t_buffer *buf, t_node *node, int c);
bool			handle_str(t_buffer *buf, t_node *node, const char *str);
bool			handle_percent(t_buffer *buf, t_node *node);
bool			handle_int(t_buffer *buf, t_node *node, int arg);
bool			handle_uint(t_buffer *buf, t_node *node, unsigned int arg);
bool			handle_hex(t_buffer *buf, t_node *node, unsigned int arg);
bool			handle_ptr(t_buffer *buf, t_node *node, void *ptr);
bool			handle_float(t_buffer *buf, t_node *node, float arg);

/********************************* FT_PRINTF **********************************/

int				ft_printf(const char *format, ...);

#endif
