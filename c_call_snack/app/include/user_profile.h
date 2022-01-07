#ifndef USER_PROFILE_H
#define USER_PROFILE_H

extern PyObject *g_mod;

int read_file(const char* filename, char* buf, size_t buf_size);

#endif
