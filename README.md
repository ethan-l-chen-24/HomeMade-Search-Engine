# Ethan Chen
## CS50 Fall 2021, Tiny Search Engine

GitHub username: ethan-l-chen-24

To build, run `make`.

To clean up, run `make clean`.

### Lab 4

For this project, I attribute a few sources
    - the course notes, especially for creating and grabbing from archive files
    - StackOverflow and TutorialsPoint for figuring out how to build a string (which, by 
        the way, is way too difficult in C)
    - the Makefiles from previous labs, which as usual, I copied a fair amount from
    - the Pseudocode, which I not only adapted, but modified slightly for my IMPLEMENTATION.md file
    - Huge HUGE shoutout to Kysen for helping me debug an error in my code
    - finally, the support of my friends and family. Thank you mom and dad, even though
        I never even mentioned this project to you, for your unconditional love and
        psychopathic help throughout it all

It is worth noting that
    - I used all camelCase for this code. I know that the other modules that were given are all in snake   
        case, but I can't stand the look of it, so sorry for that inconsistency!
    - I wasn't sure where to put the directory that webpage files are inserted into, so I placed it in
        common, for I assumed that _indexer_ and _query_ are also going to draw from those files, so
        I thought it would make sense for it to be placed in the common section
    - I made all of my * for pointers next to the data type, I just prefer char* string in comparison to something like char *string

### Lab 5
For this project, I attribute a few sources
    - the previously lab, from which I took heavily
    - The comments from my grader on Lab 4, (which hopefully I have fixed)
    - Jack McGary for helping me understand the lab during office hours
    - Professor Prioleau for fixing my compilation error in my makefile
    - finally, the support of my friends and family. Jeff, this one is for you
    - the Lab instructions, from which I copied some text when writing my design spec file

It is worth noting that
    - I changed the output directory of crawler from common to data
    - I rearranged some of the crawler methods and added some new helper methods to the common directory (don't worry, crawler still works)
    - I was not able to fix the problem when it comes to the include statement and absolute paths. It seemed to me that even though there were no visible (red squiggly) errors, it simply would not compile without the absolute filepath
    - I added some of my own methods outisde of the requirements. Namely, I made the intToString() method in word.c and stringBuilder() in pagedir.c, as both methods are used very frequently throughout all modules