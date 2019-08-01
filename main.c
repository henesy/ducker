#include <u.h>
#include <libc.h>

// Debug output
int		chatty = 0;

// Read rfork(2)
// TODO - function for printing in plaintext which rfork(2) flags are being used
// 	by using a table and the & operator in a loop
int		rforkflags = RFPROC | RFNOWAIT | RFNAMEG | RFNOMNT | RFCENVG | RFFDG | RFREND;

// Command to run
char	*cmd	= nil;
char	**cargs	= nil;

void
usage(void)
{
	fprint(2, "usage: %s [-D] [-p proto | -a arch] [-r root] [-c cfg] [-u user] [cmd args…]\n", argv0);
	exits("usage");
}

void	parsecfg(char*);
void	parseproto(char*);
void	parsearch(char*);
void	doas(char*);

// It's not docker ­ run a process in a pre-defined sandbox
void
main(int argc, char *argv[])
{
	// TODO ­ maybe use ns(1) files instead of proto?
	char	*protof	= nil;	// Proto file, if any
	char	*rootf	= nil;	// Root directory for proto, default to /
	char	*archf	= nil;	// Archfs archive file, if any
	char	*cfgf	= nil;	// Cfg ndb(2) file, if any
	char	*user	= nil;	// User to change to, if any

	ARGBEGIN {
	case 'D':
		chatty++;
		break;
	
	case 'p':
		protof = EARGF(usage());
		break;
		
	case 'r':
		rootf = EARGF(usage());
		break;
		
	case 'a':
		archf = EARGF(usage());
		break;
		
	case 'c':
		cfgf = EARGF(usage());
		break;
	
	case 'u':
		user = EARGF(usage());
		break;

	} ARGEND
	
	// If no cmd and no cfg, don't know what to run
	if(cfgf == nil && argc <= 0)
		usage();
	
	// Can't use both proto and arch
	if(protof != nil && archf != nil)
		usage();
	
	/* Set up environment */
	
	// Parse cfg, or expect argument for cmd -- argument takes precedence
	if(argc > 0){
		cmd = argv[0];
		cargs = argv;
	}else if(cfgf != nil)
		parsecfg(cfgf);
	else
		usage();
	
	// Parse proto or arch
	if(protof != nil)
		parseproto(protof);
	
	if(archf != nil)
		parsearch(archf);
	
	// Default rootf to /
	if((protof != nil || archf != nil) && rootf == nil) {
		rootf = calloc(2, sizeof(char));
		strncpy(rootf, "/", 2);
		rootf[1] = '\0';
	}
	
	/* Main logic */
	
	if(chatty){
		fprint(2, "Rfork flags = %x\n", rforkflags);
		fprint(2, "Child\n\tcmd = %s\n\targc = %d\n", cmd, argc);
	}
	
	// Change user
	// TODO ­ might not be the right place for this in the logic
	// 	maybe after rfork, but before exec?
	if(user != nil)
		doas(user);

	int pid = rfork(rforkflags);
	
	if(pid < 0)
		sysfatal("err: rfork failed - %r");
	
	if(pid == 0){
		// Child
		exec(cmd, cargs);
		sysfatal("err: exec failed, did you use a full path? - %r");
	}
	
	/* Clean up */
	// TODO - this might be bad ☺ -- maybe free rootf with a flag test for if alloc'd?
	if(rootf != nil)
		free(rootf);
	
	if(cfgf != nil)
		free(cfgf);
	
	if(archf != nil)
		free(archf);
	
	if(protof != nil)
		free(protof);

	exits(nil);
}

// Parse ndb(2) cfg file
void
parsecfg(char *fname)
{
	int fd = open(fname, OREAD);
	
	if(fd < 0)
		sysfatal("err: could not open cfg file ­ %r");
	
	// TODO - parse cfg
	sysfatal("not implemented");
}

// Parse proto(2) fs definition file
void
parseproto(char *fname)
{
	int fd = open(fname, OREAD);
	
	if(fd < 0)
		sysfatal("err: could not open proto file ­ %r");
	
	// TODO - parse cfg for state and bind onto root
	sysfatal("not implemented");
}

// Parse mkfs(8) archive file & mount with archfs()
void
parsearch(char *fname)
{
	int fd = open(fname, OREAD);
	
	if(fd < 0)
		sysfatal("err: could not open arch file ­ %r");
	
	// TODO - mount mkfs archive and mount onto root
	sysfatal("not implemented");
}

// Use cap(3) to change to user -- stretch goal, use auth/as for now
void
doas(char *user)
{
	int fd = open("#¤", OREAD);
	
	if(fd < 0)
		sysfatal("err: could not open #¤ ­ %r");
	
	// TODO - change to user using cap(3); need capability string and hash
	sysfatal("not implemented");
}
