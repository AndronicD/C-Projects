#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"tema.h"

#define MAX_LEN 100

TCarte amestecare_tip(char* tip, TCarte pack, FILE* inputfile){
    if(strcmp(tip,"inversare")==0)
		pack = inversare(pack);
    if(strcmp(tip,"intercalare")==0)
		pack = intercalare(pack);
    if(strcmp(tip,"mongean")==0)
		pack = mongean(pack);
    if(strcmp(tip,"npiles")==0)
		pack = npiles(pack,inputfile);
	return pack;
}

int main(){
    int nr_am, i;
    TCarte pack = createPack();
	
    FILE* inputfile = fopen("in", "r");
    FILE* outputfile = fopen("out", "w");
	
    char *cerinta = (char*)malloc(MAX_LEN * sizeof(char));
    char *tip = (char*)malloc(MAX_LEN * sizeof(char));
	
	
	fscanf(inputfile,"%s", cerinta);
	fscanf(inputfile,"%d", &nr_am);
	
	for(i=0; i<nr_am; i++){
		fscanf(inputfile,"%s",tip);
		pack = amestecare_tip(tip,pack,inputfile);
	}
	
	if(strcmp(cerinta,"amestecare")==0)
		printPackOut(pack,outputfile);
	
    fclose(inputfile);
    fclose(outputfile);
    pack = freePack(pack);
    free(cerinta);
    free(tip);
	
    return 0;
}