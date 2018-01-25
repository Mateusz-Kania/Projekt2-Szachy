
struct changesSet {
	bool castle;
	bool passant;
	unsigned char change1[3];
	unsigned char change2[3];
	unsigned char change3[3];
	unsigned char change4[3];
	unsigned char kingsPrevPosition[2];
	bool whiteCastlesPrevChances[2];
	bool blackCastlesPrevChances[2];
};

struct changesStackElement {
	struct changesSet changesSet;
	struct changesStackElement *nextElement;
};
struct changesStack {
	unsigned int size;
	bool maxSizeSet;
	unsigned int maxSize;
	struct changesStackElement *nextElement;
};


	void saveChanges(struct changesStack &changesStack, struct changesSet &changesSet);
	void addToChangesStack(struct changesStack &changesStack, struct changesSet &changesSet);
	struct changesSet getFirstElement(struct changesStack &changesStack);
	void deleteChangesStack(struct changesStack &changesStack);
	void deleteLastElement(struct changesStack &changesStack);
	void deleteFirstElement(struct changesStack &changesStack);
	changesStack createChangesStack(unsigned int size);
	changesStack createChangesStack();
