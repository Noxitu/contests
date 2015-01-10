#ifndef KOLLIB_H
#define KOLLIB_H

#ifdef __cplusplus
extern "C" {
#endif

int NumberOfStudents();
int FirstNeighbor(int i);
int SecondNeighbor(int i);

int NumberOfQueries();
int QueryFrom(int i);
int QueryTo(int i);

#ifdef __cplusplus
}
#endif

#endif
