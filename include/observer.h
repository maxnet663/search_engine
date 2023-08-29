#ifndef IOBSERVER_H
#define IOBSERVER_H

/*
 * we will use Observer pattern to update objects
 * responsible for database management InvertedIndex
 * and extracting up-to-date information from json files ConverterJSON
 */

class IObserver {
    virtual void update() = 0;
};

class IObservable {
public:
    virtual void notify() = 0;
    virtual void addObserver(IObserver *subscriber) = 0;
    virtual void removeObserver(IObserver *subscriber) = 0;
};

#endif //IOBSERVER_H
