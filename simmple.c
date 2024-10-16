#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 3  // Matris boyutu (NxN)

int A[N][N], B[N][N], C[N][N];
pthread_mutex_t mutex; // Mutex kilidi

// İş parçacığı fonksiyonu (matris çarpım işlemi)
void* multiply(void* arg) {
    int *data = (int*)arg;
    int sum = 0;
    
    // Sırasıyla satır ve sütunları çarp
    for (int i = 0; i < N; i++) {
        sum += data[i] * data[i + N];
    }
    
    // Mutex kilidi al
    pthread_mutex_lock(&mutex);
    C[data[2 * N]][data[2 * N + 1]] = sum;
    pthread_mutex_unlock(&mutex); // Mutex kilidini bırak
    
    free(arg); // Dinamik olarak ayrılan belleği serbest bırak
    pthread_exit(0);
}

int main() {
    // Matris A ve B'yi doldur
    printf("Matris A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10; // Rastgele değer atama
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    
    printf("Matris B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 10; // Rastgele değer atama
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    
    pthread_t threads[N * N]; // İş parçacıkları
    pthread_mutex_init(&mutex, NULL); // Mutex başlat
    
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Bellek ayır
            int* data = (int*)malloc((2 * N + 2) * sizeof(int));
            
            for (int k = 0; k < N; k++) {
                data[k] = A[i][k];    // Matris A'dan satır al
                data[k + N] = B[k][j]; // Matris B'den sütun al
            }
            data[2 * N] = i; // Satır numarası
            data[2 * N + 1] = j; // Sütun numarası
            
            // İş parçacığını oluştur ve başlat
            pthread_create(&threads[count++], NULL, multiply, (void*)data);
        }
    }
    
    // Tüm iş parçacıklarının bitmesini bekledim
    for (int i = 0; i < N * N; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Sonuç matrisini yazdır
    printf("Sonuç Matrisi C:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    pthread_mutex_destroy(&mutex); // Mutex'i yok et
    return 0;
}
