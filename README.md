# Long-Number-Adder
Adds numbers that are too large to fit into the `long` data type.

Built in C. Later, I may write this program in other languages like Java and C# and add them to this repo.

Currently only works for positive Base10 numbers, using C strings.

Program can be optimized by only dealing with `char` values and not converting between characters (ASCII values) and their decimal representations. E.g. `'9'`(char) versus `9`(int).
