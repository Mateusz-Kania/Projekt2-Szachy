#include "stdafx.h"
#include "changesStack.h"

void saveChanges(struct changesStack &changesStack, struct changesSet &changesSet) {
	addToChangesStack(changesStack, changesSet);
	if (changesStack.maxSizeSet) {
		if (changesStack.size > changesStack.maxSize) {
			deleteLastElement(changesStack);
		}
	}
}
void addToChangesStack(struct changesStack &changesStack, struct changesSet &changesSet) {
	changesStackElement *newElement = new changesStackElement;
	changesStack.size++;
	newElement->changesSet = changesSet;
	newElement->nextElement = changesStack.nextElement;
	changesStack.nextElement = newElement;
}
struct changesSet getFirstElement(struct changesStack &changesStack) {
	changesSet returnedChangeSet = changesStack.nextElement->changesSet;
	//tutaj moze dodawac zmiany w 2 stosie
	deleteFirstElement(changesStack);

	return returnedChangeSet;
}
void deleteChangesStack(struct changesStack &changesStack) {
	if (changesStack.size != 0) {
		changesStackElement** indicatorsArray = new changesStackElement*[changesStack.size];
		*indicatorsArray = changesStack.nextElement;
		for (unsigned int i = 1; i < changesStack.size; ++i) {
			*(indicatorsArray + i) = (*(indicatorsArray + i - 1))->nextElement;//idk
		}
		for (unsigned int i = 0; i < changesStack.size; ++i) {
			delete *(indicatorsArray + i);
		}
		delete[] indicatorsArray;
	}
}
void deleteLastElement(struct changesStack &changesStack) {//nie uwzglednia bledu usuniecia ostatniego elementu przy 0 elementach
	changesStackElement *currentElement = changesStack.nextElement;
	for (unsigned int i = 0; i < changesStack.size - 1; ++i) {
		currentElement = currentElement->nextElement;
	}

	changesStack.size--;
	//tutaj moga byc usuniete wskazniki changesSet
	delete currentElement;
	currentElement = nullptr;
}
void deleteFirstElement(struct changesStack &changesStack) {
	changesStackElement* secondElement = changesStack.nextElement->nextElement;
	delete changesStack.nextElement;
	changesStack.size--;
	changesStack.nextElement = secondElement;
}
changesStack createChangesStack(unsigned int maxSize) {
	changesStack newChangesStack;
	newChangesStack.maxSizeSet = true;
	newChangesStack.maxSize = maxSize;
	newChangesStack.size = 0;
	return newChangesStack;
}
changesStack createChangesStack() {
	changesStack newChangesStack;
	newChangesStack.maxSizeSet = false;
	newChangesStack.size = 0;
	return newChangesStack;
}

