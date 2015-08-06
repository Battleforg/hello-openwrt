#include "delete.h"

void dfs_remove_dir()
{
    DIR *cur_dir = opendir(".");
    struct dirent *ent = NULL;
    struct stat st;

    if (!cur_dir)
    {
        perror("opendir:");
        return;
    }

    while ((ent = readdir(cur_dir)) != NULL)
    {
        stat(ent->d_name, &st);
    
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            chdir(ent->d_name);
            dfs_remove_dir();
            chdir("..");
        }

        remove(ent->d_name);
    }
    
    closedir(cur_dir);
}

void remove_dir(const char *path_raw)
{
    char old_path[100];

    if (!path_raw)
    {
        return;
    }

    getcwd(old_path, 100);
    
    if (chdir(path_raw) == -1)
    {
        fprintf(stderr, "not a dir or access error\n");
        return;
    }

    printf("path_raw : %s\n", path_raw);
    dfs_remove_dir();
    chdir(old_path);

    
    // 如果你想删除该目录本身的话,取消注释
    // unlink(old_path); 
     
}

// int main(int argc, char* argv[])
// {
//     // if (argc > 1)
//     // {
//         remove_dir("/home/lyk/temptttt/data");
//     // }

//     return 0;
// }
