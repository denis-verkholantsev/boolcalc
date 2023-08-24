# Boolean calculator
Variables xN, where N is a natural number (that is, the variables x1, x2, x3, ...). Can take the values 0 and 1 (not zero).  
Operations:  
- `&` and (conjunction)  
- `v` or (disjunction)  
- `~` not (negation)
- `>` imply (x → y)  
- `<` inverse imply (x ← y)  
- `+` xor
- `=` equivalence  
- `|` nor  
- `^` nand
## How to use
 - The 'boolcalc -calc input output' call computes expressions written in the 'input' file (it requests the values of variables from the user) and writes the results to the `output` file (for each function, the 
reult is written in a separate line).  
- Calling 'boolcalc -cnf input output' builds a conjunctive normal form for expressions written in the 'input' file and writes the results to the 'output' file (for each function, the result is written in a separate line).  
- Calling 'boolcalc -dnf input output' builds a disjunctive normal form for expressions written in the 'input' file and writes the results to the 'output' file (for each function, the result is written in a separate line).  
- Calling 'boolcalc -zh input output' builds the Zhegalkin polynomial for expressions written in the 'input' file and writes the results to the 'output' file (for each function, the result is written in a separate line).  
- The 'boolcalc -table input' call outputs a truth table to the console for all expressions written in the 'input' file.  
- The 'boolcalc -isfull input output` call checks the function system for completeness. If the system of functions in the 'input' file is complete, then 'yes' is output to the 'output' file, otherwise 'no'.  
- Calling 'boolcalc -h' or 'boolcalc ?' displays help for using the program.  
