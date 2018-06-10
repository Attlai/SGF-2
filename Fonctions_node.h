#define ROOT_INODE_ID 1
#define ROOT_PARENT_ID 0
#define ERRNO_NO_FREE_INODE  -10
#define ERR_ROOT_ALRDY_CREATED  -11
#define ERR_PARENT_NOT_FOLDER -12
#define ERR_ROOT_WRONG_ID -13
#define ERR_DIRECTORY_FULL -14
#define ERR_UNUSUED_INODE -15
#define ERR_DIRECTORY_NOT_EMPTY -16
#define ERR_INODE_ID_NOT_FOUND_IN_FOLDER -17

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
