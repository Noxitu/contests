#include "kollib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static int n;
static int* nodes;
static int* nodes_inv;
static int queries;
static int* query_from;
static int* query_to;

static void Init() {
  static int initialized = 0;
  int i;
  if (initialized)
    return;
  assert(2 == scanf("%d%d", &n, &queries));
  nodes = (int*)malloc(n* sizeof(int));
  nodes_inv = (int*)malloc((n+ 1)* sizeof(int));
  assert(nodes != NULL && nodes_inv != NULL);
  for (i = 0; i < n; ++i) {
    assert(1 == scanf("%d", nodes + i));
    nodes_inv[nodes[i]] = i;
  }
  query_from = (int*)malloc((queries + 1) * sizeof(int));
  query_to = (int*)malloc((queries + 1) * sizeof(int));
  assert(query_from != NULL && query_to != NULL);
  for (i = 1; i <= queries; ++i)
    assert(2 == scanf("%d %d", query_from + i, query_to + i));
  initialized = 1;
}

int NumberOfStudents() {
  Init();
  return n;
}

int FirstNeighbor(int i) {
  int a, b;
  Init();
  assert(1 <= i && i <= n);
  a = nodes[(nodes_inv[i] + 1) % n];
  b = nodes[(nodes_inv[i] + n - 1) % n];
  return a < b ? a : b;
}

int SecondNeighbor(int i) {
  int a, b;
  Init();
  assert(1 <= i && i <= n);
  a = nodes[(nodes_inv[i] + 1) % n];
  b = nodes[(nodes_inv[i] + n - 1) % n];
  return a < b ? b : a;
}

int NumberOfQueries() {
  Init();
  return queries;
}

int QueryFrom(int i) {
  Init();
  assert(1 <= i && i <= queries);
  return query_from[i];
}

int QueryTo(int i) {
  Init();
  assert(1 <= i && i <= queries);
  return query_to[i];
}
