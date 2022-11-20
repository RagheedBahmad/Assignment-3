#include <iostream>
#include <pthread.h>
#include <random>
#include <chrono>

using namespace std;
const int LENGTH = 10;
int PART = 0;
const int THREADS = 2;
int COUNT[THREADS] = { 0 };
int ARRAY[LENGTH];

void fillArray(int array[]) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, 6);
    for (int i = 0; i < LENGTH; i++) {
        array[i] = distr(generator);
    }
}

void* count1s(void* arg) {
    PART++;
    for (int i = PART * (LENGTH / 4); i < (PART + 1) * (LENGTH/ 4); i++)
        if (ARRAY[i] == 1) {
            COUNT[PART] += 1;
        }
}

int main() {
    fillArray(ARRAY);
    for (int i = 0; i < LENGTH; i++) {
        std::cout << ARRAY[i] << " ";
    }
    pthread_t threads[THREADS];

    auto start = chrono::steady_clock::now();

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, count1s, (void*)NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    auto end = chrono::steady_clock::now();

    int total_count = 0;
    for (int i = 0; i < THREADS; i++)
        total_count += COUNT[i];

    cout << "Count: " << total_count << endl;

    auto diff = end - start;

    cout << "It took " << chrono::duration <double, milli> (diff).count() << " seconds to run this program with " << THREADS << " threads";
}
