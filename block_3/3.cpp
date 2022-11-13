#include <iostream>

using std::cin;
using std::cout;

class MemorySegment {
public:
    MemorySegment* prev;
    MemorySegment* next;
    bool free;
    int start, end, id;

    MemorySegment(MemorySegment* prev, MemorySegment* next, bool free, int start, int end, int id) {
        this->prev = prev;
        this->next = next;
        this->free = free;
        this->start = start;
        this->end = end;
        this->id = id;
        if (prev != nullptr) {
            prev->next = this;
        }
        if (next != nullptr) {
            next->prev = this;
        }
    }
    void remove() const {
        if (prev != nullptr) {
            prev->next = next;
        }
        if (next != nullptr) {
            next->prev = prev;
        }
    }
};

const int maxRequestsNumber = 100000;
MemorySegment* heap[maxRequestsNumber];
MemorySegment* memorySections[maxRequestsNumber];
int memorySize, requestsCount, memorySectionsCount; 
int freeSectionsCount, operationStatus[maxRequestsNumber];

void swap(int var_a, int var_b) {
    MemorySegment* t = heap[var_a];
    heap[var_a] = heap[var_b];
    heap[var_b] = t;
    heap[var_a]->id = var_a;
    heap[var_b]->id = var_b;
}

bool greaterThan(int var_a, int var_b) {
    return (heap[var_a]->end - heap[var_a]->start) >
    (heap[var_b]->end - heap[var_b]->start);
}

void heapify(int id) {
    while (true) {
        int q = id;
        if (((id << 1) + 1 < freeSectionsCount) && greaterThan((id << 1) + 1, q)) {
            q = (id << 1) + 1;
        }
        if (((id << 1) + 2 < freeSectionsCount) && greaterThan((id << 1) + 2, q)) {
            q = (id << 1) + 2;
        }
        if (id == q) {
            return;
        }
        swap(id, q);
        id = q;
    }
}

void heapLift(int id) {
    while (id && greaterThan(id, (id - 1) >> 1)) {
        swap(id, (id - 1) >> 1);
        id = (id - 1) >> 1;
    }
}

void heapRemove() {
    freeSectionsCount--;
    if (freeSectionsCount > 0) {
        swap(0, freeSectionsCount);
        heapify(0);
    }
}

void heapRemove(int id) {
    swap(id, freeSectionsCount - 1);
    freeSectionsCount--;
    if (id >= freeSectionsCount) {
        return;
    }
    heapLift(id);
    heapify(id);
}

void heapAdd(MemorySegment* a) {
    a->id = freeSectionsCount;
    heap[freeSectionsCount] = a;
    heapLift(freeSectionsCount);
    freeSectionsCount++;
}

void allocateMemory(int size) {
    MemorySegment* c = heap[0];
    if (freeSectionsCount == 0 || ((c->end - c->start) < size)) {
        operationStatus[memorySectionsCount++] = 0;
        cout << "-1\n";
        return;
    }
    operationStatus[memorySectionsCount++] = 1;
    memorySections[memorySectionsCount - 1] = new MemorySegment(c->prev, c, false, c->start, c->start + size, -1);
    cout << 1 + c->start << "\n";
    c->start += size;
    if (c->start < c->end) {
        heapify(c->id);
    } else {
        c->remove();
        heapRemove();
        delete c;
    }
}

void freeMemory(int id) {
    id--;
    operationStatus[memorySectionsCount++] = 2;
    if (operationStatus[id] == 0) {
        return;
    }
    operationStatus[id] = 2;
    MemorySegment* c = memorySections[id];
    MemorySegment* sp = c->prev;
    MemorySegment* sn = c->next;
    bool bp = (sp != nullptr && sp->free);
    bool bn = (sn != nullptr && sn->free);
    if (!bp && !bn) {
        c->free = true;
        heapAdd(c);
        return;
    }
    if (!bp) {
        sn->start = c->start;
        heapLift(sn->id);
        c->remove();
        delete c;
        return;
    }
    if (!bn) {
        sp->end = c->end;
        heapLift(sp->id);
        c->remove();
        delete c;
        return;
    }
    sp->end = sn->end;
    heapLift(sp->id);
    c->remove();
    delete c;
    heapRemove(sn->id);
    sn->remove();
    delete sn;
}

int main() {
    cin >> memorySize >> requestsCount;
    freeSectionsCount = 1;
    heap[0] = new MemorySegment(nullptr, nullptr, true, 0, memorySize, 0);
    int t;
    for (int i = 0; i < requestsCount; i++) {
        cin >> t;
        if (t > 0)
            allocateMemory(t);
        else
            freeMemory(-t);
    }
    return 0;
}
