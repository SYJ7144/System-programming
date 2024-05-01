#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024
#define PROMPT_SIZE 256

int main(int argc, char *argv[]) {
   

   
   if (argc != 5) {
        fprintf(stderr,"Usage: %s -d destination_directory -s substring\n", argv[0]);
        exit(1);
    }
    if(strcmp(argv[1],"-d")!=0){
        fprintf(stderr,"%s: invalid option -- \'%s\'\n",argv[0],argv[1]);
        fprintf(stderr,"Usage: %s -d destination_directory -s substring\n", argv[0]);
        exit(1);
    }
    if(strcmp(argv[3],"-s")!=0){
        fprintf(stderr,"%s: invalid option -- \'%s\'\n",argv[0],argv[3]);
        fprintf(stderr,"Usage: %s -d destination_directory -s substring\n", argv[0]);
        exit(1);
    }

    if (chdir(".") != 0) {
		perror(".");
		exit(1);
	}


    char *dest_directory = argv[2];
    char *search_string = argv[4];

    if (chdir(dest_directory) != 0) {
		fprintf(stderr, "Destination directory does not exist\n");
		exit(1);
	}
    chdir("..");

    

    // Open the destination directory
    DIR *dir = opendir(".");
    DIR *dir2;
    // Iterate over directory entries
    struct dirent *entry;
    struct dirent *direntp;

    int count=0;
    
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a directory and contains the search string
        
	if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
		continue;
	char filename_org[MAX_PATH_LENGTH];
        char file_name[MAX_PATH_LENGTH];
        char *extension;
        strcpy(filename_org,entry->d_name);
    
        
	if((strchr(entry->d_name,'.'))!=NULL){
            strcpy(file_name,strtok(filename_org,"."));
            extension=strtok(NULL,".");
        }
        else{
            strcpy(file_name,entry->d_name);
        }

	if (strstr(file_name, search_string) != NULL) {
            // Build source and destination paths
            char src_path[MAX_PATH_LENGTH];
            char dest_path[MAX_PATH_LENGTH];
            snprintf(src_path, sizeof(src_path), "./%s/%s", dest_directory, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "./%s", argv[2]);


 	    dir2=opendir(dest_path);

            int check=0;
            while((direntp=readdir(dir2))!=NULL){
                if (strcmp(direntp->d_name,entry->d_name)==0) {
                char prompt[PROMPT_SIZE];
                fprintf(stderr,"File %s exists in destination directory. Overwrite? (y/n): ", entry->d_name);
                fgets(prompt, PROMPT_SIZE, stdin);
                if (prompt[0] != 'y' && prompt[0] != 'Y') {
                   
                    check=1;
                }
 
                break;
            }
            }
            closedir(dir2);
            

            if(check==0)
                if (rename(entry->d_name, src_path) != 0) {
                    perror("Error moving directory");
                    return 1;
                }
	    	else{
			count++;
		}
            
        }
    }
    if(count==0)
        fprintf(stderr,"No files containing substring system in the name (excluding extension) found.\n");
    else{
        fprintf(stderr,"%d files moved.\n",count);
    }
    closedir(dir);
    return 0;
}
