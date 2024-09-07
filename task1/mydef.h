#ifdef LOG
    #define PRINT_LOG(...) printf("[LOG] " __VA_ARGS__)
#else
    #define PRINT_LOG(...)
#endif


#ifdef DEBUG
    #define PRINT_DEBUG(...) { fprintf(stderr, "[DEBUG] " __VA_ARGS__); }
#else
    #define PRINT_DEBUG(...) {}
#endif


#define PRINT_ERROR(...)                                                                     \
{                                                                                            \
    fprintf(stderr, "[ERROR] " __VA_ARGS__);                                                 \
    fprintf(stderr, "FILE: %s; FUNC: %s; LINE: %d; \n", __FILE__, __func__, __LINE__);       \
}
