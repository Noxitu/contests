#ifndef KRALIB_H_
#define KRALIB_H_

#ifdef __cplusplus
extern "C" {
#endif

// Zwraca n (1 <= n <= 10^9) - wysokość rurki Janka (liczbę walców wchodzących
// w jej skład).
int PipeHeight();

// Zwraca m (1 <= m <= 10^9) - liczbę krążków, które Janek zamierza wrzucić
// do rurki.
int NumberOfDiscs();

// Dla danego i (1 <= i <= n) zwraca r_i (1 <= r_i <= 10^18) - średnicę otworu
// wyciętego w i-tym (od góry) walcu tworzącym rurkę.
long long int HoleDiameter(long long int i);

// Dla danego j (1 <= j <= m) zwraca k_j (1 <= k_j <= 10^18) - średnicę krążka,
// który zostanie wrzucony jako j-ty.
long long int DiscDiameter(long long int j);

#ifdef __cplusplus
}
#endif

#endif  // KRALIB_H_
