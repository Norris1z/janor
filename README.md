========================================================================
    CONSOLE APPLICATION : Janor_2 Project Overview
========================================================================

Janor is a simple file parser written in c++.

it parses files with a .jnr extension and a <?JANOR as the first line in the file.

janor has 3 keywords and one symbol which are:

the '$' which comes before any janor tag

and '//' which starts a janor comment

the keyword 'upper' makes all lines of code until the next '$' uppercase

the keyword 'lower' makes all lines of code until the next '$' lowercase

the keyword 'reverse' makes all lines of code until the next '$' in reverse order


example:

<?JANOR

// This is a janor comment

$upper

the quick brown fox jumped over the lazy dog

$lower 

THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG

$reverse

The Quick Brown Fox Jumped Over The Lazy Dog


The above code when parsed would output:


THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG


the quick brown fox jumped over the lazy dog


god yzal eht revo depmuj xof nworb kciuq eht


HOW TO PARSE A JANOR SCRIPT:


compile and run the janor parser then input a valid directory 


which conatins all your janor scripts and hit enter


then enter your filename including the .jnr extension and hit enter and watch your code compile :)
