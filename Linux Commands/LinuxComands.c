#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

typedef struct StackNode{
	char* nume;
	struct StackNode* next;
}StackNode;

typedef struct Stack{
	StackNode* head;
}Stack;

Stack* createStack(void){
	Stack* s = (Stack*)malloc(sizeof(Stack));
	s->head = NULL;
	//s->size = 0;
	return s;
}

int isStackEmpty(Stack* stack){
	//if(stack == NULL) return 0;
	//if(stack->head == NULL) return 1;
	return (stack == NULL || stack->head == NULL);
}

void push(Stack *stack, char* elem){
	StackNode* item=(StackNode*)malloc(sizeof(StackNode));
	int len = strlen(elem);
	item->nume = (char*)malloc((len+1)*sizeof(char));
	memcpy(item->nume, elem, len+1);
	item->next = stack->head;
	stack->head = item;
	//stack->size = stack->size + 1;
	return;
}

char* top(Stack *stack){	
	return stack->head->nume;
} 

void pop(Stack *stack){
	StackNode* iter = stack->head;
	stack->head = iter->next;
	free(iter->nume);
	free(iter);
	//stack->size = stack->size - 1;
	//iter->next = NULL;
	return;
}

void destroyStack(Stack *stack){
	StackNode* iter = stack->head;
	while(iter != NULL){
		StackNode* t = iter;
		stack->head->next = iter->next;
		iter = iter->next;
		//free(t->nume);
		free(t); 
	}
	free(stack);
}


void print_stdout_file(){
	fprintf(stdout, "%s\n", "File already exists");
}

void print_stdout_dir(){
	printf("%s\n", "Directory already exists");
}

void touch (Dir* parent, char* name) {
	File* f = (File*)malloc(sizeof(File));
	int len = strlen(name);
	f->name = (char*)malloc((len+1)*sizeof(char));
	memcpy(f->name, name, len+1);
	f->parent = parent;
	f->next = NULL;
	if(f->parent->head_children_files == NULL){
		f->parent->head_children_files = f;
		f->parent->head_children_files->next = NULL;
		return;
	}
	else{
		File* iter = f->parent->head_children_files;
		while(iter != NULL){
			if(!strcmp(iter->name, name)){
				print_stdout_file();
				free(f->name);
				free(f);
				return;
			}
			iter = iter->next;
		}
	}
	File* iter = f->parent->head_children_files;
	while(iter->next != NULL){
		iter = iter->next;
	}
	iter->next = f;
	iter->next->next = NULL;
	return;
}

void mkdir (Dir* parent, char* name) {
	Dir* d = (Dir*)malloc(sizeof(Dir));
	int len = strlen(name);
	d->name = (char*)malloc((len+1)*sizeof(char));
	memcpy(d->name, name, len+1);
	d->head_children_dirs = NULL;
	d->head_children_files = NULL;
	d->parent = parent;
	d->next = NULL;
	if(d->parent->head_children_dirs == NULL){
		d->parent->head_children_dirs = d;
		d->parent->head_children_dirs->next = NULL;
		return;
	}
	else{
		Dir* iter = d->parent->head_children_dirs;
		while(iter != NULL){
			if(!strcmp(iter->name, name)){
				print_stdout_dir();
				free(d->name);
				free(d);
			return;
			}
			iter = iter->next;
		}
	}
	Dir* iter = d->parent->head_children_dirs;
	while(iter->next != NULL){
		iter = iter->next;
	}
	iter->next = d;
	iter->next->next = NULL;
	return;
}

void print_dir(Dir* head){
	Dir* iter = head;
	while(iter != NULL){
		fprintf(stdout,"%s\n", iter->name);
		iter = iter->next;
	}
	return;
}

void print_file(File* head){
	File* iter = head;
	while(iter != NULL){
		fprintf(stdout, "%s\n", iter->name);
		iter = iter->next;
	}
	return;
}

void ls (Dir* parent){
	print_dir(parent->head_children_dirs);
	print_file(parent->head_children_files);
}

void rm (Dir* parent, char* name) {
	File* iter = parent->head_children_files;
	if(iter == NULL){
		printf("Could not find the file\n");
		return;
	}
	if(!strcmp(iter->name, name)){
		parent->head_children_files = iter->next;
		free(iter->name);
		free(iter);
		return;
	}
	while(iter->next != NULL){
		if(!strcmp(iter->next->name, name)){
			File* t = iter->next;
			iter->next = iter->next->next;
			free(t->name);
			free(t);
			return;
		}
		iter = iter->next;
	}
	if(!strcmp(iter->name, name)){
		free(iter->name);
		free(iter);
		return;
	}
	else{
		printf("Could not find the file\n");
	}
}
void del_file(Dir* parent){
	File* iter = parent->head_children_files;
	while(iter != NULL){
		File* t = iter;
		iter = iter->next;
		free(t->name);
		free(t);
	}
	return;
}

void rmdir (Dir* parent, char* name){
	Dir* iter = parent->head_children_dirs;
	//printf("ajunge aici");
	if(iter == NULL){
		fprintf(stdout,"Could not find the dir\n");
		return;
	}
	if(!strcmp(iter->name, name)){
		parent->head_children_dirs = iter->next;
		del_file(iter);
		free(iter->name);
		free(iter);
		return;
	}
	while(iter->next != NULL){
		if(!strcmp(iter->next->name, name)){
			Dir* d = iter->next;
			iter->next = iter->next->next;
			del_file(d);
			free(d->name);
			free(d);
			return;
		}
		iter = iter->next;
	}
	//printf("ajunge aici");
	if(!strcmp(iter->name, name)){
		del_file(iter);
		free(iter->name);
		free(iter);
		return;
	}
	else{
		fprintf(stdout, "Could not find the dir\n");
	}
}

void cd(Dir** target, char *name) {
	Dir* iter = (*target)->head_children_dirs;
	if(!strcmp(name, "..")){
		if((*target)->parent != NULL)
			*target = (*target)->parent;
		return;
	}
	int found = 0;
	while(iter != NULL){
		if(!strcmp(iter->name, name)){
			*target = iter;
			found = 1;
			break;
		}
		else iter = iter->next;
	}
	if(found) return;
	else{
		printf("No directories found!\n");
		return;
	}
}

char *pwd (Dir* target) {
	char* final = (char*)calloc(sizeof(char), 50);
	Stack *s = createStack();
	Dir* iter = target;
	while(iter != NULL){
		push(s, iter->name);
		iter = iter->parent;
	}
	while(!isStackEmpty(s)){
		strcat(final, "/");
		strcat(final, top(s));
		pop(s);
	}
	free(s);
	return final;
}

void free_file(File* head){
	if(head == NULL){
		return;
	}
	File* iter = head;
	while(iter != NULL){
		File* t = iter;
		iter = iter->next;
		free(t->name);
		free(t);
	}
	return;	
}

void free_dir(Dir* head){
	if(head == NULL){
		return;
	}
	Dir* iter = head;
	while(iter != NULL){
		if(iter->head_children_dirs != NULL){
			free_dir(iter->head_children_dirs);
		}
		if(iter->head_children_files != NULL){
			free_file(iter->head_children_files);
		}
		Dir* t = iter;
		iter = iter->next;
		free(t->name);
		free(t);
	}
	return;
}

void stop (Dir* target) {
	free_dir(target);
}

void print_dir_level(Dir* dir, int level){
	while(level){
		printf("    ");
		level--;
	}
	printf("%s\n", dir->name);
}

void print_file_level(File* file, int level){
	while(level){
		printf("    ");
		level--;
	}
	printf("%s\n", file->name);
}

void tree (Dir* target, int level) {
	if(target->head_children_dirs == NULL && target->head_children_files == NULL){
		return;
	}
	Dir* iter_d = target->head_children_dirs;
	File* iter_f = iter_d->parent->head_children_files;
	while(iter_d != NULL){
		print_dir_level(iter_d, level);
		if(iter_d->head_children_dirs != NULL){
			tree(iter_d, level+1);
		}
		while(iter_f != NULL){
			print_file_level(iter_f, level);
			iter_f = iter_f->next;
		}
		iter_d = iter_d->next;
	}
	return;
}
int find_dir(Dir* dir, char* name){
	int found = 0;
	while(dir != NULL){
		if(!strcmp(dir->name, name)){
			found = 1;
			break;
		}
		dir = dir->next;
	}
	return found;
}

int find_file(File* file, char* name){
	int found = 0;
	while(file != NULL){
		if(!strcmp(file->name, name)){
			found = 1;
			break;
		}
		file = file->next;
	}
	return found;
}

void mv(Dir* parent, char *oldname, char *newname){
	Dir* iter_d = parent->head_children_dirs;
	File* iter_f = parent->head_children_files;
	//printf("intra");
	int not_find_dir = find_dir(iter_d, oldname);
	int not_find_file = find_file(iter_f, oldname);
	if(!(not_find_dir || not_find_file)){
		printf("File/Director not found\n");
		return;
	}
	int found_dir = find_dir(iter_d, newname);
	int found_file = find_file(iter_f, newname);
	if(found_dir || found_file){
		printf("File/Director already exists\n");
		return;
	}
	while(iter_d != NULL){
		if(!strcmp(iter_d->name, oldname)){
			if(iter_d->next != NULL){
				Dir* it = iter_d->next;
				iter_d->parent->head_children_dirs = it;
				while(it->next != NULL){
					//printf("intra");
					it = it->next;
				}
				it->next = iter_d;
				iter_d->next = NULL;
				strcpy(iter_d->name, newname);
				return;
			}
			else{
				strcpy(iter_d->name, newname);
				return;
			}
			iter_d = iter_d->next;
		}
	}
	while(iter_f != NULL){
		if(!strcmp(iter_f->name, oldname)){
			if(iter_f->next != NULL){
				File* it = iter_f->next;
				iter_f->parent->head_children_files = it;
				while(it->next != NULL){
					it = it->next;
				}
				it->next = iter_f;
				iter_f->next = NULL;
				strcpy(iter_f->name, newname);
				return;
			}
			else{
				strcpy(iter_f->name, newname);
				return;
			}
			iter_f = iter_f->next;
		}
	}
}

int main () {
	Dir* home = (Dir*)malloc(sizeof(Dir));
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->next = NULL;
	home->parent = NULL;
	home->name = (char*)malloc(5*sizeof(char));
	home->name[0] = '\0';
	strcpy(home->name, "home");
	char* command = (char*)malloc(30*sizeof(char));
	Dir** current_dir = &home;
	Dir* cpy_home = home;
	do
	{
		/*Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		fscanf(stdin, "%s", command);
		if(!strcmp(command,"mkdir")){
			char* argument = (char*)malloc(3*sizeof(char));
			char* new = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", new);
			strcpy(argument, new);
			mkdir(*current_dir, argument);
			free(argument);
			free(new);
		}
		else if(!strcmp(command, "touch")){
			char* argument = (char*)malloc(3*sizeof(char));
			char* new = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", new);
			strcpy(argument, new);
			touch(*current_dir, argument);
			free(argument);
			free(new);
		}
		else if(!strcmp(command, "ls")){
			ls(*current_dir);
		}
		else if(!strcmp(command, "rm")){
			char* argument = (char*)malloc(3*sizeof(char));
			char* new = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", new);
			strcpy(argument, new);
			rm(*current_dir, argument);
			free(argument);
			free(new);
		}
		else if(!strcmp(command, "rmdir")){
			char* argument = (char*)malloc(3*sizeof(char));
			char* new = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", new);
			strcpy(argument, new);
			rmdir(*current_dir, argument);
			free(argument);
			free(new);
		}
		else if(!strcmp(command, "cd")){
			char* argument = (char*)malloc(3*sizeof(char));
			char* new = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", new);
			strcpy(argument, new);
			cd(current_dir, argument);
			free(argument);
			free(new);
		}
		else if(!strcmp(command, "tree")){
			tree(*current_dir, 0);
		}
		else if(!strcmp(command, "pwd")){
			char* final = pwd(*current_dir);
			fprintf(stdout, "%s", final);
			free(final);
		}
		else if(!strcmp(command, "mv")){
			char* old_name = (char*)malloc(3*sizeof(char));
			char* new_name = (char*)malloc(3*sizeof(char));
			char* arg1 = (char*)malloc(3*sizeof(char));
			char* arg2 = (char*)malloc(3*sizeof(char));
			fscanf(stdin, "%s", arg1);
			strcpy(old_name, arg1);
			fscanf(stdin, "%s", arg2);
			strcpy(new_name, arg2);
			mv(*current_dir, old_name, new_name);
			free(old_name);
			free(new_name);
			free(arg1);
			free(arg2);
		}
		else if(!strcmp(command, "stop")){
			stop(cpy_home);
		}
		else{
			continue;
		}
	}while(strcmp(command, "stop"));
	free(command);
	return 0;
}
