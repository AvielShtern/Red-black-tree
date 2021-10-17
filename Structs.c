/**
* @file Structs.c
* @author Aviel Shtern Aviel.Shtern@mail.huji.ac.il
* @version 1.0
* @date 3 jun 2020
* @brief A Library that realizes binary tree uses. Vectors and strings.
*/

#include <stdlib.h>
#include "Structs.h"
#include <string.h>
#include <stdbool.h>

/**
 *@def LESS (-1)
 *@brief  It will be used during comparison to indicate that the object on the right is smaller than
 * the object on the left
 */
#define LESS (-1)

/**
 *@def EQUAL (0)
 *@brief It will be used during comparison to indicate that the object on the right is equal to the
 * object on the left
 */
#define EQUAL (0)

/**
 *@def GREATER (1)
 *@brief It will be used during comparison to indicate that the object on the right is larger than
 * the object on the left
 */
#define GREATER (1)

/**
 * Calculates the norm (in squared!!!) of the vector
 * @param pVector object with type Vector
 * @return the norm (in squared!!) of the vector
 */
double calculateTheNormSquared(const Vector *pVector);

/**
 * receive two vectors and returns who has the larger norm
 * @param first object with type Vector
 * @param second object with type Vector
 * @return equal to 0 iff norm of first == norm of second (nof == nos). lower than 0 if nof < nos.
 * Greater than 0 iff nof > nos.
 */
int compareNorm(const Vector *first, const Vector *second);

int vectorCompare1By1(const void *a, const void *b)
{
	Vector *first = (Vector *) a;
	Vector *second = (Vector *) b;
	int range = (first->len < second->len) ? first->len : second->len;
	for (int i = 0; i < range; i++)
	{
		if (first->vector[i] < second->vector[i])
		{
			return LESS;
		}
		else if (first->vector[i] > second->vector[i])
		{
			return GREATER;
		}
	}
	return first->len - second->len; // in this case Whoever is longer is bound to be the bigger one
}

double calculateTheNormSquared(const Vector *pVector)
{
	double normVec = 0;
	for (int i = 0; i < pVector->len; i++)
	{
		normVec += pVector->vector[i] * pVector->vector[i];
	}
	return normVec;
}

int compareNorm(const Vector *first, const Vector *second)
{
	double normOfFirst = calculateTheNormSquared(first);
	double normOfSecond = calculateTheNormSquared(second);
	if (normOfFirst > normOfSecond)
	{
		return GREATER;
	}
	else if (normOfFirst < normOfSecond)
	{
		return LESS;
	}
	return EQUAL;
}

int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	Vector *curVec = (Vector *) pVector;
	Vector *maxVector = (Vector *) pMaxVector;
	if (curVec == NULL || maxVector == NULL || curVec->vector == NULL)
	{
		return false;
	}
	else if (maxVector->vector == NULL || compareNorm(curVec, maxVector) == GREATER)
	{
		maxVector->vector = (double *) realloc(maxVector->vector, (curVec->len) * sizeof(double));
		if (maxVector->vector == NULL)
		{
			return false;
		}
		memcpy(maxVector->vector, curVec->vector, curVec->len * sizeof(double)); // deep copy
		maxVector->len = curVec->len; // we copy just the field "vector" in memcpy function
	}
	return true;
}

Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	Vector *pMaxVector = (Vector *) calloc(1, sizeof(Vector));
	if (pMaxVector == NULL)
	{
		return NULL;
	}
	forEachRBTree(tree, copyIfNormIsLarger, pMaxVector);
	return pMaxVector;

}

void freeVector(void *pVector)
{
	if (pVector == NULL)
	{
		return;
	}
	Vector *vector = (Vector *) pVector;
	free(vector->vector);
	vector->vector = NULL;
	free(vector);
}

void freeString(void *s)
{
	free(s);
}

int stringCompare(const void *a, const void *b)
{
	char *str1 = (char *) a;
	char *str2 = (char *) b;
	return strcmp(str1, str2);
}

int concatenate(const void *word, void *pConcatenated)
{
	char *src = (char *) word;
	char *dest = (char *) pConcatenated;
	if (strcat(dest, src) != NULL && strcat(dest, "\n") != NULL)
	{
		return true;
	}
	return false;

}