# KD Tree

## What's it used for?

So, k-d trees, at the first look, may appear to be more theoretical than practical in nature. But that's really not the case.

k-d trees hold a variety of important applications, some of which include :

- 1. Nearest Neighbor Search
- 2. Database queries involving a multidimensional search key

A query asking for all the employees in the age-group of (40, 50) and earning a salary in the range of (15000, 20000) per month can be transformed into a geometrical problem where the age is plotted along the x-axis and the salary is plotted along the y-axis [4]
4] The x-axis denotes the age of the employee in years, and the y-axis denotes the monthly salary in thousand rupees.


A 2 dimensional k-d tree on the composite index of (age, salary) could help you efficiently search for all the employees that fall in the rectangular region of space defined by the query described above.


- 3. n-body Problem [5]

> How can we efficiently simulate the movements of a collection of objects moving under mutual gravational attraction?

- 4. Color Reduction [7]


What is an intelligent way to pick 256 colors to represent a full color image?


The naive method could be to pick up the colors which are used most often.

A more efficient method, however, could represent colors in terms of their RGB values and construct a 3 dimensional k-d tree in order to divide the space containing all the colors of the image. The construction of the k-d tree would stop when the count of the leaf nodes becomes equal to 256. The average of the RGB value of each of the 256 partitions could then be used to get a 256 color palette for the full color image.

- http://groups.csail.mit.edu/graphics/classes/6.838/S98/meetings/m13/kd.html





