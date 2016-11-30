#include <assert.h>
#include <stdio.h>

static const int __KH_N[] = {22092, 495603428, 423796462, 423604809, 462290170,
                             479334009, 432383276, 422670586, 446300736, 457140260};
static const int __KH_A[] = {86269, 95304, 58980, 19284, 76760, 83975, 23576, 96606,
                             43598, 48600};
static const int __KH_B[] = {78739, 15089, 43295, 45645, 81567, 53653, 68584, 21369,
                             22468, 62028};
static const int __KH_C[] = {32956, 17638, 64352, 23947, 94820, 33545, 16519, 73433,
                             87990, 28548};
static const int __KH_SIGN[] = {1, 0, -1, 0, 1, 0, -1, 0, 1, 0};
static const int __KH_BIAS[] = {0, 0, 0, 0, 0, -654321, 0, 0, 0, 0};
static int __KH_n, __KH_a, __KH_b, __KH_c, __KH_sign, __KH_bias;
static int __KH_initialized = 0;

static void __KH_Init() {
  int TID;
  scanf("%d", &TID);
  assert(TID >= 1 && TID <= 10);
  --TID;
  __KH_n = __KH_N[TID];
  __KH_a = __KH_A[TID];
  __KH_b = __KH_B[TID];
  __KH_c = __KH_C[TID];
  __KH_sign = __KH_SIGN[TID];
  __KH_bias = __KH_BIAS[TID];
  __KH_initialized = 1;
}

#ifdef __cplusplus
extern "C" {
#endif

long long GetN() {
  if (!__KH_initialized) {
    __KH_Init();
  }
  return __KH_n;
}

long long GetTaste(long long j) {
  if (!__KH_initialized) {
    __KH_Init();
  }
  assert(j >= 0 && j < __KH_n);
  int i = (int)(j + 1);
  unsigned long long temp;
  int ret;
  i += __KH_a;
  temp = __KH_b;
  temp = temp * i * i * i * i * i * i;
  temp = temp + __KH_c;
  temp = temp * i;
  ret = temp % (2 * __KH_n - 1);
  ret -= (__KH_n - 1);
  if (__KH_sign != 0) {
    if (ret < 0)
      ret = __KH_sign * (-ret);
    else if (ret == 0)
      ret = __KH_sign;
    else
      ret = __KH_sign * ret;
  } else {
    ret += __KH_bias;
  }
  return (long long)(-ret);
}

#ifdef __cplusplus
}
#endif

int test_initializer = [](){ __KH_Init(); return 0; }();