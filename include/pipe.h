/* pipe.h: contains all the macros and definition for the pipe type in XINU */

#define MAXPIPES 10
#define PIPE_SIZE 1024

enum pipe_state_t {
	PIPE_FREE,
	PIPE_USED,
	PIPE_CONNECTED,
	PIPE_OTHER,
	PIPE_READ_DISCONNECTED,
	PIPE_WRITE_DISCONNECTED,
	PIPE_DISCONNECTED,


};

struct pipe_t {
	pipid32 pipid;			    // Pipe ID
	enum pipe_state_t state;	// Pipe state defined by the enum
	char	pipe_buffer[PIPE_SIZE];   
	pid32	owner;				
	pid32	reader;				
	pid32	writer;				
	int read_pos;		//head
	int write_pos;		//tail
	sid32   read_sem;
	sid32  write_sem;	


 // LAB2: TODO: Fill the structure with the fields as required.
};

extern struct pipe_t pipe_tables[MAXPIPES];// Table for all pipes


