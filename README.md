# command-line-shell
The purpose of this project was to write a small command line shell using C. 

## Date Completed
Fall 2013 - University of South Florida

## Supported Features
### Shell  Commands
    - cd <directory> - change the current default directory to <directory>.
    *If the <directory> argument is not present, report the current directory. If the directory does
    not exist an appropriate error should be reported. This command should also change the
    PWD environment variable.*
    - clr - clear the screen.
    - dir <directory> - list the contents of directory <directory>
    - environ - list all the environment strings
    - echo <comment> - display <comment> on the display followed by a new line (multiple spaces/tabs may be reduced to a single space)
    - help - display the user manual using the more filter
    - pause - pause operation of the shell until 'Enter' is pressed
    - quit - quit the shell

### Supports the use of internal commands, `<ssh>`, `<file>`, ...
### Supports background and forground processes:
    - Background process note by &
    - Forground process run as default
  
### Supports I/O redirection:
	  - file output, command > filename, example, pwd > output.txt
	  - file input, command < filename, example, wc -l < batchfile

## Author
* **Senzinna** - *Initial work* - [command-line-shell](https://github.com/senzinna/command-line-shell)
