#include <iostream>
#include <string>
#include <pthread.h>
//#include <unistd.h>
#include <time.h>

volatile bool exitRequired = false;
int counter = 0;
int counter2 = 0;
const int MAX = 100;
pthread_mutex_t counterLock;
pthread_mutex_t counterLock2;
const int threadCount = 10;


void *threadFunction(void *params) {
    const char* message = (const char *)params;
    timespec delay {0, 10};
    timespec remain;
    while(!exitRequired) {
        pthread_mutex_lock(&counterLock);
        if (counter < MAX) {
            nanosleep(&delay, &remain);
            //std::cout << ' ' << message << "counter :" << counter << std::endl;
            ++counter;
        }
        pthread_mutex_lock(&counterLock2);
        std::cout << ' ' << message << "counter2 :" << counter2 << std::endl;
        pthread_mutex_unlock(&counterLock2);
        pthread_mutex_unlock(&counterLock);
        // std::cout << ' ' << message << std::endl;
        // nanosleep(&delay, &remain);
    }
    return 0;
}

void *threadFunction2(void *params) {
    const char* message = (const char *)params;
    timespec delay {0, 10};
    timespec remain;
    while(!exitRequired) {
        pthread_mutex_lock(&counterLock2);
        if (counter2 < MAX) {
            nanosleep(&delay, &remain);
            //std::cout << ' ' << message << "counter :" << counter << std::endl;
            counter2 += counter;
        }
        pthread_mutex_lock(&counterLock);
        std::cout << ' ' << message << "counter :" << counter << std::endl;
        pthread_mutex_unlock(&counterLock);
        pthread_mutex_unlock(&counterLock2);
        // std::cout << ' ' << message << std::endl;
        // nanosleep(&delay, &remain);
    }
    return 0;
}

int main() {
    timespec delay {1, 10000000};
    timespec remain;


    pthread_mutex_init(&counterLock, 0);
    pthread_mutex_init(&counterLock2, 0);

    pthread_t thOne, thTwo;
    pthread_create(&thOne, 0, threadFunction, (void *)"****");
    pthread_create(&thTwo, 0, threadFunction2, (void *)"----");

    // pthread_t th[threadCount];
    // for (int i = 0; i < threadCount; ++i) {
    //     pthread_create(th + i, 0, threadFunction, (void *)std::to_string(i).c_str());
    // }
    
    std::cout << "Thread created!" << std::endl;

    nanosleep(&delay, &remain);
    exitRequired = true;

    pthread_join(thOne, 0);
    pthread_join(thTwo, 0);

    // for (int i = 0; i < threadCount; ++i) { 
    //     pthread_join(th[i], 0);
    // }
    std::cout << "Thread finished!" << "    Counter: " << counter <<std::endl << "    Counter2: " << counter2 <<std::endl;
    return 0;
}