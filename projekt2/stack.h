#include "stdafx.h"

void saveChanges(struct changesStack &changesStack, struct changesSet &changesSet);
void addToChangesStack(struct changesStack &changesStack, struct changesSet &changesSet);
struct changesSet getFirstElement(struct changesStack &changesStack);
void deleteChangesStack(struct changesStack &changesStack);
void deleteLastElement(struct changesStack &changesStack);
void deleteFirstElement(struct changesStack &changesStack);
changesStack createChangesStack(unsigned int size);
changesStack createChangesStack();
