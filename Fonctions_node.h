

void Initialiser_BLOC(BLOC block);
//void Initialiser_META(META *metadata);
//void Initialiser_INODE(INODE *node);
void Initialiser_DISK(DISK *partition);

int Creer_INODE(DISK* partition,char *name,type_inode type);
int Trouver_place_DISK(DISK *partition);
int Remove_INODE(DISK* partition,int rm_id);
int create_folder(DISK *partition,char* nom,int id_parent);
int remove_folder(DISK* partition,int id_dossier,mode_suppression mode);
int seek_folder_slot_from_inode(INODE* dossier,int id_fichier);
int seek_free_slot_folder(INODE* dossier_parent);
int create_file(DISK *partition,char *nom,int id_parent);
bool check_inode_name_is_unique(INODE *parent,char *name);
int remove_file(DISK* partition,int id_fichier);
int list_content_folder(DISK *partition,int id_dossier_pere);
DISK Initialize_System(int* current_id);
int change_current_directory(DISK* partition,int *current_id,char* nom_destination);
int seek_id(DISK* partition,int current_id,char* nom);
