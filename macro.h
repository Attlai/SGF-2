#define ROOT_INODE_ID 1
#define ROOT_PARENT_ID 0


#define NEW(type) malloc(sizeof(type));


#ifdef DEBUG_ON
#define DEBUG(msg) printf(msg);
#define DEBUG1(msg,i) printf(msg,i);
#else
#define DEBUG(msg)
#define DEBUG1(msg,i)
#endif //


#define TEST(test_fonc)  printf(test_fonc()? " + %s : OK\n" : " !! %s : KO\n",  #test_fonc );
#define INIT_TEST    bool test_ok = true;
#define END_TEST     return test_ok;

#define TESTU1(test,message,code)\
              if(test)\
                {\
                printf(message, code); \
                test_ok = false;\
                }

#define TESTU0(test,message)\
              if(test)\
                {\
                printf(message); \
                test_ok = false;\
                }

