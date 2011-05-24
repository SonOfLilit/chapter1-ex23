#include <stdio.h>
#include <limits.h>
#define DEFAULT 0
#define DOUBLE_QUOTE 1
#define SINGLE_QUOTE 2
#define MULTILINE_COMMENT 3
#define LINE_COMMENT 4

int state; // the curren state we are in
int c; // the current character we have

void handle_default_state();
void handle_single_quote_state();
void handle_double_quote_state();
void handle_multiline_comment_state();
void handle_line_comment_state();
void handle_dash();
void handle_quoted_state(char id);


main()
{
	int lastchar, n;
	extern int c, state;
	state = DEFAULT;
	while((c = getchar()) != EOF) {
		switch(state)
		{
			case DEFAULT:
				handle_default_state();
				break;
			case DOUBLE_QUOTE:
				handle_double_quote_state();
				break;
			case SINGLE_QUOTE:
				handle_single_quote_state();
				break;
			case MULTILINE_COMMENT:
				handle_multiline_comment_state();
				break;
			case LINE_COMMENT:
				handle_line_comment_state();
				break;
			default:
				break;
		}
	}
	
	putchar('\n');
}

void handle_default_state() {
	extern int state, c;
	
	if(c == '\'' && state == DEFAULT) {
		state = SINGLE_QUOTE;
		putchar(c);
	} 
	
	if(c == '"' && state == DEFAULT) {
		state = DOUBLE_QUOTE;
		putchar(c);		
	} else if(c == '*') {
		state = MULTILINE_COMMENT;
	} else if(c == '/') {
		handle_dash();
	} else if(c == EOF) {
		return;
	} else {
		putchar(c);
	}
}

void handle_line_comment_state() {
	extern int state, c;
	if(c == '\n') {
		state = DEFAULT;
		putchar(c);
	}
}

void handle_multiline_comment_state() {
	extern int state, c;
	
	int lastchar = c;
	c = getchar();
	if(c == EOF) {
		c = lastchar;
		state = DEFAULT;
	} else if(c == '/') {
		state = DEFAULT;
		c = getchar();
		handle_default_state();
	}
}

void handle_double_quote_state() {
	extern int state;
	handle_quoted_state('"');	
	putchar(c);
}

void handle_quoted_state(char id) {
	extern int state, c;
	
	if(c == '\\') {
		int lastchar = c;
		putchar(lastchar);
		c = getchar();
		if(c == EOF) return;
		if(c == id) state = DOUBLE_QUOTE;
	} else if(c == id) {
		state = DEFAULT;
	}	
} 

void handle_single_quote_state() {
	extern int state, c;
	handle_quoted_state('\'');
	putchar(c);
}

void handle_dash() {
	extern int state, c;
	int lastchar = c;
	c = getchar();
	
	if(c == EOF) {
		c = lastchar;
		handle_default_state();
	} else if(c == '*') { // If we have /* , this is a MULTILINE_COMMENT
		state = MULTILINE_COMMENT;
		//c = getchar(); // fetch the next character
		//handle_default_state();
	} else if(c == '/') {
		state = LINE_COMMENT;
	}
}

