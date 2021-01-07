#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct singly_linked_list_node singly_linked_list_node;
typedef struct singly_linked_list singly_linked_list;

char* readline();
char** split_string(char*);

int hash_func (char *word, int m);
void free_singly_linked_list(singly_linked_list_node *node);
void insert_node_into_singly_linked_list(singly_linked_list *singly_linked_list,
                                                   char node_data[], int count);
void insert_word (singly_linked_list *sll, char *word);
bool check_linked_lists (singly_linked_list_node *node_n,
                                               singly_linked_list_node *node_m); 
bool compare_hash_tables (singly_linked_list **hash_table_n,
                                             singly_linked_list **hash_table_m);
void free_hash_table (singly_linked_list **hash_table, int array_length);
void checkMagazine(int magazine_count, char **magazine, int note_count,
                                                                   char **note);


struct singly_linked_list_node {
    char *data;
    int count;
    singly_linked_list_node *next;
};

struct singly_linked_list {
    singly_linked_list_node *head;
    singly_linked_list_node *tail;
};

//create a singly linked list new node and assign data
singly_linked_list_node *create_singly_linked_list_node(char *node_data,
                                                               int node_count) {
    singly_linked_list_node *node = malloc(sizeof(singly_linked_list_node));
    node->data = node_data;
    node->count = node_count;
    node->next = NULL;
    return node;
}

//create a singly linked list and assign head and tail
singly_linked_list *create_singly_linked_list(singly_linked_list_node 
                                                                   *firstNode) {
    singly_linked_list *newList = malloc(sizeof(singly_linked_list));
    newList->head = firstNode;
    newList->tail = firstNode;
    return newList;
}

//insert new node at the end of the singly linked list
void insert_node_into_singly_linked_list(singly_linked_list *singly_linked_list,
                                                  char node_data[], int count) {
    singly_linked_list_node* node= create_singly_linked_list_node(node_data, 1);
    if (!(singly_linked_list)->head) {
        (singly_linked_list)->head = node;
    }
    else {
        (singly_linked_list)->tail->next = node;
    }
    (singly_linked_list)->tail = node;
}

//free memory used for the singly linked list
void free_singly_linked_list(singly_linked_list_node* node) {
    while (node) {
        singly_linked_list_node *temp = node;
        node = node->next;
        free(temp);
    }
}

//evaluate the word as the sum of its characters ascii values
int hash_func (char *word, int m) {
    int sum=0;
    int i=0;
    while (*(word+i)!='\0') {
        sum+=*(word+i);
        i++;
    }
    //return the sum mod m to avoid illegal memory access
    return (sum%m);
}

//check for recurrence of the word and insert in a new node only if the word
//hasn't appeared yet with else increment count in the correct node
void insert_word (singly_linked_list *sll, char *word) {
    singly_linked_list_node *current_node = sll->head;
    while (current_node!=NULL) {
        if (!strcmp(word, current_node->data)) {
            current_node->count++;
            return;
        }
        else {
        current_node = current_node->next;
        }
    }
    //if we reach here, this means the word hasn't appeared yet, so we add it
    //in a new node and assign the count 1
    insert_node_into_singly_linked_list (sll, word, 1);
    return;
}

//check if the second linked list contains all the words and the appropriate 
//count that appear in the first linked list
bool check_linked_lists (singly_linked_list_node *node_n,
                                              singly_linked_list_node *node_m) {
    //save node of magazine in a temporary variable in order to be able to 
    //retrieve it for the while loop
    singly_linked_list_node *temp = node_m;
    while (node_n->next != NULL) {
        while (node_m->next != NULL) {
            if (!strcmp (node_n->data, node_m->data)) {
                if (node_n->count > node_m->count) {
                    //if we reach here this means that some word exists in note
                    //but magazine doesn't have sufficient count
                    return false;
                }
            }
            node_m = node_m->next;
        }
        //change the node of magazine back to its original value
        node_m = temp;
        node_n = node_n->next;
    }
    return true;
}

// compare hash tables, if both hash tables in index i are not null check linked 
// list at index i.
bool compare_hash_tables (singly_linked_list **hash_table_n,
                                            singly_linked_list **hash_table_m) {
    for (int i=0; i<1000; i++) {
        if (hash_table_n[i]==NULL) {
            continue;
        }
        else {
            if (hash_table_m[i]==NULL) {
                return false;
            }
            else {
                // if both hash tables are not null at idx i, check linked lists
                if (!check_linked_lists (hash_table_n[i]->head,
                                 hash_table_m[i]->head)) {
                    // if the linked list check didnt pass, note words are not 
                    //included in magazine.
                    return false;
                }
            }
        }
    }
    return true;
}
// free memory that was allocated for the hash tables.
void free_hash_table (singly_linked_list **hash_table, int array_length) {
    for(int i=0; i<array_length; i++) {
        if (hash_table[i]!=NULL) {
            free_singly_linked_list(hash_table[i]->head);
        }
        free(hash_table[i]);
    }
    free(hash_table);
}

// Complete the checkMagazine function below.
void checkMagazine(int magazine_count, char **magazine, int note_count,
                                                                  char **note) {
    int array_length=1000;
    // start the hash tables
    singly_linked_list **hash_table_n = malloc(8*array_length);
    singly_linked_list **hash_table_m = malloc(8*array_length);
    // filling magazine hash table
    for (int i=0; i<magazine_count; i++) {
        int hash_num = hash_func(*(magazine+i), array_length);
        char *word = *(magazine+i);
        //check if there's isn't a linked list yet and create one
        if(hash_table_m[hash_num] == NULL){
            singly_linked_list_node *new_node =
                                         create_singly_linked_list_node(word,1);
            hash_table_m[hash_num] = create_singly_linked_list(new_node);
        }
        //if there is, add the word accordingly
        else {
            insert_word (hash_table_m[hash_num], *(magazine+i));
        }
    }
    //filling note hash table
    for (int j=0; j<note_count; j++) {
        int hash_num = hash_func(*(note+j), array_length);
        char *word = *(note+j);
        if(hash_table_n[hash_num] == NULL){
            singly_linked_list_node *new_node =
                                         create_singly_linked_list_node(word,1);
            hash_table_n[hash_num] = create_singly_linked_list(new_node);
        }
        else {
            insert_word (hash_table_n[hash_num], *(note+j));
        }
    }
    //save result in a separate variable to free hash tables
    bool result = compare_hash_tables (hash_table_n, hash_table_m);
    free_hash_table (hash_table_n, array_length);
    free_hash_table (hash_table_m, array_length);
    if (result) {
        printf ("Yes");
    }
    else {
        printf("No");
    }
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
