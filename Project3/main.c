#include "bmp_header.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "jsmn.h"

int lenght(char * argv, char ** fisier) {
	FILE * fr = fopen(argv, "r");
	
	fseek(fr, 0, SEEK_END);
	int leng = ftell(fr);
	fseek(fr, 0, SEEK_SET);
	
	* fisier = (char * ) malloc(leng);
	fread( * fisier, 1, leng, fr);
	
	fclose(fr);
	return leng;
}

int numar(char * fisier, jsmntok_t * tok, int i)
{
	int val = 0;
	for (char * k = fisier + tok[i].start; k < fisier + tok[i].end; k++) {
		val = val * 10 + *k - '0';
	}
	
	return val;
}

void parse(char * fisier, jsmntok_t * tok, int r, bmp_fileheader * head, bmp_infoheader * info, unsigned char *** bitmap)
{
	int j, i;
	for (i = 0; i <= r; i++) {
		long unsigned int lung_tok = tok[i].end - tok[i].start;
		if (memcmp(fisier + tok[i].start, "file_header", lung_tok) == 0) {
			break;
		}
	}
	
	for(j=i; j<r; j++)
	{;
		long unsigned int lung_tok = tok[j].end - tok[j].start;
		if (memcmp(fisier + tok[j].start, "offset", lung_tok) == 0) {
			j++;
			head -> imageDataOffset = numar(fisier,tok,j);
		}
		
		if (memcmp(fisier + tok[j].start, "signature", lung_tok) == 0) {
			j++;
			head -> fileMarker1 = * (fisier + tok[j].start);
			head -> fileMarker2 = * (fisier + tok[j].start + 1);;
		}
		
		if (memcmp(fisier + tok[j].start, "reserved", lung_tok) == 0) {
			j++;
			head -> unused1 = numar(fisier,tok,j);
			head -> unused2 = head -> unused1;
		}
		
		if (memcmp(fisier + tok[j].start, "file_size", lung_tok) == 0) {
			j++;
			head -> bfSize = numar(fisier,tok,j);
		}
		
		if (memcmp(fisier + tok[j].start, "colors_used", lung_tok) == 0) {
			j++;
			info -> biClrUsed = numar(fisier,tok,j);
		}
		
		if (memcmp(fisier + tok[j].start, "size", lung_tok) == 0) {
			j++;
			info -> biSize = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "colors_important", lung_tok) == 0) {
			j++;
			info -> biClrImportant = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "image_size", lung_tok) == 0) {
			j++;
			info -> biSizeImage = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "y_pixels_per_meter", lung_tok) == 0) {
			j++;
			info -> biYPelsPerMeter = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "x_pixels_per_meter", lung_tok) == 0) {
			j++;
			info -> biXPelsPerMeter = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "width", lung_tok) == 0) {
			j++;
			info -> width = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "planes", lung_tok) == 0) {
			j++;
			info -> planes = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "bit_count", lung_tok) == 0) {
			j++;
			info -> bitPix = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "compression", lung_tok) == 0) {
			j++;
			info -> biCompression = numar(fisier,tok,j);
		}
		if (memcmp(fisier + tok[j].start, "height", lung_tok) == 0) {
			j++;
			info -> height = numar(fisier,tok,j);
		}
	}
	
	int len = info->bitPix*info->height*info->width/8;
	*bitmap = malloc(info->height*sizeof(unsigned char*));
	for(i=0; i<info->height; i++)
		(*bitmap)[i] = malloc(len/info->height);
	
	for (i = 0; i <= r; i++) {
		long unsigned int lung_tok = tok[i].end - tok[i].start;
		if (memcmp(fisier + tok[i].start, "0", lung_tok) == 0) {
			break;
		}
	}
	
	for(j=info->height-1; j>=0; j--)
		for(int l=0; l<len/info->height; l++)
				(*bitmap)[j][l] = numar(fisier, tok, i + 3*j*info->height + l);
}

void swap(unsigned char *a, unsigned char *b)
{
	unsigned char temp = *a;
	*a=*b;
	*b=temp;
}

void fix(bmp_fileheader * head, bmp_infoheader * info, unsigned char ** bitmap) {
	
	for(int i=2; i<9*8+2; i+=8)
		for(int j=2; j<9*8+2; j+=8)
			for(int m=0; m<5; m++)
				for(int k=0, l=4; k<l; k++, l--)
					swap(&bitmap[i+m][3*j+3*k+0],&bitmap[i+m][3*j+3*l+0]),
					swap(&bitmap[i+m][3*j+3*k+1],&bitmap[i+m][3*j+3*l+1]),
					swap(&bitmap[i+m][3*j+3*k+2],&bitmap[i+m][3*j+3*l+2]);
			
}

int task3(bmp_fileheader * head, bmp_infoheader * info, unsigned char ** bitmap)
{
	int **tabla = malloc(9*sizeof(int*));
	for(int i=0; i<9; i++)
		tabla[i]=malloc(9*sizeof(int));
		
	for(int i=2; i<9*8+2; i+=8)
	{
		for(int j=2; j<9*8+2; j+=8)
		{
			tabla[i/8][j/8] = 0;
			if(bitmap[i][3*j+1] != 175)tabla[i/8][j/8] = 1;
				else if(bitmap[i][3*j+3+1] != 175)tabla[i/8][j/8] = 4;
					else if(bitmap[i+1][3*j+1] == 175){
						if(bitmap[i+1][3*j+3*4+1] == 175){
							if(bitmap[i+3][3*j+1] == 175)tabla[i/8][j/8] = 8;
								else tabla[i/8][j/8] = 9;
						}else if(bitmap[i+3][3*j+1] == 175)tabla[i/8][j/8] = 6;
							else tabla[i/8][j/8] = 5;
					}else if(bitmap[i+3][3*j+1] == 175)tabla[i/8][j/8] = 2;
						else if(bitmap[i+4][3*j+1] == 175)tabla[i/8][j/8] = 3;
							else tabla[i/8][j/8] = 7;
		}
	}
	
	for(int i=0; i<9; i++){
		int suma = 0;
		for(int j=0; j<9; j++)
			suma = suma+tabla[i][j];
		
		if(suma!=45)
		{
			for(int k=0; k<9; k++)
				free(tabla[k]);
			free(tabla);
			return 0;
		}

	}
	
	for(int k=0; k<9; k++)
		free(tabla[k]);
	free(tabla);
	return 1;
}

int main(int argc, char * argv[]) {
	if(strlen(argv[2]) !=3)return 0;
	
	char * out = malloc(50*sizeof(char));
	char * fisier;
	
	long len = lenght(argv[1], & fisier);
	jsmn_parser p;

	int tok_num = 73 * 73 * 3 + 18 * 2 + 1;
	jsmntok_t * t = (jsmntok_t * ) malloc(tok_num * sizeof(jsmntok_t));

	jsmn_init( & p);
	int r = jsmn_parse( & p, fisier, len, t, tok_num);
	
	char * q = strtok(argv[1], "./");
	q = strtok(NULL, "./");
	q = strtok(NULL, "./");
	
	sprintf(out, "output_task1_%s.bmp", q);
	FILE * output = fopen(out, "wb");
	
	// for( int i = 1; i < r; i++)
		// printf( "%.*s\n", t[i].end - t[i].start,	
						  // fisier + t[i].start);
						  
	bmp_fileheader * header = (bmp_fileheader * ) malloc(sizeof(bmp_fileheader));
	bmp_infoheader * info = (bmp_infoheader * ) malloc(sizeof(bmp_infoheader));
	unsigned char ** bitmap = NULL;
	
	parse(fisier, t, r, header, info, &bitmap);
	
	fwrite(header, sizeof(bmp_fileheader), 1, output);
	fwrite(info, sizeof(bmp_infoheader), 1, output);
	
	for(int i=info->height-1; i>=0; i--)
	{
		fwrite(bitmap[i],1,3*info->width,output);
		putc(0,output);
	}
	
	fclose(output);
	sprintf(out, "output_task2_%s.bmp", q);
	output = fopen(out, "wb");
	
	fix(header,info,bitmap);
	fwrite(header, sizeof(bmp_fileheader), 1, output);
	fwrite(info, sizeof(bmp_infoheader), 1, output);
	
	for(int i=info->height-1; i>=0; i--)
	{
		fwrite(bitmap[i],1,3*info->width,output);
		putc(0,output);
	}
	
	fclose(output);
	sprintf(out, "output_task3_%s.json", q);
	output = fopen(out, "wb");
	
	int ok = task3(header,info,bitmap);

	if(ok)fprintf(output, "{\n\t\"input_file\":\t\"%s.json\",\n\t\"game_state\":\t\"Win!\"\n}\n", q);
		else fprintf(output, "{\n\t\"input_file\":\t\"%s.json\",\n\t\"game_state\":\t\"Loss :(\"\n}\n", q);

	fclose(output);
	free(out);
	for(int i=0; i<info->height; i++)
		free(bitmap[i]);
	free(bitmap);
	free(header);
	free(info);
	free(fisier);
	free(t);

	return 0;
}