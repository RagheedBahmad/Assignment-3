#include <iostream>
#include <pthread.h>
#include <random>
#include <chrono>
using namespace std;
const int LENGTH = 100000;
int PART = 1;
const int THREADS = 1;
int COUNT[THREADS] = { 0 };
int ARRAY[LENGTH];

void fillArray(int array[]) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, 5);
    for (int i = 0; i < LENGTH; i++) {
        array[i] = distr(generator);
    }
}

void* count1s(void* arg) {
    for (int i = PART * (LENGTH / THREADS); i < (PART + 1) * (LENGTH/ THREADS); i++) {
        if (ARRAY[i] == 1) {
            COUNT[PART] += 1;
        }
    }
    PART++;
}

int main() {
    fillArray(ARRAY);

    pthread_t threads[THREADS];

    auto start = chrono::steady_clock::now();

    for (unsigned long long &thread: threads) {
        pthread_create(&thread, nullptr, count1s, (void *) nullptr);
//        sleep(1);
    }
    for (unsigned long long thread: threads)
        pthread_join(thread, nullptr);

    auto end = chrono::steady_clock::now();

    int total_count = 0;
    for (int i: COUNT) {
        total_count += i;
    }
    auto diff = end - start;
    cout << chrono::duration<double, milli>(diff).count() << " ";
}
