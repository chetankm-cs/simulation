main : main.c
	cc -g -Wall main.c make_decision.c -o run
clean:
	rm run 
