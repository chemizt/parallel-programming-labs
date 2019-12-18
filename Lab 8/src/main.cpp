#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <math.h>
#include <omp.h>
#include <vector>
#include <stack>
#include <string>

#include "auxUtils.hpp"
#include "inUtils.hpp"
#include "mathUtils.hpp"
#include "outUtils.hpp"
#include "randUtils.hpp"
#include "typeDefs.hpp"

#ifndef _WIN32
#include <unistd.h>
#define _sleep(x) usleep(x * 1000)
#endif
#ifdef _WIN32
#include <Windows.h>
#define _sleep(x) Sleep(x)
#endif

using namespace std::chrono;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::locale;
using std::ofstream;
using std::vector;
using std::stack;
using std::string;

#define SLEEP_MAX_RAND 5000
#define TEST_ITERATIONS 5

random_device rD;
mt19937_64 rNG(rD());
uniform_int_distribution<> msgDistribution(0, RAND_MAX_VALUE);
uniform_int_distribution<> slpDistribution(0, SLEEP_MAX_RAND);
omp_lock_t lock;

class Actor
{
    protected:
        bool isWaiting;
        time_point<steady_clock, nanoseconds> waitStartTime;
        uInt actionPeriod;
        uInt id;
    public:
        Actor();
        bool getWaitingStatus();
        time_point<steady_clock, nanoseconds> getWaitStartTime();
        uInt getActionPeriod();
        uInt getId();
        void startWaiting();
        void stopWaiting();
};

Actor::Actor()
{
    this->waitStartTime = steady_clock::now();
    this->isWaiting = false;
}

uInt Actor::getId()
{
    return this->id;
}

uInt Actor::getActionPeriod()
{
    return this->actionPeriod;
}

void Actor::startWaiting()
{
    this->waitStartTime = steady_clock::now();
    this->isWaiting = true;
}

void Actor::stopWaiting()
{
    this->isWaiting = false;
}

bool Actor::getWaitingStatus()
{
    return this->isWaiting;
}

time_point<steady_clock, nanoseconds> Actor::getWaitStartTime()
{
    return this->waitStartTime;
}

class Storage
{
    private:
        uInt maxCapacity;
        stack<int>* buffer;
    public:
        Storage() { };
        Storage(uInt mxCap);
        stack<int>* getBuffer();
        uInt getMaxCapacity();
};

Storage* PublicStorage;

Storage::Storage(uInt mxCap)
{
    this->maxCapacity = mxCap;
    this->buffer = new stack<int>;
}

stack<int>* Storage::getBuffer()
{
    return this->buffer;
}

uInt Storage::getMaxCapacity()
{
    return this->maxCapacity;
}

class Producer : public Actor
{
    public:
        using Actor::Actor;
        Producer(uInt newId);
        uInt produceMessage();

};

Producer::Producer(uInt newId)
{
    #pragma omp critical
    this->actionPeriod = slpDistribution(rNG) + 2500;
    this->id = newId;
}

uInt Producer::produceMessage()
{
    uInt msg;
    #pragma omp critical
    {
        msg = msgDistribution(rNG);
        rNG.seed(rNG());
    }
    return msg;
}

class Consumer : public Actor
{
public:
    using Actor::Actor;
    Consumer(uInt newId);
};

Consumer::Consumer(uInt newId)
{
    #pragma omp critical
    this->actionPeriod = slpDistribution(rNG) + 5000;
    this->id = newId;
}

void producerProcedure(Producer* prod, bool* msgStored, bool* infOutput, const uInt msg)
{
    if (!(*infOutput)) logOutput("Производитель " + std::to_string(prod->getId()) + " создал сообщение " + std::to_string(msg) + " и пытается поместить его в хранилище");
    if (PublicStorage->getBuffer()->size() < PublicStorage->getMaxCapacity())
    {
        PublicStorage->getBuffer()->push(msg);
        logOutput("Производитель " + std::to_string(prod->getId()) + " поместил сообщение " + std::to_string(msg) + " в хранилище");
        if (prod->getWaitingStatus())
        {
            logOutput("Производитель " + std::to_string(prod->getId()) + " находился в ожидании освобождения места в хранилище " + std::to_string(duration_cast<milliseconds>(steady_clock::now() - prod->getWaitStartTime()).count()) + " мс");
            prod->stopWaiting();
        }
        logOutput("Хранилище содержит " + std::to_string(PublicStorage->getBuffer()->size()) + " сообщений");
        *msgStored = true;
    }
    else
    {
        if (!(*infOutput))
        {
            logOutput("Хранилище заполнено. Производитель " + std::to_string(prod->getId()) + " ожидает появления свободного места");
            prod->startWaiting();
            *infOutput = true;
        }
    }
}

void consumerProcedure(Consumer* cons, bool* msgRetr, bool* infOutput)
{
    if (!(*infOutput)) logOutput("Потребитель " + std::to_string(cons->getId()) + " запросил сообщение из хранилища");
    if (PublicStorage->getBuffer()->size() > 0)
    {
        uInt message = PublicStorage->getBuffer()->top();
        PublicStorage->getBuffer()->pop();
        logOutput("Потребитель " + std::to_string(cons->getId()) + " извлёк сообщение " + std::to_string(message) + " из хранилища");
        if (cons->getWaitingStatus())
        {
            logOutput("Потребитель " + std::to_string(cons->getId()) + " находился в ожидании появления сообщений в хранилище " + std::to_string(duration_cast<milliseconds>(steady_clock::now() - cons->getWaitStartTime()).count()) + " мс");
            cons->stopWaiting();
        }
        logOutput("Хранилище содержит " + std::to_string(PublicStorage->getBuffer()->size()) + " сообщений");
        *msgRetr = true;
    }
    else
    {
        if (!(*infOutput))
        {
            logOutput("Хранилище пусто. Потребитель " + std::to_string(cons->getId()) + " ожидает появления сообщений в хранилище");
            cons->startWaiting();
            *infOutput = true;
        }
    }
}

void producerCritWrapper(uInt pQ, const int runs)
{
    #pragma omp parallel num_threads(pQ)
    {
        Producer* localProducer = new Producer(omp_get_thread_num());
        int i = 0;
        while (i < runs || runs == -1)
        {
            bool* msgHasBeenStored = new bool;
            bool* infoHasBeenOutput = new bool;
            *msgHasBeenStored = false;
            *infoHasBeenOutput = false;
            uInt message = localProducer->produceMessage();
            
            _sleep(localProducer->getActionPeriod());
            while (!(*msgHasBeenStored))
            {
                #pragma omp critical
                {
                    auto start = steady_clock::now();
                    producerProcedure(localProducer, msgHasBeenStored, infoHasBeenOutput, message);
                    if (!(*infoHasBeenOutput)) logOutput("Производитель " + std::to_string(localProducer->getId()) + " находился в критической секции " + std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()) + " мкс");
                }
                if (!(*msgHasBeenStored)) _sleep(500);
            }
            i++;
        }
    }
}

void consumerCritWrapper(uInt cQ, const int runs)
{
    #pragma omp parallel num_threads(cQ)
    {
        Consumer* localConsumer = new Consumer(omp_get_thread_num());
        int i = 0;
        while (i < runs || runs == -1)
        {
            bool* msgHasBeenRetrieved = new bool;
            bool* infoHasBeenOutput = new bool;
            *msgHasBeenRetrieved = false;
            *infoHasBeenOutput = false;

            _sleep(localConsumer->getActionPeriod());
            while (!(*msgHasBeenRetrieved))
            {
                #pragma omp critical
                {
                    auto start = steady_clock::now();
                    consumerProcedure(localConsumer, msgHasBeenRetrieved, infoHasBeenOutput);
                    if (!(*infoHasBeenOutput)) logOutput("Потребитель " + std::to_string(localConsumer->getId()) + " находился в критической секции " + std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()) + " мкс");
                }
                if (!(*msgHasBeenRetrieved)) _sleep(500);
            }
            i++;
        }
    }
}

void producerLockWrapper(uInt pQ, const int runs)
{
    #pragma omp parallel num_threads(pQ)
    {
        Producer* localProducer = new Producer(omp_get_thread_num());
        int i = 0;
        while (i < runs || runs == -1)
        {
            bool* msgHasBeenStored = new bool;
            bool* infoHasBeenOutput = new bool;
            *msgHasBeenStored = false;
            *infoHasBeenOutput = false;
            uInt message = localProducer->produceMessage();

            _sleep(localProducer->getActionPeriod());
            while (!(*msgHasBeenStored))
            {
                omp_set_lock(&lock);
                auto start = steady_clock::now();
                producerProcedure(localProducer, msgHasBeenStored, infoHasBeenOutput, message);
                if (!(*infoHasBeenOutput)) logOutput("Производитель " + std::to_string(localProducer->getId()) + " удерживал замок " + std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()) + " мкс");
                omp_unset_lock(&lock);
                if (!(*msgHasBeenStored)) _sleep(500);
            }
            i++;
        }
    }
}

void consumerLockWrapper(uInt cQ, const int runs)
{
    #pragma omp parallel num_threads(cQ)
    {
        Consumer* localConsumer = new Consumer(omp_get_thread_num());
        int i = 0;
        while (i < runs || runs == -1)
        {
            bool* msgHasBeenRetrieved = new bool;
            bool* infoHasBeenOutput = new bool;
            *msgHasBeenRetrieved = false;
            *infoHasBeenOutput = false;

            _sleep(localConsumer->getActionPeriod());

            while (!(*msgHasBeenRetrieved))
            {
                omp_set_lock(&lock);
                auto start = steady_clock::now();
                consumerProcedure(localConsumer, msgHasBeenRetrieved, infoHasBeenOutput);
                if (!(*infoHasBeenOutput)) logOutput("Потребитель " + std::to_string(localConsumer->getId()) + " удерживал замок " + std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()) + " мкс");
                omp_unset_lock(&lock);
                if (!(*msgHasBeenRetrieved)) _sleep(500);
            }
            i++;
        }
    }
}

int main()
{
    uInt producerQuantity;
    uInt consumerQuantity;
    uInt storageSize;
    uInt versatileVar;
    ofstream resultsFile;

    resultsFile.open(LOG_FILE_NAME);
    resultsFile.clear();
    resultsFile.close();
    resultsFile.open(LOG_FILE_NAME, std::ios_base::app);
    omp_set_nested(true);
    rNG.seed(rNG());

    cout << "Введите желаемое количество производителей: ";
    cin >> producerQuantity;
    resultsFile << getCurrentTimeStampAsString() << " " << "Количество производителей: " << producerQuantity << endl;
    cout << "Введите желаемое количество потребителей: ";
    cin >> consumerQuantity;
    resultsFile << getCurrentTimeStampAsString() << " " << "Количество потребителей: " << consumerQuantity << endl;
    cout << "Введите желаемый объём хранилища: ";
    cin >> storageSize;
    resultsFile << getCurrentTimeStampAsString() << " " << "Объём хранилища: " << storageSize << endl;
    resultsFile.close();
    PublicStorage = new Storage(storageSize);

    do 
    {
        cout << "Доступные действия:\n"
        << "0. Выход\n"
        << "1. Моделирование (критические секции)\n"
        << "2. Моделирование (замки)\n"
        << "3. Контрольный прогон (критические секции)\n"
        << "4. Контрольный прогон (замки)\n"
        << "Выберите желаемое действие: ";
        cin >> versatileVar;

        switch (versatileVar)
        {
            case 1:
            {
                cout << "Выполняется моделирование задачи с использованием критических секций\n"
                << "Нажмите Ctrl+C для остановки" << endl;
                #pragma omp parallel sections num_threads(2)
                {
                    #pragma omp section
                    {
                        producerCritWrapper(producerQuantity, -1);
                    }
                    #pragma omp section
                    {
                        consumerCritWrapper(consumerQuantity, -1);
                    }
                }
                break;
            }
            case 2:
            {
                cout << "Выполняется моделирование задачи с использованием замков\n"
                << "Нажмите Ctrl+C для остановки" << endl;
                omp_init_lock(&lock);
                #pragma omp parallel sections num_threads(2)
                {
                    #pragma omp section
                    {
                        producerLockWrapper(producerQuantity, -1);
                    }
                    #pragma omp section
                    {
                        consumerLockWrapper(consumerQuantity, -1);
                    }
                }
                break;
            }
            case 3:
            {
                cout << "Выполняется контрольный прогон программы с использованием критических секций\n"
                << "Количество итераций для каждой сущности: " << TEST_ITERATIONS << endl;
                #pragma omp parallel sections num_threads(2)
                {
                    #pragma omp section
                    {
                        producerCritWrapper(producerQuantity, TEST_ITERATIONS);
                        logOutput("Производители закончили работать!");
                    }
                    #pragma omp section
                    {
                        consumerCritWrapper(consumerQuantity, TEST_ITERATIONS);
                        logOutput("Потребители закончили работать!");
                    }
                }
                cout << "Контрольный прогон завершён!\n" << endl;
                break;
            }
            case 4:
            {
                cout << "Выполняется контрольный прогон программы с использованием замков\n"
                << "Количество итераций для каждой сущности: " << TEST_ITERATIONS<< endl;
                omp_init_lock(&lock);
                #pragma omp parallel sections num_threads(2)
                {
                    #pragma omp section
                    {
                        producerLockWrapper(producerQuantity, TEST_ITERATIONS);
                        logOutput("Производители закончили работать!");
                    }
                    #pragma omp section
                    {
                        consumerLockWrapper(consumerQuantity, TEST_ITERATIONS);
                        logOutput("Потребители закончили работать!");
                    }
                }
                cout << "Контрольный прогон завершён!\n" << endl;
                break;
            }
            case 0:
            {
                cout << "Выход..." << endl;
                break;
            }
            default:
            {
                cout << "Значение не определено! Для выхода введите 0" << endl;
                break;
            }
        }
    }
    while (versatileVar != 0);
    
    return 0;
}