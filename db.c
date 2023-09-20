#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NODESIZE 232

struct node {
	char par[64];
	char key[64];
	char val[64];
	struct node *prev;
	struct node *next;
} typedef nodet;

struct node *thead;
struct node *temp[64];

void tolist(struct node *prev, struct node *from, struct node *to) {
	struct node *next = prev->next;
    to->next = next;
    from->prev = prev;
	next->prev = to;
	prev->next = from;
}

void fromlist(struct node *from, struct node *to) { 
    struct node *prev = from->prev;
	struct node *next = to->next;
    prev->next = next;
    next->prev = prev;
	to->next = from;
	from->prev = to;	
}

int listlen(struct node *random) {
    struct node *i;
    int c = 1;
	for(i = random->next; i != random; i = i->next)
	    c++;
	return c;
}

struct node *mknode(void) {
	struct node *nNode = (struct node*) malloc(NODESIZE);
	strcpy(nNode->par, "");
	strcpy(nNode->key, "");
	strcpy(nNode->val, "");
	nNode->prev = nNode;
	nNode->next = nNode;
	return nNode;
}

void printn(struct node *toPrint) {
    if(strcmp(toPrint->par, "") != 0)  
   	    printf("[%s]\n", toPrint->par);
    if(strcmp(toPrint->key, "") != 0)  
        printf("%s=", toPrint->key);
    if(strcmp(toPrint->val, "") != 0)  
        printf("%s\n", toPrint->val);
}

void fprintn(struct node *toPrint, FILE *toRw) {
    if(strcmp(toPrint->par, "") != 0)  
   	    fprintf(toRw, "[%s]\n", toPrint->par);
    if(strcmp(toPrint->key, "") != 0)  
        fprintf(toRw, "%s=", toPrint->key);
    if(strcmp(toPrint->val, "") != 0)  
        fprintf(toRw, "%s\n", toPrint->val);
}

int scann(struct node *toRw) {
	char raw[128];
	strcpy(raw, "EOF");
	gets(raw);
	if(!strcmp(raw, "EOF")) return 0;
	if(raw[0] == 0) return 1;
    if(raw[0] == '[') {
        strcpy(toRw->par, strtok(raw, "[]"));
    } else {
    	strcpy(toRw->key, strtok(raw, "=\n"));
    	strcpy(toRw->val, strtok(NULL, "=\n"));
	}
	return 1;
}
int fscann(struct node *toRw, FILE *toRead) {
	char raw[128];
	strcpy(raw, "EOF");
	fgets(raw, 128, toRead);
	if(!strcmp(raw, "EOF")) return 0;
	if(raw[0] == 0) return 1;
    if(raw[0] == '[') {
        strcpy(toRw->par, strtok(raw, "[]"));
    } else {
    	strcpy(toRw->key, strtok(raw, "=\n"));
    	strcpy(toRw->val, strtok(NULL, "=\n"));
	}
	return 1;
}

void printl(struct node *head) {
    struct node *i;
    printn(head);
	for(i = head->next; i != head; i = i->next)
	    printn(i);
}

void fprintl(struct node *head, FILE *toRw) {
    struct node *i;
    printn(head);
	for(i = head->next; i != head; i = i->next)
	    fprintn(i, toRw);
}

struct node *parfroml(struct node *random, char *par) {
	struct node *i, 
	            *j;
	int c = 0, l = listlen(random);
	for(i = random; strcmp(i->par, par) != 0 && c <= l; i = i->next) 
	    c++;
	if(strcmp(par, i->par) != 0) 
	    return mknode();
	for(j = i; j->next->par[0] == 0; j = j->next);
	fromlist(i, j);
	return i;
}

void rmlist(struct node *toRm) {
	if(thead == 0) thead = toRm;
	if (toRm->next != thead) rmlist(toRm->next);
	free(toRm);
	thead = 0;
}

struct node *fscanl(FILE *toRead) {
	struct node *nList = mknode();
	while(fscann(nList, toRead)) {
	    struct node *lNode = mknode();
		tolist(nList, lNode, lNode);
		nList = lNode;
	}
	nList = nList->prev;
	struct node *lNode = nList->next;
	fromlist(lNode, lNode);
	rmlist(lNode);
	return nList;
}

struct node *scanl(void) {
	struct node *nList = mknode();
	while(scann(nList)) {
	    struct node *lNode = mknode();
		tolist(nList, lNode, lNode);
		nList = lNode;
	}
	nList = nList->prev;
	struct node *lNode = nList->next;
	fromlist(lNode, lNode);
	rmlist(lNode);
	return nList;
}

struct node *totemp(struct node *random) {
	int i;
	for(i = 0; temp[i] != 0; i++);
	temp[i] = random;
	return random;
}

struct node *nodebykey(char *key, struct node *random) { 
	thead = random->prev;
	if(strcmp(random->prev->key, key) == 0) return random->prev;
	for(; strcmp(random->key, key) != 0; random = random->next)
		if(thead == random) {
			struct node *n = mknode();
			strcpy(n->key, key);
			strcpy(n->val, "Not found in this list.");
			totemp(n);
			return n;
		}
	return random;
}

void cltemp() {
	int i;
	for(i = 0; temp[i] != 0; i++) {
		rmlist(temp[i]);
		temp[i] = 0;
	}
}