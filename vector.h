/*
	Original implementation by: https://aticleworld.com/implement-vector-in-c/
	Majority of the source code credit goes to that article. I only added a few new features and improvements to the code, and turned it into a small and simple portable header file.
*/

#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <stdlib.h> /* For malloc */
#include <string.h> /* For memcpy */

#ifdef VECTOR_PRINT_ERRORS
#include <stdio.h> /* For printf */
#endif

typedef void* vector;

/* Initializes the vector. */
vector vector_init();
/* Frees all of the memory from the vector. */
int vector_free(vector vec);

/* Pushes back 'item' to the back of the vector.*/
int vector_push_back(vector vec, void* item, size_t size_of_item);
/* Pops back the last element of the vector.*/
int vector_pop_back(vector vec);

/* Gets the 'index' of the vector. */
void* vector_get(vector vec, size_t index);
/* Gets the first element of the vector. */
void* vector_front(vector vec);
/* /* Gets the last element of the vector. */
void* vector_end(vector vec);
/* Returns a string version of the vector. */
char* vector_str(vector vec, char* str_to_add_between_elements);

/* Sets the 'index' of the vector to 'item'. */
int vector_set(vector vec, size_t index, void* item, size_t size_of_item);
/* Inserts an item into the provided index. */
int vector_insert(vector vec, size_t index, void* item, size_t size_of_item);
/* Deletes the specific 'index' item. */
int vector_erase(vector vec, size_t index);
/* Deletes everything in the vector. */
int vector_clear(vector vec);

/* Gets the size (amount of indexes) of the vector. */
size_t vector_size(vector vec);


#ifdef VECTOR_IMPLEMENTATION

/* Resizes the memory of the vector depending on the given capacity. Function is used internally and not made for external use. */
int vector_resize(vector vec, size_t capacity);


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

	if (!v) {
		#ifdef VECTOR_PRINT_ERRORS
		printf("vector_resize: Provided vector is an invalid vector handle.\n");
		#endif
		return -1;
	}

	void** items = realloc(v->items, sizeof(void*) * capacity);

	if (items) {
		v->items = items;
		v->capacity = capacity;

		return 0;
	}

	return -1;
}


int vector_push_back(vector vec, void* item, size_t size_of_item) {
	internal_vector* v = vec;

	if (!v) {
		#ifdef VECTOR_PRINT_ERRORS
		printf("vector_push_back: Provided vector is an invalid vector handle.\n");
		#endif
		return -1;
	}

	if (v->capacity == v->size) {
		if (vector_resize(vec, v->capacity * 2) != 0) {
			#ifdef VECTOR_PRINT_ERRORS
			printf("vector_push_back: Failed to resize vector.\n");
			#endif
			return -2;
		}
	}

	void* new_item = malloc(size_of_item);
	if (!new_item) {
		#ifdef VECTOR_PRINT_ERRORS
		printf("vector_push_back: Failed to allocate a copy of 'item'.\n");
		#endif
		return -3;
	}

	void* res = memcpy(new_item, item, size_of_item);
	if (!res) {
		#ifdef VECTOR_PRINT_ERRORS
		printf("vector_push_back: Failed to copy 'item' to array.\n");
		#endif
		return -4;
	}

	v->items[v->size++] = new_item;

	return 0;
}


int vector_pop_back(vector vec) {
	internal_vector* v = vec;

	return vector_erase(vec, v->size - 1);
}


int vector_set(vector vec, size_t index, void* item, size_t size_of_item) {
	internal_vector* v = vec;

	if (v && (index >= 0) && (index < v->size)) {
		void* copied_item = malloc(size_of_item);
		memcpy(copied_item, item, size_of_item);

		if (copied_item == NULL)
			return -2;

		v->items[index] = copied_item;

		return 0;
	}

	return -1;
}


void* vector_get(vector vec, size_t index) {
	internal_vector* v = vec;
	void* readData = NULL;

	if (v && (index >= 0) && (index < v->size))  /* 0 <= index < (size of vector). */
		readData = v->items[index];

	return readData;
}


void* vector_front(vector vec) {
	internal_vector* v = vec;
	void* readData = NULL;

	if (v && (0 < v->size))
		readData = v->items[0];

	return readData;
}


void* vector_end(vector vec) {
	internal_vector* v = vec;
	void* readData = NULL;

	if (v && 0 < v->size)
		readData = v->items[v->size - 1];

	return readData;
}


char* vector_str(vector vec, char* str_to_add_between_elements) {
	internal_vector* v = vec;

	if (!v)
		return NULL;


	size_t i, size = 0;
	for (i = 0; i < v->size; i++) {
		char* txt = (char*)vector_get(vec, i);

		if (i != v->size - 1)
			size += strlen(txt) + strlen(str_to_add_between_elements);
		else
			size += strlen(txt);

	}

	char* result = malloc(size + 1);
    char* ptr = result;

    for (i = 0; i < v->size; i++) {
        char* txt = (char*)vector_get(vec, i);

		if (i != v->size - 1)
			ptr += sprintf(ptr, "%s%s", txt, str_to_add_between_elements);
		else
			ptr += sprintf(ptr, "%s", txt);
    }
    *ptr = '\0'; /* Since we're at the end of our sentence, this line is equivalent to doing 'ptr[strlen(ptr)] = '\0';' */

	return result;
}


int vector_insert(vector vec, size_t index, void* item, size_t size_of_item) {
	internal_vector* v = vec;

	if (!v)
		return -1;

	if ((index < 0) || (index >= v->size))
		return -1;


	void* copied_item = malloc(size_of_item);
	memcpy(copied_item, item, size_of_item);

	v->size++; /* Less memory occupied. */

	if (v->size == v->capacity)
		vector_resize(vec, v->capacity * 2);


	size_t i;
	void* new_value = copied_item;
	for (i = index; i < v->size; i++) { /* Now we gotta reshuffle the memory... */
		void* og = v->items[i];
		v->items[i] = new_value;
		new_value = og;
	}

	return 0;
}


int vector_erase(vector vec, size_t index) {
	internal_vector* v = vec;

	if (!v)
		return -1;

	if ((index < 0) || (index >= v->size))
		return -1;

	v->items[index] = NULL;

	size_t i;
	for (i = index; i < v->size - 1; i++) { /* Now we gotta reshuffle the memory... */
		v->items[i] = v->items[i + 1];
		v->items[i + 1] = NULL;
	}

	v->size--; /* Less memory occupied. */
	if ((v->size > 0) && ((v->size) == (v->capacity / 4)))
		vector_resize(vec, v->capacity / 2);

	return 0;
}


int vector_clear(vector vec) {
	internal_vector* v = vec;

	if (!v)
		return -1;

	free(v->items);

	v->items = malloc(sizeof(*v->items) * v->capacity); /* Essentially just re-init the vector again. */
	v->size = 0;

	return 0;
}


int vector_free(vector vec) {
	internal_vector* v = vec;

	if (!v)
		return -1;

	free(v->items);
	free(v);

	return 0;
}


size_t vector_size(vector vec) {
	internal_vector* v = vec;

	return v->size;
}

#endif /* C_VECTOR_IMPLEMENTATION */

#endif /* C_VECTOR_H */
