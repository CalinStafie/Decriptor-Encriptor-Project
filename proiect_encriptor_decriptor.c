#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main(int argc, char **argv)
{
    int sursa, destinatie, permutari, in, k, i, j, a;
    char buffer[10];
    char cuvant[10000];
    int page_size = getpagesize();
    sursa = open(argv[1], O_RDONLY);
    if(sursa < 0)
        printf("Fisierul sursa lipsa.");
    destinatie = open("destinatie.txt", O_RDWR | O_CREAT | O_TRUNC, 0000700);

    int numar_cuvinte = 1;
    memset(buffer, 0, 10);
    memset(cuvant, 0, 10000);
    k = 0;
    while(in = read(sursa, buffer, 1) > 0)
    {
        if(buffer[0] == ' ')
        {

            if(k != 0)
            {
                ++numar_cuvinte;

                memset(buffer, 0, 10);
                memset(cuvant, 0, 10000);
                k = 0;
            }
        }
        else
            cuvant[k++] = buffer[0];
    }

    if(argc == 2)
    {
        permutari = open("permutari.txt", O_RDWR | O_CREAT | O_TRUNC, 0000700);

        char shm_name[] = "shmem";
        int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR  | S_IWUSR);

        if(shm_fd < 0)
        {
            perror(NULL);
            return errno;
        }
        size_t dimens = page_size * numar_cuvinte;
        if(ftruncate(shm_fd, dimens) == -1)
        {
            perror(NULL);
            shm_unlink(shm_name);
            return errno;
        }


        sursa = open(argv[1], O_RDONLY);
        char *shm_ptr;
        for(i = 0; i < numar_cuvinte; ++i)
        {
            shm_ptr = mmap(0, page_size, PROT_WRITE, MAP_SHARED, shm_fd, page_size * i);
            if(shm_ptr == MAP_FAILED)
            {
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }
            memset(buffer, 0, 10);
            k = 0;
            while(in = read(sursa, buffer, 1) > 0)
            {
                if(buffer[0] == ' ')
                {
                    if(k != 0)
                        break;
                }
                else
                {
                    ++k;
                    shm_ptr += sprintf(shm_ptr, "%c", buffer[0]);
                }
            }

            munmap(shm_ptr, page_size);

        }

        for(i = 0; i < numar_cuvinte; ++i)
        {
            shm_ptr = mmap(0, page_size, PROT_READ, MAP_SHARED, shm_fd, page_size * i);

            pid_t pid = fork();
            if(pid < 0)
                return errno;
            else
                if(pid == 0)
                {
                    int lungime_cuvant = strlen(shm_ptr);
                    int v[lungime_cuvant], n = lungime_cuvant;
                    char criptat[lungime_cuvant + 10];

                    for(j = 0; j < n; ++j)
                        v[j] = j + 1;

                    k = 0;
                    while(n)
                    {
                        srand(time(NULL));
                        int index = rand() % n;
                        int numar = v[index];

                        int aux;
                        aux = v[index];
                        v[index] = v[n-1];
                        v[n-1] = aux;
                        --n;

                        criptat[k++] = shm_ptr[numar-1];

                        char s[20];
                        sprintf(s, "%d", numar);
                        write(permutari, s, strlen(s));
                        write(permutari, " ", 1);
                    }


                    criptat[lungime_cuvant] = ' ';
                    criptat[lungime_cuvant+1] = 0;

                    write(destinatie, criptat, strlen(criptat));
                    write(permutari, "\n", 1);

                    exit(0);
                }
                else
                    wait(NULL);

            munmap(shm_ptr, page_size);
        }
        shm_unlink(shm_name);
    }

    else
        if(argc == 3)
        {

            char shm_name[] = "shmem";
            char shm_name2[] = "shmem2";

            int shm_fd2 = shm_open(shm_name2, O_CREAT | O_RDWR, S_IRUSR  | S_IWUSR);
            int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR  | S_IWUSR);
            if(shm_fd < 0)
            {
                perror(NULL);
                return errno;
            }
            size_t dimens = page_size * numar_cuvinte;
            if(ftruncate(shm_fd, dimens) == -1)
            {
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }


            if(shm_fd2 < 0)
            {
                perror(NULL);
                return errno;
            }

            if(ftruncate(shm_fd2, dimens) == -1)
            {
                perror(NULL);
                shm_unlink(shm_name2);
                return errno;
            }

            sursa = open(argv[1], O_RDONLY);

            char *shm_ptr;
            for(i = 0; i < numar_cuvinte; ++i)
            {
                shm_ptr = mmap(0, page_size, PROT_WRITE, MAP_SHARED, shm_fd, page_size * i);

                if(shm_ptr == MAP_FAILED)
                {
                    perror(NULL);
                    shm_unlink(shm_name);
                    return errno;
                }

                memset(buffer, 0, 10);
                k = 0;
                while(in = read(sursa, buffer, 1) > 0)
                {
                    if(buffer[0] == ' ')
                    {
                        if(k != 0)
                            break;
                    }
                    else
                    {
                        ++k;
                        shm_ptr += sprintf(shm_ptr, "%c", buffer[0]);
                    }
                }

                munmap(shm_ptr, page_size);
            }
            close(sursa);

            permutari = open(argv[2], O_RDONLY);
            int *shm_perm;
            for(i = 0; i < numar_cuvinte; ++i)
            {
                shm_perm = mmap(0, page_size, PROT_WRITE, MAP_SHARED, shm_fd2, page_size * i);

                if(shm_perm == MAP_FAILED)
                {
                    perror(NULL);
                    shm_unlink(shm_name2);
                    return errno;
                }

                a = 0;
                memset(buffer, 0, 10);
                memset(cuvant, 0, 10000);
                k = 0;
                while(in = read(permutari, buffer, 1) > 0)
                {
                    if(buffer[0] == ' ')
                    {
                        if(k != 0)
                        {
                            shm_perm[a++] = atoi(cuvant);
                            k = 0;
                            memset(cuvant, 0, 10000);
                        }
                    }
                    else
                        if(buffer[0] == '\n')
                            break;
                        else
                            cuvant[k++] = buffer[0];
                }
                shm_perm[a++] = atoi(cuvant);

                munmap(shm_perm, page_size);
            }
            close(permutari);

            for(i = 0; i < numar_cuvinte; ++i)
            {
                shm_ptr = mmap(0, page_size, PROT_READ, MAP_SHARED, shm_fd, page_size * i);
                shm_perm = mmap(0, page_size, PROT_READ, MAP_SHARED, shm_fd2, page_size * i);
                pid_t pid = fork();
                if(pid < 0)
                    return errno;
                else
                    if(pid == 0)
                    {
                        int lungime_cuvant = strlen(shm_ptr);
                        char decriptat[lungime_cuvant + 10];


                        for(j = 0; j < lungime_cuvant; ++j)
                            decriptat[shm_perm[j]-1] = shm_ptr[j];


                        decriptat[lungime_cuvant] = ' ';
                        decriptat[lungime_cuvant+1] = 0;

                        write(destinatie, decriptat, strlen(decriptat));

                        exit(0);
                    }
                    else
                        wait(NULL);

                munmap(shm_ptr, page_size);
                munmap(shm_perm, page_size);
            }

            shm_unlink(shm_name);
            shm_unlink(shm_name2);
        }

    close(sursa), close(destinatie), close(permutari);
    return 0;
}
