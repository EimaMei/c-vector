#define VECTOR_IMPLEMENTATION
#include <vector.h>
#include <stdio.h>


int main(void) {
	vector vec = vector_init();

	if (vec == NULL) { /* Make sure the vector was properly intialized. */
		printf("Error: Failed to init a vector\n");
		return 1;
	}


	/* Setup some random example variables. */
	char* txt = "Some text over here";
	char over_allocated_text[256] = "This makes me feel like a rat";

	int hex = 0xFF;
	char letter = 'A';


	vector_push_back(vec, txt, sizeof(txt) + 1); /* Added + 1 for the NULL character. */
	vector_push_back(vec, "Labas, pasauli!", sizeof("Labas, pasauli!") + 1);


	int i;
	for (i = 0; i < vector_len(vec); i++) { /* We get the element count of the array (array's size/length) via vector_len(vec). With that we can iterate through the vector. */
		printf("At index '%i', we have value \"%s\"\n", i, vector_get(vec, i)); /* Since we know all of the elements in the array are strings, we can print them via the string format. */
	}

	txt = "CHANGED TEXT OVER HERE";
	printf("Even though variable 'txt' now equals \"%s\", 'vec' variable's index 0 doesn't get overwritten as its value is still \"%s\"\n", txt, vector_get(vec, 0));


	/*
	 	Delete the first element as we don't need it anymore.
		Since the first element is erased, every other elements' index decrements by one (meaning the second value's index is now 0, not 1).
	*/
	vector_delete(vec, 0);
	printf("Current size of 'vec': %i\n", vector_len(vec));


	/*
		Since we know we only need to copy the *actual* value of the string (and not the entire item),
		we can just tell the function to only copy the bytes that necessary (in this case, the length of the text plus NULL character).
	*/
	vector_set(vec, 0, over_allocated_text, strlen(over_allocated_text) + 1);
	printf("Index 0 is now \"%s\"\n", vector_get(vec, 0));


	/* We can also insert any other type other than char since this is a void*. */
	vector_push_back(vec, &hex, sizeof(int));
	vector_push_back(vec, &letter, sizeof(letter));
	printf("Current size of 'vec': %i\n", vector_len(vec));


	vector_delete_everything(vec); /* Removes every value from the vector. */
	printf("Current size of 'vec': %i\n", vector_len(vec));


	vector_free(vec); /* Free the variable since we don't wanna use it anymore.*/

	return 0;
}