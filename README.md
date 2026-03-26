# WeeklyProgrammingAssignment

[![C++ DocTest](https://github.com/PaulTyree/WeeklyProgrammingAssignment/actions/workflows/BadgeTest.yml/badge.svg)](https://github.com/PaulTyree/WeeklyProgrammingAssignment/actions/workflows/BadgeTest.yml)

This program takes in information about a user's birding trip(s) and neatly organizes the data while calculating the average amount of birds seen per hour.

Week 6 is focused on operater overloading as well as templates.

I have included several overloaded operators such as:

operator==
  
operator<<
  
operator[]
  
operator=+
  
operator=-

The use of this was added

Lastly, two templates were added, a function template and a class template.

Week 7 changes are focused on exceptions and exception classes.

Exception classes are formatted for my dynamic array and my class managers in order to make sure when removing trips only actual array options are affected

Try, Throw, and Catch statements are used throughout my code now in order to guarantee that the items are used well and meaningfully.

This demonstrates my understanding of the use of try, throw, catch, and exception statements and usages.

Week 8 is made to create a new recursive function within our program

This is done in the Manager template class, using the function countTripsRecursive

This function is used to print the number of trips a user has taken in an instance of the program

It is used after the user has added all of their trips as well as deleted all of their trips

Week 9 focuses on changing our items array into a vector as well as sorting the vector in different ways

items.size() is used for a dealing with the dynamic resizing of our vector to add and remove trips

A sequential search is used in order to find a specific location within the trips

A bubble sort is then used to sort trips based on the amount of birds that were seen from least amount seen to most amount seen

Finally, a binary search is used on the vector that was just sorted using the bubble method in order to find the index location of a trip that has "x" amount of birds.

Week 10 is all about experimenting with linked lists in order to hone our knowledge on pointers.

Our vector class has changed to be a linked list and we implement data into both front and back. We are also able to search, print, and delete data all within the list as well.
