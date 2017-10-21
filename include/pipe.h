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


};

struct pipe_t {
	pipid32 pipid;			    // Pipe ID
	enum pipe_state_t state;	// Pipe state defined by the enum
	char	pipe_buffer[PIPE_SIZE];   
	pid32	owner;				
	pid32	reader;				
	pid32	writer;
	int read_pos;
	int write_pos;
	sid32   read_sem;
	sid32  write_sem;	


 // LAB2: TODO: Fill the structure with the fields as required.
};

extern struct pipe_t pipe_tables[MAXPIPES];// Table for all pipes

/*
pipid32 did32_to_pipid32(did32 devpipe) {
    ASSERT(devpipe >= PIPELINE0 && devpipe <= PIPELINE9);
    return devpipe - PIPELINE0;
}

did32 pipid32_to_did32(pipid32 pip) {
    ASSERT(pip >= 0 && pip <= 9);
    return PIPELINE0 + pip;
}

*/
