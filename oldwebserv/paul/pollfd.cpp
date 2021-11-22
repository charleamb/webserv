#include "server.hpp"

void deleteOne(struct pollfd **tab, size_t *size, int fd_to_delete)
{
    if (!tab || !*tab || !size || *size == 0)
        return ;
    bool found = 0;
    for (size_t i = 0; i < *size; i++)
        if ((*tab)[i].fd == fd_to_delete)
        {
            found = true;
            break ;
        }
    if (!found)
        return ;

    struct pollfd *tmp = 0;
    if (*size > 1)
    {
        tmp = new struct pollfd[*size - 1];
        int count = 0;
        for (size_t j = 0; j < *size; j++)
        {
            if ((*tab)[j].fd != fd_to_delete)
            {
                tmp[count].fd = (*tab)[j].fd;
                tmp[count].events = (*tab)[j].events;
                tmp[count++].revents = (*tab)[j].revents;
            }
        }
    }
    delete[] *tab;
    if (*size == 1)
        *tab = 0;
    (*size)--;
    *tab = tmp;
    std::cout << "6\n";
}

void addOne(struct pollfd **tab, size_t *size, int fd_to_add)
{
    if (!tab || !*tab)
        return;
    struct pollfd *tmp = new struct pollfd[*size + 1];
    for (size_t i = 0; i < *size; i++)
    {
        tmp[i].fd = (*tab[i]).fd;
        tmp[i].events = (*tab[i]).events;
        tmp[i].revents = (*tab[i]).revents;
    }
    tmp[*size].fd = fd_to_add;
    tmp[*size].events = POLLIN;
    tmp[*size].revents = 0;
    delete[] *tab;
    (*size)++;
    *tab = tmp;
}