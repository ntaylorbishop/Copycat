#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int(SortingComparator)(const void*, const void*);

int intcmp(const void *a, const void *b) {
	return *(const int *)a - *(const int *)b;
}

void myswap(void *e1, void *e2, int size) {
	void* sp = (void*)malloc(size);
	memcpy(sp, e1, size);
	memcpy(e1, e2, size);
	memcpy(e2, sp, size);
	free(sp);
}

void QuickSort(void* base, size_t n, size_t size, SortingComparator* sortCallback) {
	int i;
	int rv;
	void* mp;
	void* sp;  //for swap

	if (n <= 1) { 
		return; 
	}

	mp = base;
	for (i = 1; i < n; ++i) {
		rv = sortCallback(base, base + size * i);
		if (rv > 0) {
			mp += size;
			if (mp != base + size*i) {
				//swap element at base+size*i with element at mp
				//printf("swap %d and %dn", *(int*)(base+size*i), *(int*)mp);
				myswap(base + size*i, mp, size);
			}
		}
	}
	//swap the base element with the element at mp
	myswap(base, mp, size);
	QuickSort(base, (mp - base) / size, size, sortCallback);
	QuickSort(mp + size, n - 1 - (mp - base) / size, size, sortCallback);
}