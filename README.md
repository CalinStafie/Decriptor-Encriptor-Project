# Decriptor-Encriptor Project

Pentru criptare, programul primeste un fisier de cuvinte pe care le proceseaza si face o permutare random al fiecarui cuvant si apoi se va scrie intr-un fisier de iesire toate cuvintele criptate cu ajutorul permutarilor random si va scrie in alt fisier toate permutarile random generate.

Pentru descriptare, programul va primi un fisier de cuvinte criptate si un alt fisier care contine permutarile literelor fiecarui cuvant, se vor scrie cuvintele decriptate intr-un fisier de iesire.

Cerinta: 
Sa se implementeze un encriptor/decriptor care primeste un fisier de intrare cu diferite cuvinte. Programul mapeaza fisierul de intrare in memorie si porneste mai multe procese care vor aplica o permutare random pentru fiecare cuvant. Permutarile vor fi scrise intr-un fisier de iesire. Programul poate primi ca argument doar fisierul de intrare, in acest caz va face criptarea cuvintelor; sau va primi fisierul avand cuvintele criptate si permutarile folosite pentru criptare, caz in care va genera fisierul de
output avand cuvintele decriptate.

Pentru rezolvarea acestei cerinte am folosit urmatoarele concepte: shared_memory pentru mai multe procese, maparea in memorie a cuvintelor, manipularea fisierelor si diferite operatii pe fisiere.

Pentru a rula programul este de preferat sa fie rulat intr-un Terminal al unui sistem de operare de tip UNIX, cu urmatoarele comenzi:

```
gcc proiect_encriptor_decriptor.c
```
Pentru criptare: 
```
./a.out intrare.txt
```
Pentru decriptare: 
```
./a.out intrare.txt permutari.txt
```
Mai jos am selectat cateva randuri pentru a arata cum am folosit conceptul de shared_memory.
```c
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

```

```c
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
```

La partea de procese, creez pentru fiecare data un copil pentru fiecare cuvant pe care il iau din shared_memory.
```c
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
```
