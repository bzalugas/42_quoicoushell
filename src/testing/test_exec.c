/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 16:45:52 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/17 14:37:09 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/quoicoushell.h"
#include "cutest/CuTest.h"

static void	free_cmd2(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	free_split(cmd->argv);
	free(cmd->redir_in);
	free(cmd->redir_out);
	free(cmd->lim_heredoc);
}

static void	free_cmds2(t_lstcmds *cmds)
{
	ft_lstclear(&cmds->cmds, free_cmd2);
	free_split(cmds->env);
	free_split(cmds->paths);
}

void	onetest_exec_stdout(CuTest *tc, t_lstcmds *cmds, t_shell *sh, char *ex)
{
	int		fd[2];
	pid_t	pid;
	char	*buf;
	int		buflen;

	if (!ex)
		buflen = 50;
	else
		buflen = ft_strlen(ex) * 2;
	if (pipe(fd) == -1)
		exit(errno);
	pid = fork();
	if (pid == -1)
		exit(errno);
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		run_all_cmds(cmds, sh);
		free_cmds2(cmds);
		ft_lstclear(&sh->env_vars, free_variable);
		ft_lstclear(&sh->local_vars, free_variable);
		free(sh->cwd);
		exit(0);
	}
	close(fd[1]);
	buf = ft_calloc(buflen, sizeof(char));
	read(fd[0], buf, buflen);
	close(fd[0]);
	free_cmds2(cmds);
	ft_lstclear(&sh->env_vars, free_variable);
	ft_lstclear(&sh->local_vars, free_variable);
	free(sh->cwd);
	CuAssertStrEquals(tc, ex, buf);
}

void	test_exec_basic1(CuTest *tc)
{
	t_lstcmds	cmds;
	t_cmd		cmd;
	t_shell		sh;
	char		**paths;

	sh = (t_shell){0};
	paths = (char **)malloc(sizeof(char *) * 2);
	paths[0] = strdup("/usr/bin/");
	paths[1] = NULL;
	cmds = (t_lstcmds){NULL, .fd[0][0] = -1, .fd[0][1] = -1, .fd[1][0] = -1,
		.fd[1][1] = -1, .n_cmds = 1, .env = NULL, .paths = paths};
	cmd = (t_cmd){0, 2, ft_split("cat'src/testing/test_files/input1'", '\''), NULL, NULL, false, NULL};
	ft_lstadd_back(&cmds.cmds, ft_lstnew(&cmd));
	onetest_exec_stdout(tc, &cmds, &sh, "Hello everyone!");
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
	free(output->buffer);
	free(output);
	free(suite);
}

int	main(void)
{
	run_all_tests();
	return (0);
}
