This is a Unix shell, I have created for my CPSC 351 Operating Systems class, it has so many features

1) parses commands and can echo them back as a series of words, one word per line, if the last word on the line is ECHO 
(2) can print SPACE for each space char it finds in the input 

(3) can print PIPE for each pipe it finds in the input 

(4) can run built-in commands:  help, cd, mkdir, exit, !!   

(5) can fork/exec one command without command line redirection:  

(6) can fork/exec one command WITH command line redirection:  

(7) can fork/exec two programs with pipes between them, (A | B) 
