
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



#define MAX_MEMORY 10000

char magic[3];



void listDir(char *dirName, bool isRecursive,bool isName,char *nameStartsWith,bool isPermission,char* permission)
{
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char name[MAX_MEMORY];

    dir = opendir(dirName);
    if (dir == 0) {
        return;
    }

    // iterate the directory contents
    while ((dirEntry=readdir(dir)) != 0) {

        if(strcmp(dirEntry->d_name,".") == 0 || strcmp(dirEntry->d_name,"..") == 0) //
            continue;
        // build the complete path to the element in the directory
        snprintf(name, MAX_MEMORY, "%s/%s",dirName,dirEntry->d_name);  //ia fiecare directory file in name 
        
        // get info about the directory's element
        lstat (name, &inode);


          char permissions[9] = "---------";


        if(!lstat(name,&inode))
        {

    if (inode.st_mode & S_IRUSR) permissions[0] = 'r';   //user
    if (inode.st_mode & S_IWUSR) permissions[1] = 'w';
    if (inode.st_mode & S_IXUSR) permissions[2] = 'x';

    if (inode.st_mode & S_IRGRP) permissions[3] = 'r';  //group r w x
    if (inode.st_mode & S_IWGRP) permissions[4] = 'w';
    if (inode.st_mode & S_IXGRP) permissions[5] = 'x';

    if (inode.st_mode & S_IROTH) permissions[6] = 'r';   //other
    if (inode.st_mode & S_IWOTH) permissions[7] = 'w';
    if (inode.st_mode & S_IXOTH) permissions[8] = 'x';

             if (S_ISDIR(inode.st_mode))
                {
                    
                    if(isName && strncmp(dirEntry->d_name,nameStartsWith,strlen(nameStartsWith)) == 0)
                    {
                        if(isPermission && strncmp(permissions,permission,strlen(permission))==0)
                        {
                            printf("%s\n", name);
                        }
                        else if
                        (!isPermission)
                        {
                            printf("%s\n", name);
                        }
                        
                    }
                    else if(!isName)
                    {
                       if(isPermission && strncmp(permissions,permission,strlen(permission))==0)
                        {
                            printf("%s\n", name);
                        }
                        else if
                        (!isPermission)
                        {
                            printf("%s\n", name);
                        }
                     }
                    
                    if(isRecursive)
                    {
                        listDir(name,isRecursive,isName,nameStartsWith,isPermission,permission);
                    }
                }
        else 
            if (S_ISREG(inode.st_mode))   //reg is for text files
                {
                if(isName && strncmp(dirEntry->d_name,nameStartsWith,strlen(nameStartsWith)) == 0)
                    {
                   if(isPermission && strncmp(permissions,permission,strlen(permission))==0)
                        {
                            printf("%s\n", name);
                        }
                        else if
                        (!isPermission)
                        {
                            printf("%s\n", name);
                        }
                    }
                    else if(isName == false)
                    {

                    if(isPermission && strncmp(permissions,permission,strlen(permission))==0)
                        {
                            printf("%s\n", name);
                        }
                        else if
                        (!isPermission)
                        {
                            printf("%s\n", name);
                        }
                     }
                    
                }

        
    }
 }

        // test the type of the directory's element
       closedir(dir);
}

/*MAGIC: 2
HEADER_SIZE: 2
VERSION: 2
NO_OF_SECTIONS: 1
SECT_NAME: 13
SECT_TYPE: 4
SECT_OFFSET: 4
SECT_SIZE: 4*/


void parse(char* path)
{

    int fileDescriptor;
    if(!(fileDescriptor = open(path,O_RDONLY)))   //DESCHID FD
    {
        printf("ERROR");
        return;
    }
char magic[3];
int heade_size =0;
int version =0;
int no_of_sections =0;
char sect_name[14];
int sect_type =0;
int sect_offset =0;
int sect_size =0;

read(fileDescriptor,&magic,2);
read(fileDescriptor,&heade_size,2);
read(fileDescriptor,&version,2);
read(fileDescriptor,&no_of_sections,1);

magic[2] = '\0';

if(strcmp(magic,"6P")!=0)
{
printf("ERROR\nwrong magic");
return;
}

if(!(version>=119 && version<=191))
{
     printf("ERROR\nwrong version");
return;
}

if(!(no_of_sections>=7 && no_of_sections<=20))
{
         printf("ERROR\nwrong sect_nr");
return;
}


int array[] ={57,83,90,16,23,27};
int arrayLenght = sizeof(array)/sizeof(array[0]);
for(int i=0;i<no_of_sections;i++)
{
read(fileDescriptor,&sect_name,13);
read(fileDescriptor,&sect_type,4);
read(fileDescriptor,&sect_offset,4);
read(fileDescriptor,&sect_size,4);

bool typeInArray = false;

for(int j=0;j<arrayLenght;j++)
    {   
        if(sect_type == array[j])
        {
            typeInArray = true;
        }
    }
    if(!typeInArray)
    {
       printf("ERROR\nwrong sect_types");
     return;
    }

}


/*SUCCESS
version=<version_number>
nr_sections=<no_of_sections>
section1: <NAME_1> <TYPE_1> <SIZE_1>
section2: <NAME_2> <TYPE_2> <SIZE_2>
*/


printf("SUCCESS\nversion=%d\nnr_sections=%d\n",version,no_of_sections);
lseek(fileDescriptor,7,SEEK_SET);
for(int i=1;i<=no_of_sections;i++)
{
read(fileDescriptor,&sect_name,13);
read(fileDescriptor,&sect_type,4);
read(fileDescriptor,&sect_offset,4);
read(fileDescriptor,&sect_size,4);

printf("section%d: %s %d %d\n",i,sect_name,sect_type,sect_size);
}


}

int main(int argc, char **argv){

    strncpy(magic,"6P",2);
    magic[2] = '\0';
    bool isRecursive = false;
    bool isName = false;
    bool isPermission = false;




    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("23178\n");
        }
        if(argc >=3)
        {

            //FOR LIST OPTION 
            if(strcmp(argv[1],"list") == 0)
            {
                char path[MAX_MEMORY];
                char nameStartsWith[MAX_MEMORY];
                char permission[MAX_MEMORY];
                for(int i=2 ;i< argc;i++)
                {
                    if(strncmp(argv[i],"path=",5) == 0)
                        {
                            strcpy(path,argv[i]+5);
                        }
                    if(strncmp(argv[i],"recursive", 9) == 0)
                    {
                        isRecursive = true;
                    }
                    if(strncmp(argv[i], "name_starts_with=",17)==0)
                    {
                        isName = true;
                        strcpy(nameStartsWith,argv[i]+17);
                    }
                    if(strncmp(argv[i],"permissions=",12)==0)
                    {
                        isPermission = true;
                        strcpy(permission,argv[i]+12);

                    }
                }
            
             struct stat fileMetadata;    //
              if (stat(path, &fileMetadata) < 0) {  // get info 
                     printf("ERROR\ninvalid directory path");
                     exit(1);
                  }
            printf("SUCCESS\n");
            listDir(path,isRecursive,isName,nameStartsWith,isPermission,permission);


            }
            //FOR PARSE OPTION
            else if(strcmp(argv[1],"parse")==0)
            {
                char path[MAX_MEMORY];
                strcpy(path,argv[2]+5);
                parse(path);
            }
        }
    }
    return 0;
}