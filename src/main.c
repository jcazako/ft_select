#include "ft_select.h"

static t_select	*ret_tree(void)
{
	static t_select	*data = NULL;

	if (!data)
	{
		if (!(data = (t_select*)malloc(sizeof(t_select))))
		{
			ft_printf("Error malloc\n");
			exit(2);
		}
	}
	return (data);
}

int	putit(int c)
{
	t_select	*data;
	
	data = ret_tree();
	ft_putchar_fd(c, data->fd);
	return (0);
}

static void	ft_resize(int c)
{
	t_select	*data;

	data = ret_tree();
	(void)c;
	tputs(tgetstr("cl", NULL), data->fd, putit);
	print_select(data);
}

int		main(int ac, char **av)
{
	t_select	*data;
	struct termios	term;
	char		*str_1;
	char		*str_2;
	
	data = ret_tree();
	if (!(data->tree = get_arg(ac, av)))
		return (1);
	if ((data->fd = open("/dev/tty", O_RDWR)) == -1)
	{
		ft_trdel(&data->tree, (void(*)(void*, size_t))del_elem);
		free(data);
		return (2);
	}
	if (!(data->pos = get_pos_lst(data->tree)))
	{
		close(data->fd);
		ft_trdel(&data->tree, (void(*)(void*, size_t))del_elem);
		free(data);
		return (3);
	}
	signal(SIGWINCH, ft_resize);
	tcgetattr(0, &term);
	term.c_lflag ^= (ECHO | ICANON);
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (3);
	str_1 = getenv("TERM");
	str_2 = NULL;
	tgetent(str_2, str_1);
	tputs(tgetstr("ti", NULL), data->fd, putit);
	tputs(tgetstr("vi", NULL), data->fd, putit);
	while (42)
	{
		tputs(tgetstr("cl", NULL), data->fd, putit);
		print_select(data);
		if (get_key(data))
			break ;
	}
	tputs(tgetstr("te", NULL), data->fd, putit);
	tputs(tgetstr("ve", NULL), data->fd, putit);
	display_selected(data);
	free_all(data);
	//while (42);
	return (0);
}
