#define ERRNO_NO_FREE_INODE  -10

void Initialiser_BLOC(BLOC block);
//void Initialiser_META(META *metadata);
//void Initialiser_INODE(INODE *node);
void Initialiser_DISK(DISK *partition);

int Creer_INODE(DISK* partition,char *name,type_inode type);
int Trouver_place_DISK(DISK *partition);
