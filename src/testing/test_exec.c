/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 16:45:52 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/13 17:48:43 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/quoicoushell.h"
#include "cutest/CuTest.h"

void	test_exec_basic1(CuTest *tc)
{
	int			fd[2];
	pid_t		pid;
	t_lstcmds	cmds;
	t_cmd		cmd;
	char		*paths[2];
	char		buf[4096];

	paths[0] = strdup("/usr/bin/");
	paths[1] = NULL;
	cmds = (t_lstcmds){NULL, .fd[0][0] = -1, .fd[0][1] = -1, .fd[1][0] = -1,
		.fd[1][1] = -1, .n_cmds = 1, .env = NULL, .paths = paths};
	cmd = (t_cmd){0, ft_split("echo'Hello everyone!'", '\''), NULL, NULL, false, NULL};
	ft_lstadd_back(&cmds.cmds, ft_lstnew(&cmd));
	if (pipe(fd) == -1)
		exit (errno);
	pid = fork();
	if (pid == -1)
		exit (errno);
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		run_all_cmds(&cmds);
	}
	close(fd[1]);
	bzero(buf, 4096);
	read(fd[0], &buf, 4095);
	close(fd[0]);
	CuAssertStrEquals(tc, "Hello everyone!\n", buf);
}

static CuSuite *exec_getSuite()
{
	CuSuite	*s = CuSuiteNew();

	SUITE_ADD_TEST(s, test_exec_basic1);

	return (s);
}

static void	run_all_tests(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, exec_getSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int	main(void)
{
	run_all_tests();
	return (0);
}
