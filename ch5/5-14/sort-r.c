/*
 * Filename:	sort-r.c
 * Author:		Andrew Suzuki <andrew.b.suzuki@gmail.com>
 * Date:		06/07/2015
 *
 * Exercise 5-14. Modify the sort program to handle a -r flag,
 * which indicates sorting in reverse (decreasing) order.
 * Be sure that -r works with -n.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXLINES 5000
char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void _qsort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(char *, char *);
int rnumcmp(char *, char *);
int rstrcmp(char *, char *);

int main(int argc, char *argv[]) {
	int i;
	int nlines;
	int numeric = false;
	int reverse = false;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n") == 0)
			numeric = true;
		else if (strcmp(argv[i], "-r") == 0)
			reverse = true;
	}

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		if (reverse) {
			_qsort((void **) lineptr, 0, nlines - 1,
				(int (*)(void *, void *))(numeric ? rnumcmp : rstrcmp));
		} else {
			_qsort((void **) lineptr, 0, nlines - 1,
				(int (*)(void *, void *))(numeric ? numcmp : strcmp));
		}
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("input too big to sort\n");
		return 1;
	}
}

void _qsort(void *v[], int left, int right, int (*comp)(void *, void *)) {
	int i, last;
	void swap(void *v[], int, int);

	if (left >= right)
		return;
	swap(v, left, (left + right) / 2);
	last = left;
	for (i = left + 1; i <= right; i++)
		if ((*comp)(v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	_qsort(v, left, last - 1, comp);
	_qsort(v, left + 1, right, comp);
}

#include <stdlib.h>

int numcmp(char *s1, char *s2) {
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

int rnumcmp(char *s1, char *s2) {
	return -numcmp(s1, s2);
}

int rstrcmp(char *s1, char *s2) {
	return -strcmp(s1, s2);
}

void swap(void *v[], int i, int j) {
	void *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

#define MAXLEN 1000
int _getline(char *, int);
char *alloc(int);

int readlines(char *lineptr[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = _getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			line[len - 1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

void writelines(char *lineptr[], int nlines)
{
	int i;

	for (i = 0; i < nlines; i++)
		printf ("%s\n", lineptr[i]);
}

int _getline(char s[], int limits)
{
    int i;
    for (i = 0; i < limits - 1 && (s[i] = getchar()) != EOF && s[i] != '\n'; i++);
    if (s[i] == '\n')
		i++;
    s[i] = '\0';
    return i;
}

#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n) {
		allocp += n;
		return allocp - n;
    } else
		return 0;
}