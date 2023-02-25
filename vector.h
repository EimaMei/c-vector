/*
	Original implementation by: https://aticleworld.com/implement-vector-in-c/
	Majority of the source code credit goes to that article. I only added a few new features and improvements to the code, and turned it into a small and simple portable header file.
*/

#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <stdlib.h> /* For malloc */
#include <string.h> /* For memcpy */

typedef void* vector;

/* Initializes the vector. */
vector vector_init();
/* Frees all of the memory from the vector. */
int vector_free(vector vec);

/* Pushes back 'item' to the back of the vector.*/
int vector_push_back(vector vec, void* item, size_t size_of_item);
/* Pops back the last element of the vector.*/
int vector_pop_back(vector vec);

/* Sets the 'index' of the vector to 'item'. */
int vector_set(vector vec, int index, void* item, size_t size_of_item);
/* Gets the 'index' of the vector. */
void* vector_get(vector vec, int index);
/* Returns a string version of the vector. */
char* vector_str(vector vec);

/* Deletes the specific 'index' item. */
int vector_erase(vector vec, int index);
/* Deletes everything in the vector. */
int vector_clear(vector vec);

/* Gets the size (amount of indexes) of the vector. */
size_t vector_size(vector vec);

/* Resizes the memory of the vector depending on the given capacity. Function is used internally and not made for external use. */
int vector_resize(vector vec, size_t capacity);



#ifdef VECTOR_IMPLEMENTATION

typedef struct internal_vector {
	void** items;     /* Value of the vector itself. */
	size_t size;      /* Current length of the vector. */
	size_t capacity;  /* Total allocated memory for the vector. */
} internal_vector;



vector vector_init() {
	internal_vector* vec = malloc(sizeof(internal_vector));

	if (vec) {
		vec->items = malloc(sizeof(*vec->items) * 8); /* Allocate 64 bytes (8 array allocations) to the vector. */
		vec->capacity = 8;
		vec->size = 0;
	}

	return vec;
}


int vector_resize(vector vec, size_t capacity) {
	internal_vector* v = vec;

	if (v) {
		void** items = realloc(v->items, sizeof(void*) * capacity);

		if (items) {
			v->items = items;
			v->capacity = capacity;

			return 0;
		}
	}

	return -1;
}


int vector_push_back(vector vec, void* item, size_t size_of_item) {
	internal_vector* v = vec;

	if (v) {
		void* copied_value = malloc(size_of_item); /* Copy the item's value into the array so that the value doesn't get accidentally overwritten. */
		memcpy(copied_value, item, size_of_item);

		if (v->capacity == v->size) { /* We don't have enough memory to store the next value. */
			if (vector_resize(vec, v->capacity * 2) != -1) /* Instead of realloc every two seconds, just multiply. Might be a little inefficient memory wise but CPU wise btter. */
				v->items[v->size++] = copied_value;
		}
		else /* We have enough memory. */
			v->items[v->size++] = copied_value;

		return 0;
	}

	return -1;
}


int vector_pop_back(vector vec) {
	internal_vector* v = vec;

	return vector_erase(vec, v->size - 1);
}


int vector_set(vector vec, int index, void* item, size_t size_of_item) {
	internal_vector* v = vec;

	if (v && (index >= 0) && (index < v->size)) {
		void* copied_value = malloc(size_of_item); /* Copy the item's value into the array so that the value doesn't get accidentally overwritten. */
		memcpy_s(copied_value, size_of_item, item, size_of_item);

		v->items[index] = copied_value;
		return 0;
	}

	return -1;
}


void* vector_get(vector vec, int index) {
	internal_vector* v = vec;
	void* readData = NULL;

	if (v) {
		if ((index >= 0) && (index < v->size))  /* 0 <= index < (size of vector). */
			readData = v->items[index];
	}
	return readData;
}


char* vector_str(vector vec) {
	internal_vector* v = vec;

	if (v) {
		char* result = malloc(v->size + 1);

		if (!result)
			return NULL;

		int i;
		for (i = 0; i < v->size; i++) /* Copy the vector to the result. */
			result[i] = *((short*)v->items[i]);
		result[v->size] = '\0';

		return result;
	}

	return NULL;
}


int vector_erase(vector vec, int index) {
	internal_vector* v = vec;

	if (v) {
		if ((index < 0) || (index >= v->size))
			return -1;

		v->items[index] = NULL;

		int i;
		for (i = index; i < v->size - 1; i++) { /* Now we gotta reshuffle the memory... */
			v->items[i] = v->items[i + 1];
			v->items[i + 1] = NULL;
		}

		v->size--; /* Less memory occupied. */
		if ((v->size > 0) && ((v->size) == (v->capacity / 4)))
			vector_resize(vec, v->capacity / 2);

		return 0;
	}
	return -1;
}


int vector_clear(vector vec) {
	internal_vector* v = vec;

	if (v) {
		free(v->items);

		v->items = malloc(sizeof(*v->items) * 8); /* Essentially just re-init the vector again. */
		v->capacity = 8;
		v->size = 0;

		return 0;
	}

	return -1;
}


int vector_free(vector vec) {
	internal_vector* v = vec;

	if (v) {
		free(v->items);
		free(v);

		return 0;
	}

	return -1;
}


size_t vector_size(vector vec) {
	internal_vector* v = vec;

	return v->size;
}

#endif /* C_VECTOR_IMPLEMENTATION */

#endif /* C_VECTOR_H */