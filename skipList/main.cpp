#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "skipList.h"

int main(int argc, char* argv[]) {
	SkipList* list = createList();
	insertNode(list, 1, 1);
	insertNode(list, 6, 1);
	insertNode(list, 8, 1);
	insertNode(list, 3, 1);
	insertNode(list, 5, 1);

	erase(list, 8);
	insertNode(list, 7, 1);
	insertNode(list, 2, 1);
	
	PrintList(list);

	return 0;
}
