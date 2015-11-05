
#ifdef __cplusplus
extern "C"{
#endif

    typedef struct namemap{
        char name[256];
        unsigned int address;
    }namemap_t;

    struct namemap* ListInstances(int *len);
    void FreeMap(struct namemap*);
    unsigned int FindAddress(const char *name);

#ifdef __cplusplus
}
#endif
