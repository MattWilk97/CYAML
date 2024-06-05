#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <stdbool.h>

void process_maps_in_map(FILE **yaml_file, char *line, int chars) {
	int c, fix_size, key_i = 0, indents = 0, value_size = 0, key_size = 0, char_c = chars;
	char *key = malloc((char_c)*sizeof(char)), *value = malloc((char_c)*sizeof(char));
	char *sub_line = malloc((char_c)*sizeof(char)), *cpy;
	strncpy(sub_line, line, char_c);

	key_size = char_c - 2;
	key = realloc(key, key_size*sizeof(char));
	strncpy(key, sub_line, key_size);
	key[key_size - 1] = '\0';
	//key_i = char_c - 2;
				
	printf("owner: %s\n", key);

	//while((c = fgetc(*yaml_file)) != '}') {
		
	while((c = fgetc(*yaml_file)) != '}') {
		char_c++;
		sub_line = realloc(sub_line, char_c*sizeof(char));
		sub_line[char_c - 1] = (char)c;
					
		if(sub_line[char_c - 2] == '{' && isalnum(c) != 0) {
			char_c = 1;
			sub_line = realloc(sub_line, char_c*sizeof(char));
			sub_line[char_c - 1] = (char)c;
		}

		switch(c) {
			case '{':
				process_maps_in_map(yaml_file, sub_line, char_c);
				break;
			case '\n':
				if(sub_line[char_c - 3] == '{') {
					sub_line[char_c - 1] = '\0';
					free(sub_line);
					sub_line = NULL;
					free(key);
					key = NULL;
					free(value);
					value = NULL;
					char_c = 0;
					indents = 0;
				} 
				break;
			case ':':
				//printf("line: %s ", sub_line);
				//process_mapping(yaml_file, sub_line, char_c, indents);

				key_size = char_c;
				key = realloc(key, key_size*sizeof(char));
				strncpy(key, sub_line, key_size);
				key[key_size - 1] = '\0';

				printf("Key: %s ", key);
				
				//char_c = 0;
				do {
				//while((c = fgetc(*yaml_file)) != '\n')	{
					c = fgetc(*yaml_file);
					char_c++;
					sub_line = realloc(sub_line, char_c*sizeof(char));
					sub_line[char_c - 1] = (char)c;
					
					switch(c) {
						case '{':
							c = '\n';
							//process_maps_in_map(yaml_file, sub_line, char_c);
							break;
						case '#':
							while ((c = fgetc(*yaml_file)) != '\n'); 

							sub_line[char_c - 1] = '\0';

							break;
						case ',':
							value_size = char_c - (key_size + 1);
							value = realloc(value, value_size*sizeof(char));
				 
							strncpy(value, sub_line + (key_size + 1), value_size);
							value[value_size - 1] = '\0';

							c = fgetc(*yaml_file);
							switch(c){
								case '\n':
									break;
								case ' ':
									c = fgetc(*yaml_file);
									switch(c) {
										case '\n':
											break;
										case '#':
											while ((c = fgetc(*yaml_file)) != '\n'); 

											sub_line[char_c - 1] = '\0';
	
											break;
										default:
											//char_c++;
											//sub_line[char_c - 1] = (char)c;
											c = '\n';
											break;
									}
									break;
							}
							break;
						case '}':
							if(isalnum(sub_line[char_c - 2]) != 0) {
								value_size = char_c - (key_size + 1);
								value = realloc(value, value_size*sizeof(char));
					 
								strncpy(value, sub_line + (key_size + 1), value_size);
								value[value_size - 1] = '\0';
							}
							break;
					}
				} while(c != '\n');

				if(sub_line[chars - 1] != ',') {
					value_size = char_c - (key_size + 1);
					value = realloc(value, value_size*sizeof(char));
				 
					strncpy(value, sub_line + (key_size + 1), value_size);
					value[value_size - 1] = '\0';
				}
				//value[value_size - 1] = '\0';
				printf("Value: %s\n", value);

				free(sub_line);
				sub_line = NULL;
				free(key);
				key = NULL;
				free(value);
				value = NULL;
				char_c = 0;
				indents = 0;
				//key_i = char_c + 2;
				break;/*
			case ',':
				value_size = char_c - (key_size + 1);
				value = realloc(value, value_size*sizeof(char));
				 
				strncpy(value, sub_line + (key_size + 1), value_size);
				value[value_size - 1] = '\0';
				
				
				printf("Key: %s Value: %s \n", key, value);
				//sub_line[char_c - 1] = '\0';
				//printf("comma_ %s\n", key);
				
				//printf("before free");
				free(sub_line);
				sub_line = NULL;
				char_c = 0;
				//printf("Here");
				break;*/
			case '#':
				while ((c = fgetc(*yaml_file)) != '\n'); 

				sub_line[char_c - 1] = '\0';

				free(sub_line);
				sub_line = NULL;
				free(key);
				key = NULL;
				free(value);
				value = NULL;
				char_c = 0;
				//key_i = 0;
				indents = 0;
				
				//c = -1;
				break;
			case ' ':
				if(char_c == 1) {
					fix_size = char_c - 1;
					cpy = malloc(fix_size*sizeof(char));
					strncpy(cpy, sub_line + 1, fix_size);
					cpy[fix_size - 1] = '\0';

					free(sub_line);
					sub_line = NULL;
					sub_line = malloc(fix_size*sizeof(char));
								
					strncpy(sub_line, cpy, fix_size);

					free(cpy);
					cpy = NULL;

					indents++;
					while ((c = fgetc(*yaml_file)) == ' ') {
						indents++;
					}

					char_c = fix_size;

					char_c++;
					sub_line[char_c - 1] = (char)c;
				}
				break;
		}
	}
}

void process_mapping(FILE **yaml_file, char *line, int chars, int line_indent) {
	int c, fix_size, key_i = 0, indents = 0, value_size = 0, key_size = 0, char_c = chars;
	char *key = malloc((char_c)*sizeof(char)), *value = malloc((char_c)*sizeof(char));
	char *sub_line = malloc((char_c)*sizeof(char)), *cpy;
	strncpy(sub_line, line, char_c);
	
	//Initial owner
	//int key_size = char_c;
	//key = realloc(key, key_size*sizeof(char));
	//strncpy(key, *line, key_size);
	//key[key_size - 1] = '\0';
	//key_i = *chars;
	
	while((c = fgetc(*yaml_file)) != '\n'){
		//c = fgetc(*yaml_file);
		//if(c != ' ') {
		char_c++;
		sub_line = realloc(sub_line, char_c*sizeof(char));
		sub_line[char_c - 1] = (char)c;
		//}
		
		switch(c) {
			//case ',':
				//printf("%s\n", sub_line);
			//	free(sub_line);
			//	sub_line = NULL;
			//	char_c = 0;
				//value_size = (*chars) - key_i - 1;
				//value = realloc(value, value_size*sizeof(char));
				//strncpy(value, (*line) + (key_i + 1), value_size);
				//value[value_size - 1] = '\0';
				//printf("Key: %s Value: %s \n", key, value);
			//	break;
			//case ':':
			//	break;
			case '{':
				process_maps_in_map(yaml_file, sub_line, char_c);
				break;
			/*case '#':
				while ((c = fgetc(*yaml_file)) != '\n'); 
				
				sub_line[char_c - 1] = '\0';

				free(sub_line);
				sub_line = NULL;
				free(key);
				key = NULL;
				free(value);
				value = NULL;
				char_c = 0;
				key_i = 0;
				indents = 0;
				
				//c = -1;
				break;*/
			case ' ':
				if(char_c == 1) {
					fix_size = char_c - 1;
					cpy = malloc(fix_size*sizeof(char));
					strncpy(cpy, sub_line + 1, fix_size);
					cpy[fix_size - 1] = '\0';

					free(sub_line);
					sub_line = NULL;
					sub_line = malloc(fix_size*sizeof(char));
								
					strncpy(sub_line, cpy, fix_size);

					free(cpy);
					cpy = NULL;

					indents++;
					while ((c = fgetc(*yaml_file)) == ' ') {
						indents++;
					}

					char_c = fix_size;

					char_c++;
					sub_line[char_c - 1] = (char)c;
				}
				break;
		}
	}

	free(sub_line);
	sub_line = NULL;
	free(key);
	key = NULL;
	free(value);
	value = NULL;
	char_c = 0;
	key_i = 0;
	indents = 0;
}

int main(int argc, char* argv[]) {

	const char* file_name = "config.yaml";
	int is_ok = EXIT_FAILURE;

	FILE* yaml_file = fopen(file_name, "r");
	if(!yaml_file) {
		perror("File opening failed");
		return is_ok;
	}

	int c, chars = 0;
	char *line = malloc(chars*sizeof(char));
	
	int dashes = 0, comment_i = 0, line_indent = 0, val_chars = 0;
	while ((c = fgetc(yaml_file)) != EOF) {
		chars++;
		line = realloc(line, chars*sizeof(char));
		line[chars - 1] = (char)c;

		switch(c) {
			case ':':
				process_mapping(&yaml_file, line, chars, line_indent);
				break;
			case '\n':
				free(line);
				line = NULL;
				//free(key);
				//key = NULL;
				line_indent = 0;
				chars = 0;
				break;
		/*
			case '-':
				//dashes++;
				//sequence = true;
				c = fgetc(yaml_file);
				
				chars++;
				line = realloc(line, chars*sizeof(char));
				line[chars - 1] = (char)c;
				
				//if(c == '-')
					//sequence = false;
					//dashes++;

				c = fgetc(yaml_file);
				
				chars++;
				line = realloc(line, chars*sizeof(char));
				line[chars - 1] = (char)c;

				if(c == '-')
					structure = true;
				else if(c == '\n') {
					//seq_map_s = true;
					line[chars] = '\0';

					//push(&head, line, chars, line_indent, mapping, sequence, structure, comment, comment_i, map_s, map_e, sequence_s, sequence_e, seq_map_s);
				
					//if(head->seq_map_s)
						//printf(" Start of a sequence of mappings ");

					//printf("Indents %d, %s", head->indent, head->line_text);
				
					//mapping = false;
					//sequence = false;
					//structure = false;
					//comment = false;
					//map_s = false;
					//map_e = false;
					//sequence_s = false;
					//sequence_e = false;
					//seq_map_s = false;
					//line_indent = 0;
					//comment_i = 0;
					//dashes = 0;
					chars = 0;
					free(line);
					line = NULL;
					//val_chars = 0;
					free(value);
					value = NULL;
					free(key);
					key = NULL;
				}
					
				break;*/
			case '#':
				while ((c = fgetc(yaml_file)) != '\n');
				
				free(line);
				line = NULL;
				line_indent = 0;
				chars = 0;
				break;
			case ' ':
				if(chars == 1) {
					line_indent++;
					while ((c = fgetc(yaml_file)) == ' ') {
						line_indent++;
					}
					line[chars - 1] = (char)c;
				}
				break;
		}
	}

	if(ferror(yaml_file))
		puts("I/O error when reading");
	else if (feof(yaml_file)) {
		puts("End of file is reached successfully");
		is_ok = EXIT_SUCCESS;
	}

	fclose(yaml_file);
	return is_ok;
}
