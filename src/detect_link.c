/*
** EPITECH PROJECT, 2019
** detect_link
** File description:
** detect_link
*/

#include "my.h"
#include "minishell.h"

bool detect_if_arg(char *arg)
{
    int i = 0;

    for (i = 0; arg && arg[i] && (arg[i] == ' ' || arg[i] == '\t'); i++);
    if (arg == NULL || (arg && !arg[i])) {
        my_puterror("Invalid null command.\n");
        return (false);
    }
    return (true);
}

char **reverse_order(char **tab, int min, int max)
{
    int len = 0;
    char **result = NULL;
    int i = 0;

    for (len = min; len < max + 1 && tab && tab[len]; len++);
    if ((result = malloc(sizeof(char *) * len)) == NULL)
        return (NULL);
    for (len = min; len < max + 1 && tab && tab[len]; len++) {
        if ((result[i] = my_strdup(tab[len])) == NULL)
            return (NULL);
        i++;
    }
    result[i] = NULL;
    for (i--; i >= 0 && result && result[i] && tab && tab[min]; i--) {
        if ((tab[min] = my_strdup(result[i])) == NULL)
            return (NULL);
        min++;
    }
    return (tab);
}

bool detect_pipes(command *arg, char *str)
{
    int j = 0;
    int mem = 0;

    for (int i = 0; str && str[i]; j++) {
        mem = j;
        for (; str[i] && str[i] != ';'; i++)
            if (str[i] == '|') {
                j++;
                if (detect_if_arg(arg->instance[j]) == false)
                    return (false);
            }
        if (mem != j)
            arg->instance = reverse_order(arg->instance, mem, j);
        if (arg->instance == NULL)
            return (false);
        i++;
    }
    return (true);
}

bool detect_link(command *arg, char *str)
{
    int j = 0;

    for (int i = 0; str && str[i]; j++) {
        if ((arg->instance = my_taballoc(arg->instance)) == NULL)
            return (false);
        if ((arg->instance[j] = malloc(sizeof(char) * my_strlen(str))) == NULL)
            return (false);
        for (; str[i] == ' ' || str[i] == '\t'; i++);
        for (int k = 0; str[i] && str[i] != ';' && str[i] != '|'; k++) {
            arg->instance[j][k] = str[i];
            i++;
        }
        i++;
    }
    if (detect_pipes(arg, str) == false)
        return (false);
    return (true);
}
