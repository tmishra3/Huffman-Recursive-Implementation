## Stage one (compression of input text file with -c option):
 
 In stage one a string of chars from an input file are read and it
uses a series of functions to compress it using the huffman compression. This mp
utilizes several variables and functions.There is a  structure defined
known as the HTREE. The HTREE contains the letter and the weight of each letter.
In addition, there are the child node pointers that go to the left and right.
Finally, it contains a  pointer to the next tree in the sequence.
The program starts out by opening the file specified by the user and reading
each letter and copying it to the array inputText. At the end of copying,
the file is closed. Then, a loop is run from the beginning of the inputText
array to end of the inputText array. For each char, a piece of memory is
allocated using malloc for an HTREE data type. If for some reason malloc
returns a null, the program is aborted due to lack of memory. Each time the
memory is allocated for a new HTREE, the letter is set to the char read and
the weight is set to 1 by default. The three pointers are all set to NULL. Then
the appendForest function is called. This function will read the char and
will appropriately go to the correct HTREE that contains that char and
if found in the forest tree, increments its weight or appends a new
tree in the initial forest. This function uses the function searchForestTree
to search for the letter in forest trees. If it is found, the pointer to that
tree is returned and is used to increment its weight. Once all the characters
have been read from the input file, a linked list of the HTREEs has been
created. For example, with the input chars being "aaaabbcd," a chain of linked
lists are created with the first being an HTREE with letter a and weight 4 with
the next pointer pointing to an HTREE with letter b and weight 2 etc. Once initial
forest tree is created from input text file, serialized function is called to
create a string with letter and weight. For example for the input file with text
as "aaaabbcd", serialize function will create a string "a;4;b;2:c;1:d;1" in
variable weightArr.  This letter;weight string is later printed as fisrt line
in outfile and is terminated by new line character ("\n"). Next, the huffmanize
function is called to create a huffman tree. To create a huffman tree, the
forest tree is searched to find the last smallest and small trees nodes
based on the weight of tree node. A new tree is created by assigning the smallest
tree node set to the left pointer and small tree node set to right pointer. The
weight of new tree node is the sum of smallest and small tree. The letter is set
to null.This process is repeated recursively until no node is left in the forest
tree. Once the huffman tree is complete, the output file is opened for write.
The string created by serialized function ( ex.weightArray = "a;4;b;2:c;1:d;1") is
written to output file and terminated with new line character. The huffman code
of each letter is determined by traversing the huffman tree. The huffman code
of each letter is stored in an array and is written to output file. The output file is
created as a text file with the same name as the input file with a .hc
extension.

## Stage 2(Decompression of compressed file with -d option):

In stage 2, the input compressed file with file extension .hc is opened for reading.
The first line of the compressed input file is the weight/letter string created in checkpoint
one. For example, this string will be "a;4:b;2:c;1:d;1".  This string is saved into a variable
and used  by the unserialzeTree function to create a initial forest tree. This initial forest
tree is exactly the same as created in stage one. The Second line of compressed input file is
the huffman code of the input text file of stage one. This is also saved into an array and will
be used for the decompressing of input compressed files. After creating an initial forest tree
by using the unserializeTree function, the huffmanize function is called to create huffman a tree
which is exactly the same as in stage one. Once a huffman tree is created, the output file is
opened for writing. This output file will have a .dc file extension. The huffman code which was
saved into an array is traversed in the loop to decide upon the corresponding letter by searching
through the huffman tree. Once the letter is found in a leaf node, it is written to the output
file. After decoding all huffman codes, the output file with file extension .dc is closed. This
output file should be exactly the same as the input text file of checkpoint one.
