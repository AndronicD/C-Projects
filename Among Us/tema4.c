#include "utils.h"



void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{
	int n = *((int*) input);
	int i, j;
	int **mat;
	char *str1;
	mat = (int**)malloc(n * sizeof(int *));
	for(i = 0; i < n; i++)
	{
		mat[i] = calloc(n, sizeof(int));
	}
	str1 = (char*)malloc(MAX_LEN_STR_OUT);
	char *s = (char*)malloc(MAX_LEN_STR_OUT);
	strcpy(s,"");
  	char *s1 = (char*)malloc(MAX_LEN_STR_OUT);
  	char *s2 = (char*)malloc(MAX_LEN_STR_OUT);
	for(i = 0; i < n; i++)
	{
    	for(j = 0; j < n; j++)
    	{
      		sprintf(s1,"%d", i+1);
      		sprintf(s2,"%d", j+1);
      		strcat(s1,s2);
      		mat[i][j]=atoi(s1);
    	}
  	}
	for (int i = 0; i < n / 2; i++) {
        for (int j = i; j < n - i - 1; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[n - 1 - j][i];
            mat[n - 1 - j][i] = mat[n - 1 - i][n - 1 - j];
            mat[n - 1 - i][n - 1 - j] = mat[j][n - 1 - i];
            mat[j][n - 1 - i] = temp;
        }
    }
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{	
		
			sprintf(str1,"%d ", mat[i][j]);
			if(j == n-1 && i != n-1)
			sprintf(str1, "%d\n", mat[i][j]);
			strcat(s, str1);
		}
	}
	free(s1);
	free(s2);
	free(str1);
	for(i = 0; i < n; i++)
	 {
		 free(mat[i]);
	 }
	 free(mat);
	return s;
}

// Task 2
void *DecodeString(void *input)
{
	char* sir = (char*) input;
	char *sir2 = (char*)malloc(10*sizeof(char));
	char *p;
	int aux;
	int sum=0;
	p = strtok(sir, "_");
  	while(p != NULL){
		aux = atoi(p);
    	sum = sum + aux;
    	p = strtok(NULL, "_");
  	}
	
  	sprintf(sir2, "%d", sum);
	return sir2;
}

//Task 3
void *InvertArray(void *input)
{
	int* sir1 = (int*) input;
	int n = sir1[0];
	char *sir = (char*)malloc(MAX_LEN_STR_ATR);
	char *sir2 = (char*)malloc(MAX_LEN_STR_ATR);
	int i, aux;
	if(n%2==0)
	{
		for(i = 1; i <= n; i+=2)
		{
			aux = sir1[i];
			sir1[i] = sir1[i+1];
			sir1[i+1] = aux;
		}
	}
	else if(n%2==1) 
	{
		for(i = 1; i <= n/2; i++)
		{
			aux = sir1[i];
			sir1[i] = sir1[n-i+1];
			sir1[n-i+1] = aux;
		}
	}
	strcpy(sir,"");
	for(i = 1; i <= n; i++){
	sprintf(sir2, "%d ", sir1[i]);
	strcat(sir, sir2);
	}
	free(sir2);
	return sir;
}

//Task 4
Player *allocPlayer()
{
	Player *player= (Player*)malloc(sizeof(Player));
	player->name = (char*)malloc(MAX_LEN_STR_ATR);
	player->hat = (char*)malloc(MAX_LEN_STR_ATR);
	player->color = (char*)malloc(MAX_LEN_STR_ATR);
	player->alive = 1;
	
	player->numberOfLocations = 0;
	player->indexOfLocation = 0;
	
	return player;
}

//Task 4
Game *allocGame()
{
	Game *game= (Game*)malloc(sizeof(Game));
	game->name = (char*)malloc(MAX_LEN_STR_ATR);
	return game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	char *aux=(char*)malloc(10*sizeof(char));
	Player *player = allocPlayer();
	
	fscanf(inputFile, "%s\n%s\n%s\n%d\n", player->name, player->color, player->hat, &(player->numberOfLocations));
	player->locations = (Location*)malloc(sizeof(Location)*player->numberOfLocations);
	for(int i = 0; i < player->numberOfLocations; i++)
	{
		fscanf(inputFile, "(%d,%d) ", &((player->locations[i]).x), &((player->locations[i]).y));
	}
	
	fscanf(inputFile,"%s", aux);
	if(strcmp(aux,"Rotator")==0)
	{
		player->playerRole = Rotator;
		player->ability = Abilities[player->playerRole];
	}
	if(strcmp(aux,"Invertor")==0)
	{
		player->playerRole = Invertor;
		player->ability = Abilities[player->playerRole];
	}
	if(strcmp(aux,"Decoder")==0)
	{
		player->playerRole = Decoder;
		player->ability = Abilities[player->playerRole];
	}
	if(strcmp(aux,"Impostor")==0)
	{
		player->playerRole = Impostor;
		player->ability = Abilities[player->playerRole];
	}
	free(aux);
	return player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{	
	int i;
	Game *game = allocGame();
	fscanf(inputFile ,"%s\n%d\n%d\n", game->name, &(game->killRange), &(game->numberOfCrewmates));
	
	game->crewmates = (Player**)malloc(sizeof(Player*) * game->numberOfCrewmates);
	for(i = 0; i < game->numberOfCrewmates; i++){
		game->crewmates[i] = ReadPlayer(inputFile); 
	}
	game->impostor = ReadPlayer(inputFile);
	return game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{	
	char *role = fromEnumtoString(player->playerRole);
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", player->name, player->color, player->hat, role);
	fprintf(outputFile, "Player's locations: ");
	for(int i = 0; i < player->numberOfLocations; i++)
	{
		fprintf(outputFile,"(%d,%d) ", (player->locations[i]).x, (player->locations[i]).y);
	}
	fprintf(outputFile,"\n");
	free(role);
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile,"Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n", game->numberOfCrewmates);
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\tCrewmates:\n");
	for(int i = 0; i < game->numberOfCrewmates; i++){
	WritePlayer((game->crewmates[i]),outputFile);
	}
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
}
int distance(Location a, Location b)
{
	int distance = abs(a.x-b.x)+abs(a.y-b.y);
	return distance;
}
//Task 7
void *KillPlayer(void *input)
{
	Game *game = (Game*) input;
	Location im_now = (game->impostor)->locations[(game->impostor)->indexOfLocation];
	char *sir = (char*)malloc(MAX_LEN_STR_ATR);
	int in_kill = 0;
	int dist_nou;
	int dist;
	
	while((game->crewmates[in_kill])->alive == 0){
		in_kill++;
	}
	dist = distance(im_now, (game->crewmates[in_kill])->locations[(game->crewmates[in_kill])->indexOfLocation]);
	
	for(int i = in_kill+1; i < game->numberOfCrewmates; i++)
	{
		if((game->crewmates[i])->alive == 1)
		{
			dist_nou = distance(im_now, (game->crewmates[i])->locations[(game->crewmates[i])->indexOfLocation]);
			if(dist_nou <= dist)
			{
				in_kill = i;
				dist = dist_nou;
			}
		}
	}
	
	if(dist <= game->killRange){
		game->crewmates[in_kill]->alive = 0;
		sprintf(sir, "Impostor %s has just killed crewmate %s from distance %d.", game->impostor->name, game->crewmates[in_kill]->name, dist);
	}
	else{
		sprintf(sir, "Impostor %s couldn't kill anybody.", game->impostor->name);
	}
	return sir;

}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix){
	char* output;
	for(int i = 0; i < game->numberOfCrewmates; i++){
		Player *player = game->crewmates[i];
		
		if(player->alive){
			player->indexOfLocation = (player->indexOfLocation+1)%player->numberOfLocations;
			output = (char*)(player->ability(inputMatrix[i]));
			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\nCrewmate %s's output:\n%s\n",  
			 player->name, (player->locations[player->indexOfLocation]).x, (player->locations[player->indexOfLocation]).y, player->name, output);
			free(output);
		}else {
			fprintf(outputFile, "Crewmate %s is dead.\n", player->name);
		}
	}
	
	Player *imp = game ->impostor;
	imp->indexOfLocation = (imp->indexOfLocation+1)%imp->numberOfLocations;
	output = (char*)(imp->ability((void*)game));
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\nImpostor %s's output:\n%s\n",
	 imp->name, (imp->locations[imp->indexOfLocation]).x, (imp->locations[imp->indexOfLocation]).y, imp->name, output);
	 
	free(output);
}


// Task 9
void FreePlayer(Player *player)
{
	free(player->locations);
	free(player->color);
	free(player->name);
	free(player->hat);
	free(player);
}

// Task 9
void FreeGame(Game *game)
{
	free(game->name);
	FreePlayer(game->impostor);
	for(int i = 0; i < game->numberOfCrewmates; i++){
		FreePlayer(game->crewmates[i]);
	}
	free(game->crewmates);
	free(game);
}