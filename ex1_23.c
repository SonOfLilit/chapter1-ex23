/*
* Some general comments
* 
* 1) Always compile with all warnings turned on (-Wall in gcc).
*    That way you'll notice, for example, that:
*       int lastchar, n;
*    are never used.
* 
* 2) Your code fails on itself :-) Can you find the bugs and fix it?
*/

#include <stdio.h>
#include <limits.h>
#define DEFAULT 0
#define DOUBLE_QUOTE 1
#define SINGLE_QUOTE 2
#define MULTILINE_COMMENT 3
#define LINE_COMMENT 4

/* Globals are bad for your health and prevent testability and
 * reusability. Can you refactor the code to eliminate these being globals?
 */
int state;
int c;

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
		// Are you familiar with function lookup tables? They could
		// make this more elegant. But I don't think you learned it yet.
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
	
	// Huh?
	putchar('\n');
}

void handle_default_state() {
	// Is this needed?
	extern int state, c;
	
	// What happens if state is not default?
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
	} else if(c == EOF) { // If this can happen, I think, something's wrong
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
		// Is this needed?
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
		// I really can't figure out what this does
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
	// As I wrote to you in chat, the code would be more elegant if
	// the state machine logic (what state and what input leads to what
	// actions and what state) would be completely separate from the machine
	// running code (get input, dispatch to relevant logic based on state).
	// 
	// Can you replace the need for this function with introducing more states?
	extern int state, c;
	int lastchar = c;
	c = getchar();
	
	if(c == EOF) {
		c = lastchar;
		handle_default_state(); // ummm... this kinda scared me for a moment
	} else if(c == '*') { // If we have /* , this is a MULTILINE_COMMENT
		state = MULTILINE_COMMENT;
		//c = getchar(); // fetch the next character
		//handle_default_state();
	} else if(c == '/') {
		state = LINE_COMMENT;
	}
}
