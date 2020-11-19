/*
** EPITECH PROJECT, 2019
** disp_prompt
** File description:
** disp_prompt
*/

#include "my.h"
#include "minishell.h"
#include "get_next_line.h"

char *clean_begining(char *str)
{
    int j = 0;
    int i = 0;
    char *result = malloc(sizeof(char) * (my_strlen(str) + 1));

    if (result == NULL)
        return (NULL);
    for (j = 0; str && str[j] && (str[j] == ' ' || str[j] == '\t'); j++);
    for (i = 0; str[j]; j++) {
        result[i] = str[j];
        i++;
    }
    result[i] = 0;
    return (result);
}

bool cut_string(char *str, int spaces, command *arg)
{
    int i = 0;

    arg->args = malloc(sizeof(char *) * (spaces + 1));
    if (arg->args == NULL)
        return (false);
    arg->args[0] = malloc(sizeof(char) * (my_strlen(str) + 1));
    for (i = 0; str && str[i] && (str[i] == ' ' || str[i] == '\t'); i++);
    arg = list_args(str, arg, i);
    if (arg == NULL)
        return (false);
    for (i = 0; arg->args[i]; i++)
        if (my_strlen(arg->args[i]) == 0)
            arg->args[i] = NULL;
    return (true);
}

char *manage_name(char *str, command *arg)
{
    bool redirect = false;
    int spaces = 1;

    if ((str = clean_begining(str)) == NULL || detect_if_exit(str))
        return (NULL);
    if (my_strlen(str) == 0)
        return (str);
    for (int i = 0; str && str[i]; i++)
        if (str[i] == ' ')
            spaces++;
    if (cut_string(str, spaces, arg) == false)
        return (NULL);
    for (int i = 0; str && str[i]; i++)
        if (str[i] == ' ' || str[i] == '\t')
            str[i] = 0;
    for (int i = 0; str && str[i]; i++)
        if (str[i] == '/')
            redirect = true;
    if (redirect == false)
        str = search_bin(arg, str);
    return (str);
}

bool exec_prompt(char *str, command *arg, pid_t father_pid, int status, int i, int old_dp[2])
{
    char *name = my_strdup(arg->instance[i]);
    int dp[2];

    if (detect_cases(arg, arg->instance[0]) == false && detect_cases(arg, arg->instance[1]) == false) {
        arg->instance[0] = manage_name(arg->instance[0], arg);
        arg->instance[1] = manage_name(arg->instance[1], arg);
        if (arg->instance[i] == NULL)
            return (false);
        if (my_strlen(arg->instance[i]) == 0)
            return (true);
        //if (check_access(arg->instance[i], name) == false)
        //return (true);
        //if (i == 0) {
        if (pipe(dp) != 0)
            return (true);
        father_pid = fork();
        //}
        if (father_pid == 0) {
            dup2(dp[0], 0);
            close(dp[1]);
            status = execve(arg->instance[0], arg->args, arg->env);
        }
        else {
            //waitpid(father_pid, &status, WUNTRACED | WCONTINUED);
            dup2(dp[1], 1);
            close(dp[0]);
            status = execve(arg->instance[1], arg->args, arg->env);
            check_status(status);
        }
    }
    return (true);
}

bool disp_prompt(command *arg)
{
    char *str = get_next_line(0);
    pid_t pid = 0;
    static int status = 0;
    int i = 0;
    int dp[2];

    if (str == NULL)
        return (false);
    if (detect_link(arg, str) == false)
        return (true);
    //for (i = 0; arg->instance && arg->instance[i]; i++) {
    if (exec_prompt(arg->instance[i], arg, pid, status, 0, dp) == false)
        return (false);
        //}
    return (true);
}
