# Systems Project - MyShell

Collaborators: Matthew Cheng, Alvin Yao Yang Cheng, Owen Zeng

Documentation of Features:

-✅cd: Our shell successfully implements cd. It allows users to cd between directories, whether by using .. to go backwards or specifying a directory to go into.

-✅exit: Our shell successfully implements exiting. When using the command exit, the shell exits.

-✅printPath: this is a helper function that prints the path. It works for the purposes of displaying the path.

-✅piping: Our shell implements simple piping. It works with one pipe at a time. On either side of the pipe, larger commands can be run. Example: cat < input.txt | ... > output.txt

-✅redirectR: this redirection does work, it sends the output to the designated file.

-✅redirectL: this redirection does work as well, it redirects stdin from a file

-✅the path: the path is working. On every prompt, the path is displayed correctly to tell the user where they currently are.

-✅the prompt: the prompt is working correctly. With every prompt, the correct path is displayed.


Functions:
-void parse_args( char * line, char ** arg_ary, char * s );

-void printPath();

-void exitShell();

-void cd(char directory[], int shouldPrint);

-void redirectR(char* cmd, char* output, char** args);

-void redirectL(char* cmd, char* input, char** args);

-void pipeP(char* cmd1, char* cmd2);

-int err();
