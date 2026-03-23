// PagedArray.h
#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H

#include <string>
#include <cstddef>
#include <fstream>

using string = std::string;

class PagedArray {
public:
    PagedArray(const string& filePath, size_t PageBSize);
    ~PagedArray();

    int& operator[](size_t index);

    size_t getPageHits() const;
    size_t getPageFaults() const;
    size_t getSize() const;

private:
    void loadPage(size_t PageNum);
    void savePage(size_t PageNum);
    size_t replacePage();
    size_t getPageNum(size_t index) const;
    size_t getPosPage(size_t index) const;

    std::fstream file;
    string filePath;
    size_t pageBSize;
    size_t intAmount;
    size_t intperPage;
    size_t LRUcounter;

    static const size_t MAX_PAGES = 4;
    int* pages[MAX_PAGES];
    size_t loadedPages[MAX_PAGES];
    size_t lastUsed[MAX_PAGES];

    size_t pageHits;
    size_t pageFaults;
};

#endif 