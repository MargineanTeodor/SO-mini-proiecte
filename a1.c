#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> // biblioteci
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int listare3(char *path ,int size, char *str) // listare iterativa
{
	DIR * dir= NULL;
	struct dirent * en= NULL;
	dir= opendir(path);
	if(dir== NULL)
	{
		perror("ERROR\n"); // daca nu se poate citi din fisier
		return 0;
	}
	if(size!=-1)
	{
        struct stat file_info;
        char fullPath[1000];
        printf("SUCCESS\n");
        while((en=readdir(dir))!=NULL)
        {
            if(strcmp(en->d_name, ".")!=0 && strcmp(en->d_name, "..")!=0) // se verifica ca sa nu apara  bucle
            {
                snprintf(fullPath,512,"%s/%s",path,en->d_name);
                if(lstat(fullPath,&file_info) == 0)
                    if(S_ISREG(file_info.st_mode))
                        if(file_info.st_size<size)
                            printf("%s\n",fullPath);// afisare fisier

            }
        }
    }
    else
    {
        if(str!=NULL)
        {
            char *s=(char*)malloc(strlen(str));
            printf("SUCCESS\n");
            while((en=readdir(dir))!=NULL)
            {
                if(strcmp(en->d_name, ".")!=0 && strcmp(en->d_name, "..")!=0) // se verifica ca sa nu apara  bucle
                {
                    strncpy(s,en->d_name,strlen(str));
                    if(strcmp(s,str)==0)
                        printf("%s/%s\n",path,en->d_name);
                }
            }
            free(s);
        }
    }
	closedir(dir);
	return 1;
}
int listare1(char *path) // listare iterativa
{
	DIR * dir= NULL;
	struct dirent * en= NULL;
	struct stat file_info;
	dir= opendir(path);
	char fullPath[512];
	if(dir== NULL)
	{
		perror("ERROR\n"); // daca nu se poate citi din fisier
		return 0;
	}
	printf("SUCCESS\n");
	while((en=readdir(dir))!=NULL)
	{
		if(strcmp(en->d_name, ".")!=0 && strcmp(en->d_name, "..")!=0) // se verifica ca sa nu apara  bucle
		{
			snprintf(fullPath,512,"%s/%s",path,en->d_name); // se face fullpath-ul
			if(lstat(fullPath,&file_info) == 0)
				printf("%s\n",fullPath);// afisare fisier
		}
	}
	closedir(dir);
	return 1;
}
int listare2(char *path) // listare fara conditii recursiva
{
    DIR* dir = NULL;
    struct dirent *entry = NULL;
    struct stat file_info; // initializari
    char fullPath[512];
    dir =  opendir(path); // deschidere fisier
    if(dir == NULL)
    {
        perror("ERROR\n");
    	return -1;
    }
    while((entry = readdir(dir)) != NULL) // cat timp se paote citi din fisier
    {
        if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) // ca sa nu se faca bulce infinte
        {
            snprintf(fullPath,512,"%s/%s",path,entry->d_name);
            if(lstat(fullPath,&file_info) == 0)
            {
                printf("%s\n",fullPath);
                if(S_ISDIR(file_info.st_mode))// daca este fisier apealm recursiv ca in exemplul de laborator.
                {
                   listare2(fullPath);
                }
            }

        }
    }
    closedir(dir);
    return 1;
}
int listare4(char *path, int size, char *str)//listare cu conditii recursiva
{
    DIR* dir = NULL;
    struct dirent *entry = NULL;
    dir =  opendir(path); // deschidere fisier
    if(dir == NULL)
    {
    	return -1;
    }
    if(size!=-1)
    {
        char fullPath[1000];
        struct stat file_info;
        while((entry = readdir(dir)) != NULL) // cat timp se paote citi din fisier
        {
            if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) // ca sa nu se faca bulce infinte
            {
                snprintf(fullPath,512,"%s/%s",path,entry->d_name);
                if(lstat(fullPath,&file_info) == 0)
                {
                    if(S_ISREG(file_info.st_mode))
                        if(file_info.st_size<size) // daca e fier normal
                                printf("%s\n",fullPath);
                    if(S_ISDIR(file_info.st_mode)) // daca e registru
                        listare4(fullPath,size,str);
                }

            }
        }
    }
    else
    {
        if(str!=NULL)
        {
            char fullPath[1000];
            struct stat file_info;
            char *s=(char*)malloc(strlen(str));
            while((entry = readdir(dir)) != NULL) // cat timp se paote citi din fisier
            {
                if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) // ca sa nu se faca bulce infinte
                {
                    snprintf(fullPath,512,"%s/%s",path,entry->d_name);
                    if(lstat(fullPath,&file_info) == 0)
                    {
                        strncpy(s,entry->d_name,strlen(str));
                        if(strcmp(s,str)==0)
                                printf("%s\n",fullPath);
                        listare4(fullPath,size,str);
                    }
                }
            }
            free(s);

        }
    }
    closedir(dir);
    return 1;
}
int pathing(int argc, char ** argv, char ** path)
{
 	char *p;
	char s[]="path=";
	for(int i=2;i<argc ;i++)
	{
		p=strstr(argv[i],s);
		if(p)
		{
			*path=(char*)malloc(strlen(argv[i]));
			strncpy(*path,argv[i]+5,strlen(argv[i])-4);
			return 1;
		}
	}
	return 0;
}
int getSize(int argc,char **argv)
{
    char s[]="size_smaller=";
    char *p;
    int size=0;
    for(int i=2;i<argc;i++)
    {
        p=strstr(argv[i],s);
        if(p)
        {
			size=atoi(argv[i]+13);
			return size;
        }
    }
    return -1;
}
int getStr(int argc,char **argv, char **str)
{
    char s[]="name_starts_with=";
    char *p;
    for(int i=2;i<argc;i++)
    {
        p=strstr(argv[i],s);
        if(p)
        {
            *str=(char *)malloc(strlen(argv[i]));
            strncpy(*str,argv[i]+17,strlen(argv[i]));
            return 1;
        }
    }
    *str=NULL;
    return 0;
}
int list(int argc, char **argv)
{
	char *path;
	int size=-1;
	pathing(argc,argv ,&path);
    char *str=NULL;
	size=getSize(argc,argv); // preluam  din argumente datele 
	getStr(argc,argv,&str);
	if(path==NULL)
	{
		perror("ERROR\n");
		free(path);
		free(str);  // erarea 
		return 0;
	}
	if(argc==3)
	{
		listare1(path);
		free(path);
		free(str); // listare ptr iterativa fara conditii
		return 1;
	}
	else if(argc==4)
		{
		for(int i=2;i<argc;i++)
			if(strcmp (argv[i], "recursive")==0)
			{
			printf("SUCCESS\n");
			listare2(path);
			free(path); // listare recursiva fara conditie
			free(str);
			return 1;
			}
        listare3(path,size,str);
		free(path);
		free(str); // listare iterativa cu conditie
		return 1;
		}
    else if(argc==5)
    {
        printf("SUCCESS\n");
        listare4(path,size,str);
        free(path);
        free(str); // listare recursiva cu conditie
        return 1;
    }
	return 1;
}
int parse(int argc ,char ** argv)
{
    char *path;
    pathing(argc,argv ,&path);
    int fd=open(path,O_RDONLY); // se deschide fisierul in modul de  citire
    lseek(fd,-1,SEEK_END);
    char magictest[2];
    read(fd,magictest,1);
    magictest[1]='\0';
    if(strcmp("U",magictest)!=0)
    {
        printf("ERROR\n");
        printf("wrong magic");// se verifica magic
        free(path);
        return 1;
    }
    lseek(fd,-3,SEEK_END);
    int header_size=0;  // se ia header size
    read(fd,&header_size,2);
    lseek(fd,header_size*-1,SEEK_END);
    int version=0;
    read(fd,&version,2);
    if(version<68|| version>151)
    {
        printf("ERROR\nwrong version");// se verifica versiunea 
        free(path);
        return 1;
    }
    int no_of_sections=0;
    read(fd,&no_of_sections,1);
    if(no_of_sections<7 || no_of_sections >20)
    {
        printf("ERROR\nwrong sect_nr"); // se verifica nr de sectiuni 
        free(path);
        return 1;
    }
    char name[no_of_sections][22];
    int  type[no_of_sections];
    int  offset[no_of_sections]; // se preiau date despre sectiuni 
    int size[no_of_sections];
    for(int i=0;i<no_of_sections;i++)
    {
        read(fd,name[i],20);
        name[i][20]='\0';
        read(fd,&type[i],4);
        if(type[i]<11 || type[i]>79)// se verifica tipul sectiunilor 
        {
            printf("ERROR\nwrong sect_types");
            free(path);
            return 1;
        }
        read(fd,&offset[i],4);
        read(fd,&size[i],4);
    }
    printf("SUCCESS\n");
    printf("version=%d\n",version);
    printf("nr_sections=%d\n",no_of_sections);

    for(int i=0;i<no_of_sections;i++) // se fac afisarile 
    {
        printf("section%d: %s %d %d\n",i+1,name[i],type[i],size[i]);
    }
    free(path);
    return 1;
}
int extract(int argc, char **argv)
{
     char * path;
     pathing(argc,argv,&path);// se preia path-ul
     int section=0,line=0;
     section=atoi(argv[3]+8);
     line=atoi(argv[4]+5);// se preiau linia si sectiunea 
    int fd=open(path,O_RDONLY);
    if(fd==-1)
    {
        printf("ERROR\n invalid file");
        free(path);
        return 1;
    }
    lseek(fd,-1,SEEK_END);
    char magictest[2];
    read(fd,magictest,1);
    magictest[1]='\0';
    if(strcmp("U",magictest)!=0)
    {
        printf("ERROR\n"); // se verifica magic 
        printf("invalid file");
        free(path);
        return 1;
    }
    lseek(fd,-3,SEEK_END);
    int header_size=0; 
    read(fd,&header_size,2);
    lseek(fd,header_size*-1,SEEK_END); // se ia header size si se  muta cursorul la inceputul headerului
    int version=0;
    read(fd,&version,2);
    if(version<68|| version>151)
    {
        printf("ERROR\ninvalid file");
        free(path);// se verifica versiunea 
        return 1;
    }
    int no_of_sections=0;
    read(fd,&no_of_sections,1);
    if((no_of_sections<7 || no_of_sections >20))
    {
        printf("ERROR\ninvalid file");
        free(path);// se verifica nr de sectiuni 
        return 1;
    }
    if(section-1>no_of_sections || section-1<0)
    {
        printf("ERROR\ninvalid section");
        free(path); // se verfica nr de sectiuni  trimis ca argument 
        return 1;
    }
    char name[no_of_sections][22];
    int  type[no_of_sections];
    int  offset[no_of_sections];// se preiau date despre sectiuni 
    int size[no_of_sections];
    for(int i=0;i<no_of_sections;i++)
    {
        read(fd,name[i],20);
        name[i][20]='\0';
        read(fd,&type[i],4);
        if(type[i]<11 || type[i]>79)
        { // se verifica tipul sectiunilor
            printf("ERROR\ninvalid file");
            free(path);
            return 1;
        }
        read(fd,&offset[i],4);
        read(fd,&size[i],4);
    }
    lseek(fd,offset[section-1],SEEK_SET);
    int count=1;
    int count2=0;
    char k[2];
    char linie[size[section-1]];
    for(int i=0;i<size[section-1];i++)
    {
        read(fd,k,1);
        if(k[0]=='\n')// se contorizeaza numarul de newline uri 
            count++;
        else 
            if(count==line)
                linie[count2++]=k[0]; // daca e egal cu argumentul trimis se stocheaza intr-un string;
    }
    if(count<line)
    {
        printf("ERROR\ninvalid line"); // se verifica argumentul (nr de lini din SF)
        return 1;
    }
    else 
    {
        printf("SUCCESS\n");
        for(int i=count2-1;i>=0;i--)
        {
            printf("%c",linie[i]); // se afiseaza invers
        }       
    }
    free(path);
     return 1;
}
int findall2(char* path)
{
    DIR* dir = NULL;
    struct dirent *entry = NULL;
    struct stat file_info; // initializari
    char fullPath[512];
    dir =  opendir(path); // deschidere fisier
    if(dir == NULL)
    {
        closedir(dir);
    	return -1;
    }
    while((entry = readdir(dir)) != NULL) // cat timp se paote citi din fisier
    {
        if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) // ca sa nu se faca bulce infinte
        {
            snprintf(fullPath,512,"%s/%s",path,entry->d_name);
            if(lstat(fullPath,&file_info) == 0)
            {
                if(S_ISDIR(file_info.st_mode))// daca este fisier apealm recursiv ca in exemplul de laborator.
                {
                   findall2(fullPath);
                }
                else
                {
                    int fd=open(fullPath,O_RDONLY);
                    lseek(fd,-1,SEEK_END);
                    char magictest[2];
                    read(fd,magictest,1);
                    magictest[1]='\0';
                    if(strcmp("U",magictest)!=0)
                    {
                        closedir(dir);
                        return 1; // verificare magic
                    }
                    lseek(fd,-3,SEEK_END);
                    int header_size=0;  // preluare size la header
                    read(fd,&header_size,2);
                    lseek(fd,header_size*-1,SEEK_END); // mutam cursorul la inceput de header 
                    int version=0;
                    read(fd,&version,2);
                    if(version<68|| version>151)
                    {
                        closedir(dir);
                        return 1; // verificam versiunea 
                    }
                    int no_of_sections=0;
                    read(fd,&no_of_sections,1);
                    if(no_of_sections<7 || no_of_sections >20)
                    {
                        closedir(dir);
                        return 1; // verificam nr de sectiuni 
                    }
                    char name[no_of_sections][22];
                    int  type[no_of_sections];
                    int  offset[no_of_sections];
                    int size[no_of_sections];// preluare date din sectiuni
                    for(int i=0;i<no_of_sections;i++)
                    {
                        read(fd,name[i],20);
                        name[i][20]='\0';
                        read(fd,&type[i],4);
                        if(type[i]<11 || type[i]>79)
                        {
                            closedir(dir);
                            return 1; // verificare tip sectiuni
                        }
                        read(fd,&offset[i],4);
                        read(fd,&size[i],4);
                    }
                    int sem=0;
                    for(int i=0;i<no_of_sections;i++)
                    {
                        int count=1;
                        char buff[size[i]+1];
                        lseek(fd,offset[i],SEEK_SET);
                        read(fd,buff,size[i]);
                        buff[size[i]+1]='\0';
                        for(int j=0;j<=size[i];j++)
                        {
                            if(buff[j]=='\n') // verificat nr apariti newline
                                count++;
                        }
                        if(count>15) // verificat sa fie mai mare de 15 
                            sem=1;
                    }
                    if(sem==1)
                        printf("%s\n",fullPath);
                }
            }

        }
    }
    closedir(dir);
    return 1;
}
int findall(int argc,char**argv)
{
    char* path;
    pathing(argc,argv,&path); // ia path-ul din argumente
    DIR* dir = NULL;
    dir =  opendir(path); // deschidere fisier si verifica ptr eroare
    if(dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        free(path);
        closedir(dir);
        return 1;
    }
    closedir(dir);
    printf("SUCCESS\n");
    findall2(path); // apeleaza functia ca sa afiseze recursiv fisierele
    free(path);
    return 1;
}
int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){ // afisarea varianta
            printf("41792\n");
        }
        else if(strcmp(argv[1], "list") == 0){
        	list(argc, argv);  // list
    	}
    	else if(strcmp(argv[1], "parse")== 0){
            parse(argc,argv); // parse
    	}
        else if (strcmp(argv[1],"extract")==0)
        {
            extract(argc,argv);// extract 
        }
        else if(strcmp(argv[1],"findall")==0)
        {
            findall(argc,argv);// findall
        }
    }
    return 0;
}
