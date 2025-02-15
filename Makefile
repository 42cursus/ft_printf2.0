# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/05 00:46:36 by abelov            #+#    #+#              #
#    Updated: 2023/12/05 00:46:38 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


INC_DIR		=	include

NAME		=	libftprintf.a
LIBTMP		=	libtmp.a
CC			=	cc
CFLAGS		=	-g3 -Wall -Wextra -Werror -iquote $(INC_DIR)
AR			=	ar
ARFLAGS		=	rcs
LIBFT_PATH	=	./lib/libft
LIBFT		=	$(LIBFT_PATH)/libft.a
RM 			:=	rm -rf

SRC_FILES	:= ft_printf.c \
				ft_printf_utils1.c \
				ft_printf_utils2.c \
				ft_printf_utils3.c \
				ft_printf_utils4.c \
				ft_print_hex.c \
				ft_print_unsigned.c \
				ft_vprintf.c \
				ft_vsprintf.c


OBJ_DIR		=	obj
SRC_DIR		=	src
OBJS		=	$(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

all:				$(NAME)

$(OBJ_DIR)/%.o:		$(SRC_DIR)/%.c
					$(CC) $(CFLAGS) -c $< -o $@

$(NAME):			$(LIBFT) $(OBJ_DIR) $(OBJS)
					@$(AR) $(ARFLAGS) $(OBJ_DIR)/$(LIBTMP) $(OBJS)
					@$(CC) -nostdlib -r -o $(OBJ_DIR)/$(NAME) $(OBJ_DIR)/ft_printf.o \
						-Wl,--no-whole-archive $(OBJ_DIR)/$(LIBTMP) $(LIBFT)
					@$(AR) $(ARFLAGS) $(NAME) $(OBJ_DIR)/$(NAME)

$(LIBFT):
					@$(MAKE) -C $(LIBFT_PATH) --no-print-directory -j8 all

$(OBJ_DIR):
					@mkdir -pv $(OBJ_DIR)

clean:
					@$(MAKE) -C $(LIBFT_PATH) --no-print-directory clean
					@$(RM) $(OBJ_DIR) a.out 

fclean:				clean
					@$(MAKE) -C $(LIBFT_PATH) --no-print-directory fclean
					@$(RM) $(NAME)

re:					fclean all 

norm:
					@norminette $(SRC_FILES:%.c=$(SRC_DIR)/%.c) $(INC_DIR)/*
					@$(MAKE) -C $(LIBFT_PATH) --no-print-directory norm

.PHONY:				all bonus clean fclean re libft norm
