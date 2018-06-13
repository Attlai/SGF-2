

void commande_cd(char **args);
void printDir();
char *lire_ligne(void);
char **split_ligne(char *line);
int execution_cmd(char **args,DISK * partition,int* current_id);
int display_syntax(char * command);
