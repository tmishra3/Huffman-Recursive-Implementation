/*
 * Name:
 * Netid:
 *
 * Intro paragraph: This mp  consist of two  stage program. First stage is for
 compression of input file. Compressed file is created with -c option at command line.
 The name of output file from stage one is  <inputFile>.hc.
 Second stage is for decompression of input file created in stage one.  Decompressed
 file is created with -d option at command line. The output file name from stage
 two is <inputFile>.dc.

 Stage one (compression of input text file with -c option):

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

Stage 2(Decompression of compressed file with -d option):

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
 *
 *
 *
 *
 *
 *
 */

/*
 * Header file declarations.
 * You may not include any additional header files
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE     20000        /* max input string length */
#define SYMSIZE        sizeof(char)    /* max symbol length */
#define MAXWEIGHTCHAR    5        /* num chars needed to display a
                       character appearing BUFSIZE times*/
/**
 * Huffman forest data structure: it is a list of binary trees.
 */
typedef struct htree_t {
    char letter;        /* letter of the tree */
    int weight;        /* weight of the tree */
    struct htree_t* left;    /* pointer to left subtree */
    struct htree_t* right;    /* pointer to right subtree */
    struct htree_t* next;    /* pointer to next tree in the forest */
} HTREE;



void traverseHuffmanTree (HTREE *r, int level, char path[], char *buffer[]);
/*global variable*/
HTREE * prevNode = NULL; /*used by cutForest function*/
HTREE * origNode = NULL; /* used for maintaining first element of tree*/
int rootNodeFound = 0; /*Flag for knowing that root node created in huffman tree*/
HTREE * curNode = NULL; /*used in cutForest function*/

int huffmanTreeTraversed = 0; /*flag to know if huffman code created*/
int count = 0; /* used in traversing tree*/



/**
 * MILESTONE 1: MUST USE RECURSION
 * search a list of Huffman trees hf for a given key
 * This need not search any subtree (don't look down
 * the trees specified by left and right) as it is only used
 * to search the initial forest.
 *
 * hf is a pointer to a Huffman tree
 * key is the char letter being searched for in the list
 */
HTREE* searchForestLetter(HTREE* hf, char key)
{
    HTREE *curNode;
    curNode = hf;
   
    while(curNode)
    {
       if (curNode->letter == key) /*check if the passed character matches with current node's letter*/
       {
          hf = curNode;
          return curNode;
       }
   
       curNode = searchForestLetter(curNode->next, key);
   
    }
    return curNode;
}



/**
 * MILESTONE 1: MUST USE RECURSION
 * searchForestMinHelp is a helper function used
 * to find the minimum weight tree minTree in a Huffman forest
 * hf.  Note that you only need to search the list.
 */
HTREE* searchForestMinHelp(HTREE* hf, HTREE* minTree)
{
    HTREE* curNode;
   
    /*if hf is null then return minTree*/
    /*It could be the situation when the first tree is the only tree left in forest*/
    /*As per the example the node a is the only node left whose next is null*/

    if (hf == NULL)
    {
        return minTree;
    }
   
   
    /*curNode = searchForestMinHelp(hf->next, minTree);*/
   
   
       if ((hf->weight <= minTree->weight)) /*check if hf is not null and less than minTree*/
       {
          minTree = hf;
         
         
       }
      
       curNode = searchForestMinHelp(hf->next, minTree);
}

/**
 * MILESTONE 1: MUST USE RECURSION
 * searchForestMin finds the minimum element in a Huffman
 * forest hf, it does this by making a call to a helper function
 * searchForestMinHelp which takes in the remainder of the
 * Huffman forest to be search, with minTree instantiated
 * as the first element of the forest.
 */
HTREE* searchForestMin(HTREE* hf)
{
   
    if (hf == NULL)
    {
        return NULL;
    }
    else
    {/* initialize minNode to first non-empty element of the forest*/
        return searchForestMinHelp(hf->next, hf);

    }
}

/**
 * MILESTONE 1: MUST USE RECURSION
 * cutForest takes a Huffman forest and cuts a given
 * tree out of the forest.  Note that this only needs
 * to consider the list of Huffman trees as did
 * searchForestLetter and searchForestMin.
 *
 * hf is the forest of Huffman trees to search
 * cutNode is the node to cut from the forest
 */
HTREE* cutForest(HTREE* hf, HTREE* cutNode)
{
    HTREE* retTree;
    /*HTREE* curNode;*/

    if (prevNode == NULL) /*if for the first time - set prevNode = null in huffamize function*/
    {
        prevNode = hf;
        origNode = hf;

        /*if cut node is same as first node in forest, then no need to loop*/
        /*disconnect the cut node from forest and set the next node as first node of forest*/
       
        if ((((hf->letter == cutNode->letter) &&  /*if node has a  valid letter*/
            (hf->weight == cutNode->weight) &&
            (hf->letter > 0))) ||

            (((hf->letter == cutNode->letter) && /*if node is a composite node of two valid letters then they will have left and right letter*/
            (hf->weight == cutNode->weight) &&
            (hf->letter == 0) &&
            (hf->left->letter == cutNode->left->letter) &&
            (hf->right->letter == cutNode->right->letter))))

        {
            /*set first node as next node of first tree*/
            curNode = hf->next;
            /*disconnect cut node which  is same as first node from tree*/

            hf->next = NULL;
            return curNode; /*this is second node in original tree*/

        }
       
    }
    curNode = hf;

    /*if cut node is same as first node in forest, then no need to loop*/
    /*disconnect the cut node from forext and set the next node as first node of forest*/
   
    while(curNode)
    {
       if (curNode == cutNode) /*if curNode address is same as cutNode address then we got the node to be disconnected from forest*/
       {
            /*if it is last node*/
           if (cutNode->next == NULL) /*if cut node is last node in tree*/
           {
              prevNode->next = NULL; /*cut the cutNode from forest*/
              curNode = NULL;
              return origNode; /*return pointer to first Element of tree*/

              break;
           }
           else /*cutNode is not the last tree in the forest*/
           {
               /*disconnect cutNode from forest, generally when cutNode is in middle of forest*/
               prevNode->next = cutNode->next; /*connect prevNode to next tree of cutNode*/
               curNode = NULL;
               return origNode; /*return pointer to first Element of tree*/

               break;

           }
         

       }
     
       if (curNode) /*recursion call only if curnode is not null*/
       {
          prevNode = curNode;
          retTree = cutForest(curNode->next, cutNode);
       }
       else
       {
           return origNode; /*return pointer to first Element of tree*/

       }
   
    }
    return origNode; /*return pointer to first Element of tree*/
}

/**
 * MILESTONE 1: MUST USE RECURSION
 * appendForest appends a Huffman tree called newNode
 * to the END of the list of Huffman trees specified
 * by hf
 */
void appendForest(HTREE **hf, HTREE *newNode)
{
    HTREE* curNode = NULL;
    HTREE* tree = NULL;
    HTREE** origTree = NULL;
    char key;
    HTREE* nextNode = NULL;

   
       
    origTree = hf; /*save original value to hf (*hf)*/
    tree = *hf;

    /*first check if the append is first time*/
    if (!*hf)
    {
        *hf = newNode;
        return;
    }
    /*if not the first time then search in initial forest only if valid key*/
    key = newNode->letter;
    if (key > 0)
    {
       curNode = searchForestLetter(*hf, key);
    }
    if ((curNode) && key > 0) /* node foudn in forest- increment weight*/
    {
        curNode->weight++;
    }
    else /*it is new tree. it will be appened*/
    {
      
       /*find the node whose next node is null and append*/
        if(tree)
        {
            nextNode = (tree)->next;
            if (!nextNode) /*next node is null)*/
            {
               (tree)->next = newNode;
              
               return; /*after append return*/
            }
            tree = (tree)->next;
            appendForest(&tree, newNode);  /*recursive call to append forest*/
        }
       
    }

    /*before return set the tree pointing to original tree*/
    hf = origTree;
    return;
}

/**
 * GIVEN
 * printTree is provided for you and recursively
 * prints a Huffman forest.
 */
void printTree(HTREE* hf, int level) {
    int i = level;

        if (hf != NULL) {
        while (i > 0) {
            printf("  ");
            i--;
        }
                printf("-[%c|%d]\n", hf->letter, hf->weight);
                printTree(hf->left, level + 1);
                printTree(hf->right, level + 1);
        printTree(hf->next, level);
        }
}

/**
 * MILESTONE 1: MUST USE RECURSION
 * Huffman coding algorithm: create a single Huffman tree
 * from a forest of Huffman trees.
 *
 * huffmanize takes hf, a pointer to the first element of a forest of Huffman
 * trees, and performs the following operations on the forest:
 *   if hf is empty or if there is a single tree in the forest, return
 *   else there are other trees in the forest, so
 *       a. find the tree with smallest weight with searchForestMin and call
 *          it smallestTree
 *       b. cut this tree from the forest with cutForest
 *       c. repeat (a) and (b) to find and cut the next smallest weight tree
 *          called smallTree
 *       d. create a new tree called newTree with left equal to smallestTree,
 *          right equal to smallTree, no letter, and weight equal to the sum
 *          of the weights of smallestTree and smallTree
 *       e. append newTree to the forest
 *       f. recursively call huffmanize on this updated forest (until
 *          there is a single tree with all next pointers NULL)
 */
void huffmanize(HTREE** hf)
{
    HTREE* newTree, *smallestTree, *smallTree;

    if (hf == NULL || *hf == NULL) {
        return;
    }
    else if ((*hf)->next != NULL) {
        /* find two smallest weight trees and cut from forest */
        smallestTree = searchForestMin(*hf);
        /*cut the smallest tree from tree forest*/
        prevNode = NULL; /*reset prevNode to null - will be used in cutForest function*/
        *hf = cutForest(*hf, smallestTree);

        smallTree = searchForestMin(*hf);
        /*cut smallTree from forest*/

        prevNode = NULL; /*reset prevNode to null - will be used in cutForest function*/

        /*if there is only single node left in tree then smallTree node and first element of */
        /*forest will be same. In this case no need to cut the node from forest. Simply reset*/
        /*tree and set to newTree to be created*/
        if((smallTree->letter == (*hf)->letter) &&
            (smallTree->weight == (*hf)->weight) &&
            ((*hf)->next == NULL))
        {
            rootNodeFound = 1;
        }
        else
        {
            *hf = cutForest(*hf, smallTree);
        }
       

      

        /* make a new tree with left equal to the 
         * the smallest tree, right equal to the next smallest tree,
         * and weight equal to the sum of the trees
         */
        /*allocate memory for new tree - if not enough memory abort program*/
        if ((newTree = (HTREE*)malloc(sizeof(HTREE))) == NULL)
        { 
               abort();
        }
        newTree->letter = '\0'; /*null */
        newTree->weight = smallestTree->weight + smallTree->weight;
        newTree->left = smallestTree;
        newTree->right = smallTree;
        newTree->next = NULL;
       
        /* append the tree to the forest */
        prevNode = NULL; /*used in appendForest to save original pointer to tree*/
        if (rootNodeFound == 0) /*append if root node not yet created*/
        {
           
            appendForest(hf, newTree);
       
            /* recursively repeat until done */
          
            huffmanize(hf);
        }
        else /*root node created, set it as pointer to huffman tree*/
        {
            *hf = newTree; /*this is huffman tree, reset to first node*/
           
        }
       
    }
    return;
}

/**
 * MILESTONE 1: MUST USE RECURSION
 * Takes a Huffman tree hf, a letter to find in the tree, and
 * searches the tree to locate the character, generating the
 * Huffman code of ASCII 0s and 1s in the string path
 * and finally writing the result to the string pointed to by buffer
 * at the end.
 */
void makeHuffmanCode(HTREE* hf, char letter, char* path, char** buffer)
{

    /*if calling firts time traverse through huffman tree and create huffman code*/
    /* and save into buffer array*/

    count = 0;
    /*check if  huffman code created in buffer array*/
    if(huffmanTreeTraversed == 0)  /*huffman code not yet created*/
    {

        traverseHuffmanTree(hf, 0, path, buffer); /*traverses through tree recursively and creates huffman code*/
        huffmanTreeTraversed = 1; /*huffman tree traversed, do not traverse again*/

    }
   
    return;

}

/**
 * MILESTONE 2: does not need to use recursion
 * Takes a Huffman tree hf and stores in the string pointed to
 * by buffer the serialized version of the tree to be stored
 * at the beginning of the compressed file.
 *
 * This must be executed on the pre-sorted Huffman forest,
 * that is the singly-linked list of trees with null
 * left and right pointers.
 * The input parameter is pointer to initial forest tree.
 * This initial forest tree is searched starting from first node upto end
 * and each letter and weight is used for creating letter;weight; string.
 * when all letters are read from initial forest the string buffer contains
 * letters and weight ecah delimited by ";".
 */
void serializeTree(HTREE *hf, char **buffer)
{
    HTREE* curNode;
    curNode = hf;
    char key;
    int weight;
    char tmpStr[20] = {0};

    count = 0; /*used for saving number of letters in tree*/

    /*In the loop traverse through forest tree and get letter/weight and insert*/
    /*into buffer. Loop will terminate when next node in tree is NULL*/

    while(curNode) /*curNode is not null)*/
    {
        key = curNode->letter;
        weight = curNode->weight;
        sprintf(tmpStr, "%c;%d;", key, weight); /*write each letter/weight delimited by ";"*/
        buffer[count] = strdup(tmpStr);

        count++; /*increment count for each node*/
        curNode = curNode->next;
       

    }
    return;

                 
}

/**
 * MILESTONE 2: does not need to use recursion
 * Creates an initial forest of Huffman trees at hf from
 * the serialized string header.
 * This function will parse the header string using strtok
 * function and will create initila forest tree using
 * letter and weight extracted from header string.
 */
void unserializeTree(HTREE** hf, char* header)
{
    HTREE* hNode;
   
    char* token;
    char letter;
    int weight;
    int round = 0;

    token= strtok(header, ";"); /*initialize token*/

    /* This function will parse header string using strtok and ';' as delimiter*/

    while(token != NULL)
    {
        if (round == 0) /*if first time entry to loop then letter already extracted into token*/

        {
           letter = token[0];
        }
        else
        {
           token = strtok(NULL, ";"); /*This is letter*/
       
           letter = token[0];
        }
        /*In .hc file new line character(10 '\n') was added at the end of char/weight line*/
        /*make sure this character is not taken into consideration for initial forest*/
        /*construction*/

        if (letter != 10) /*new line character excluded*/
        {
           token = strtok(NULL, ";"); /*This is weight of letter*/
       
           weight = atoi(token); /* convert to int */
   
           /* create a forest tree*/
           /*allocate memory each tree to be created- if not enough memory abort program*/
           if ((hNode = (HTREE*)malloc(sizeof(HTREE))) == NULL)
           { 
               printf("Not enough memory\n");
               abort();
           }

           hNode->letter = letter;
           hNode->weight = weight;
           hNode->left = NULL;
           hNode->right = NULL;
           hNode->next = NULL;
           prevNode = NULL; /*used in appendForest to save original pointer to tree*/
           appendForest(hf, hNode);
           round++;
        }
        else /*if new line character '\n' move to next character*/
        {
            token = strtok(NULL, ";"); /*point to next character - prev charcater was '\n'*/

        }

    }

}

/**
 * MILESTONES 1 AND 2
 */



int main(int argc, char *argv[]) {
    /**
     * Program modes:
     *
     * -c for compress
     * ./executableName -c <filename> takes a text file called filename
     *     and creates a file called <filename>.hc which has the Huffman
     *    encoded contents of <filename>
     *
     * -d for decompress
     * ./executableName -d <filename.hc> takes Huffman encoded file named
     *     <filename.hc> (that is, ending with a .hc extension); creates
     *    file called <filename> without the .hc extension which has
     *    the ASCII encoded contents of <filename.hc>
     */
    FILE *inputFile = NULL, *outputFile = NULL;
   
    char* outputFilename; /*output file name*/
    char *mode, *filename;
    char inputText[BUFSIZE] = "\0";
    char huffCode[BUFSIZE] = "\0" ; /*For Saving of Huffcode in -d option*/
    int newLineFlag = 0; /*new line indicator while reading .hc file*/
    HTREE *hf = NULL, *hnode = NULL;
    HTREE *orighf = NULL;
    HTREE* curNode = NULL;
    char path[256] = "\0"; /*array for storing left right path while trversing tree*/
    char* buffer[257]; /*array for storing huffman code all ASCII characters*/
    char key;
    char* weightArr[1000]; /*string for saving character;weight in string before printing to .hc file*/

 
   
   

    int i = 0; /* counter */

    int user_char=0; /*used for reading each character from inpuit file*/
    /*uncomment after coding - delete before compiling on linux*/
   
    if (argc < 3) {
        printf("syntax: mp5 -[c|d] <filename>\n");
        return 0;
    }
    else {
        mode = argv[1];
        filename = argv[2];
    }
   
   

    /* MILESTONE 1: compression */
    if (strcmp(mode, "-c") == 0) {
        /* MILESTONE 1: use fopen to open the filename and read the
           contents into inputText
         */
        inputFile = fopen(filename, "r");
        if (!inputFile)
        {
            printf("Cannot open file %s", filename);
            exit (1);
        }
        for(i=0;;i++)
        {
           user_char = fgetc(inputFile); /*read each character from file*/
           if (user_char > 0)  /* to make sure there is no garbage value*/
             
           {
              inputText[i] = user_char; /*save each character in inputText Array*/
             
           }
           if(feof(inputFile)) /*if at end of file break from loop*/
           {
               break;
           }
        }

         fclose(inputFile); /*close input file read into inputText*/


        /* generate initial forest of Huffman trees from inputText */
      
        for (i = 0; i<strlen(inputText); i++)
   
        {
           /*allocate memory each tree to be created- if not enough memory abort program*/
           if ((hnode = (HTREE*)malloc(sizeof(HTREE))) == NULL)
           { 
               printf("Not enough memory\n");
               abort();
           }
           hf = orighf;
           hnode->letter = inputText[i];
           hnode->weight = 1;
           hnode->left = NULL;
           hnode->right = NULL;
           hnode->next = NULL;
           prevNode = NULL; /*used in appendForest to save original pointer to tree*/
           appendForest(&hf, hnode);
           /*save the pointer to first element during the iteration*/
           if (i == 0)
           {
               orighf = hf;

           }
        }
        /*restore initial pointer of hf*/
        hf = orighf;

        /*call serialize function to create char/weight pair serialize string*/
        serializeTree(hf, weightArr);

        /*restore initial pointer of hf*/
        hf = orighf;

        /* huffamanize tree - build binary tree*/
        origNode = hf;
        huffmanize(&hf);

       
        /* MILESTONE 1: use fopen to open <filename>.hc in write mode
         * and write the Huffman encoded contents of <filename> to the
         * file
         */
            outputFilename = (char*)calloc(strlen(filename) + strlen(".hc") + 1, sizeof(char));
            strcpy(outputFilename, filename);
            strcat(outputFilename, ".hc");

         /* open the file in write mode and write each
          * code to the file by calling makeHuffmanCode
          */
            outputFile = fopen(outputFilename, "w"); /* file mode set to append text*/

           if (!outputFile)
           {
              printf("Cannot open output file %s", outputFilename);
              exit (1);
           }
           /* in the loop of inputText call makeHuffmanCode to find huffman code*/
           /* and print in outputFile*/

           huffmanTreeTraversed = 0; /*huffman tree not yet traversed*/

           /*print char/weight pair as first line in .hc file terminated by \n*/
          
           for (i = 0; i<count; i++)
           {
               fprintf(outputFile,"%s",weightArr[i]);
             

           }
           /* at the end of letter/weight pair append new line character \n*/

           fprintf(outputFile,"\n");

           for (i = 0; i<strlen(inputText); i++)
           {
             
               key = inputText[i];

               makeHuffmanCode(hf, inputText[i], path, buffer);
               /*write into outfulFile*/
               fprintf(outputFile,"%s",buffer[key]);

           }
           fclose(outputFile); /*close output file */
           
    }
    /* MILESTONE 2: decompression */
    else if (strcmp(mode, "-d") == 0)
    {

        /* open .hc file and read first line containing letter/weight of each character*/

        inputFile = fopen(filename, "r");
        if (!inputFile)
        {
            printf("Cannot open file %s", filename);
            exit (1);
        }


        for(i=0;;i++)
        {
           user_char = fgetc(inputFile); /*read each character from file*/
         
           if (newLineFlag == 0)
           {
               if (user_char > 0)  /* to make sure there is no garbage value*/
                 
               {
                  inputText[i] = user_char; /*save each character in inputText Array*/
                 
               }
           }
                /*New line character was read in input file. After this character */
                /*the line is is with huffman code. Save into array for further use*/
           else
           {
               if (user_char > 0)  /* to make sure there is no garbage value*/
                 
               {
                  huffCode[i-1] = user_char;
               }
               else
               {
                  huffCode[i-1] = 0; /*terminate with null*/
               }
           }
           if(user_char == '\n') /*if at end letter/weight line - it was terminated by new line character*/
           {
               newLineFlag = 1;
               i = 0; /* Reset row counter, since this will be incremented to 1*/
                      /*Save next row at i -1 */
             
             
           }
           if(feof(inputFile)) /*if at end of file break from loop*/
           {
               break;
           }
        }
        
       
        fclose(inputFile); /*close input file read into inputText*/
       
        /*call unserialize function to create initial forest from letter/weight pair read from input file .hc*/
        unserializeTree( &hf, inputText);

        /*call huffmanize function to create huffman tree*/
       
        /* huffamanize tree - build binary tree*/
       
        huffmanize(&hf);

        /*Open output file name .dc for writing decoded huffman code*/

        outputFilename = (char*)calloc(strlen(filename) + strlen(".dc") + 1, sizeof(char));
        strcpy(outputFilename, filename);
        strcat(outputFilename, ".dc");

        /* open the file in write mode and write each
        * code to the file by calling makeHuffmanCode
        */
        outputFile = fopen(outputFilename, "w"); /* file mode set to append text*/

        if (!outputFile)
        {
            printf("Cannot open output file %s", outputFilename);
            exit (1);
        }



       /*In the loop of huffCode array, for each huff code find letter by traversing */
        /*through huffman tree */
        /*set CurNode to root node of huffman tree*/
        curNode = hf;
if (curNode != NULL)    {
        for (i = 0; i <=(strlen(huffCode) -1); i++)
        {
            key = huffCode[i];
            /* if there is only one node in the tree then get letter and print in file*/
            if ((curNode->letter != 0) && (curNode->left == NULL) && (curNode->right == NULL))
            {
                 /*write to .dc file*/
                 fprintf(outputFile,"%c",curNode->letter);

                 /*set CurNode to root node of huffman tree again for reading next code*/
                 curNode = hf;
            }
            else
            {

                if (key == '1') /* go to right node*/
                {
                    curNode = curNode->right;
                }
                else if (key == '0') /*go to left node*/
                {
                    curNode = curNode->left;
                }
                /*check if curNode is a leaf node. i.e the letter is  not '\0' (null)*/
                /*Get the letter and write to file.*/
                /*if curNode letter is null then read next huffCode and traverse based on */
                /*code, if '0' go left, if '1' go right*/

                if (curNode->letter != 0) /*leaf node , some letter is there*/
                {
                    /*write to .dc file*/
                    fprintf(outputFile,"%c",curNode->letter);

                    /*set CurNode to root node of huffman tree again for reading next code*/
                    curNode = hf;
                }
            }
           
        }
      
    }
         
       



        fclose(outputFile); /*close output file */

    }
    else {
        printf("invalid mode selected, use -c or -d.\n");
        return 0;
    }
    return 0;
}



/* this function will traverse through the huffman tree and create huffman code for each character*/
/* and save into buffer array*/


void traverseHuffmanTree (HTREE *hf, int treelevel, char* path, char** buffer)
{

    /*Check to see if we are at leaf node, that means we got a character in tree*/

    if ((hf->left == NULL) && (hf->right == NULL))
    {

        /*If there is only one node in the tree then traverse will be only once in left side*/
        /* in this case use global variable count to create huffman code*/
        if (count == 0)
        {
             path[treelevel] = '0'; /*only one left travel*/
             path[treelevel + 1] = 0; /*terminate with null*/


            buffer[hf->letter] = strdup (path);
            return;
        }
        else
        {
           /*terminate huffman code with null character stored in path array */

           path[treelevel] = 0;

           /* copy huffman code in the buffer array*/

           buffer[hf->letter] = strdup (path);
        }
       

    }
    else
    {
            count++;  /*after every traverse increment*/
            /* if  not at leaf node traverse left*/
           
            path[treelevel] = '0'; /*for left path set '0' in the path*/
           
            /*Recursively call traverseHuffmanTree() again*/
            traverseHuffmanTree(hf->left, treelevel+1, path, buffer);
           
            path[treelevel] = '1'; /*for right path set '0' in the path*/
            /*Recursively call traverseHuffmanTree() again*/
            traverseHuffmanTree(hf->right, treelevel + 1 , path, buffer);
    }
    return;
}
