# ![normal logo ](./images/tree_small.bmp) Liber-Primus-Crib-Assistant
It's a game of guess the words. 

**Welcome to the documentation.**


## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)


## Overview 

-Liber-Primus-Crib-Assistant is an application designed to help crib words and phrases found in the unsolved pages of Liber Primus. 
-It aims to provide a simple interface to lists of pre-defined word data.;
-Enable users to filter and chose / not choose different word combinations for phrases that fit the word structure of the Liber Primus. 
-Generate crib lists to be used for further offline processing, or as data to be passed to decrypting attempts, (create a fake solution for the plaintext) etc. 


This app is still in development, basic features are working, but there is plenty of oppertunity for further improvements to the code and features.   In the current verison the basic achitecture, data object definitions and interactions through a GUI are being tested.  There is plenty of scope for further features to be added but first that will depend on if it useful and if it works as intended.   


## Main files and Path Definitions  

All paths are relative to the application folder. If installed to a folder called "Liber-Primus-Crib-Assist" the windows binary would be at  "./bin/win/LPCribAssist.exe". The main folder structure with some example files should look something like this: 

```html
Liber-Primus-Crib-Assist  [Applcation Root Directory ]
├─ bin/[binaries direcTory ]
│  ├─ win/	
│  │  ├─ LPCribAssist.exe [windows executable ] 	
├─ assets [MOVE ] 	
├─ doc [documetation directory ] 	
├─ data/
│  ├─ n_grams/	
│  │  ├─ 2-grams/				
│  │  │  ├─ 1 2.txt	[1 2 ngrams data file]
│  │  │  ├─ 1 3.txt	[1 3 ngrams data file]
│  │  │  ├─ ... 	
│  ├─ 1_grams_full / [1 grams dat directory (most complete wordlists) ]
│  ├─ phrases/[phrases ]
│  │  ├─ 1 8/ [Tutorial phrase 1 8 ]
│  │  │  ├─ 0_1 8.txt [data file for 1st ngram in phrase (0_) and ngram word lengths (1 8)]
│  │  │  ├─ phrase_config.ini [This phrases configuraiton file ]
│  │  │  ├─ puw_0.csv [data for for the 1st phrase unique words ]
│  │  │  ├─ puw_1.csv [data for for the 2nd phrase unique words ]
│  ├─lp_sents_2023.txt [dat for rune words, word lengths for each sentence in the sovled pages ]
├─ source/ [c++ source files ]
├─ config.ini  [main app config, plaintext, can be edited as needed ]
├─ README.md
├─ LICENSE
├─ LPCribAssist.pro [qmake file for compilaiton ]
├─ ...
```
 
File types (csv, txt, ini,  etc are what they are, all word data is held in what can be though of as csv fiels, adn ini data is key,value pair data that are all read by the c++ parser. (At a future date these may be rationalised or abstracted). 
 
 
## Quickstart - Installation 

Download the repository to your local PC.  

### Windows 

A compiled executable and other binary files are shared as part of the project simply run  "./bin/win/LPCribAssist.exe"

### linux 
As is often usual for linux users the best solution is to compile the source yourself. QT libraries and any dependancies are required for the build. If you use the QT developer tools (QT Creator) this should not be difficult, if you use your own tool-chain it may be.  When the application is run raw data files are typically loaded using relative paths that assume the main executabale is contained two folders lower from the root directory. This means that placing the compiled binaries in a folder such as ./bin/linux/ is advised.  


 
## Quickstart - Tutorial 

### super-quick 

A quick example using a single 2-gram with word lengths 1-8.  (More complex examples would have multiple n-grams overlapping one another.) 

Choose a new phrase 
(e.g.) In the "choose phrase tab" enter 1 8 (or 1,8) and press enter. The required file "../../data/n_grams/2-grams/1 8.txt" will be written to the "File to look for" text box. If files are written to the "Missing Files" either the paths aren't configured correctly or the files are missing. A phrase consists of meta-data, the lengths of words, the number of ngram, the starting index of the ngram in the phrase, the number of words in the phrase, the number of words in each n-gram, etc.
A "phrase meta data" overview will be displayed at the bottom of the window. This is a represention of the data required to define all the objects in the phrase.   

Create the new phrase data and write files (only if a suitable phrase has been entered).
If it is anticpated that the "phrase meta data" can be succesfully processed the "cretae new phrase" button will be green and can be pressed. When activated a pop up window should open at the "New Phrase" root directory. A new directory should be created in which to save the phrase data files being created. Once selected the app will run through the setup procedure.

Setup procedure
Using the "phrase meta data" a series of structured data objects are created. These contain lists of words or n-gram phrases and a chosen status for each entry, and described below.  

Load ngram files.  
(e.g.) load "./data/2-gram/1 8.txt"  

Cut the entries that contain words "not chosen" in the main-words lists. 
(e.g.) cut all ngrams that do not have "a" or "i" as first word.

From the loaded n-gram data find the "phrase unique words" for each word-index in the phrase.
(e.g.) Each word in a phrase has a "phrase unique word" object. The 1-8 test phrase has two "phrase uniq word" objects. from the cut n-gram data the 1st word in the phrase is "a" or "i". Therefore the 1st "phrase unique word" data oobject contains the words ["a","i"]. The 2nd "phrase unique word"  contains all the unique items from 2nd length 8 word in the cut ngram data.  

Words and/or ngrams are chosen / not chosen by interacting with the tables 
(e.g) As well as the "phrase unique word" containing words ["a", "i"] they also have a "chosen" list containing [True, True].  The ngram data also has "chosen" list as long as the number of  entries. The chosen status for entries in the "ngram tab" or the "puw tab"  "phrase_unique words" can be toggled by using the mouse / keyboard controls. Filters can be set to view "chosen" "not chosen" or "all" words for any list. For longer phrases, different "phrase unique word" or ngrams data sets can be selected using the slider.  


Rationalize the "phrase unique word" and n-gram data  
(e.g) The word lists are rationlized by using the combination of "phrase unique word" and "ngram" chosen values for each entry to decide which words are still chosen. In order to do this consistently the ngram data objecst have another list of "chosen by phrase unqiue words"  flags. The ngram has a flag for each word in its phrase. For ngram 1-8 thi smeans there is a nestsed list of length 2 lists, that would look like [[True, True],[True, True],[True, True],[True, True],...] The numerb of entreis is the number of ngram phrases. The values of these flags are set by the corresponding "phrase unique word" chosen status. If either of "ngram chosen" or "chosen by puw" are False the ngram will be set to "not chosen." On rationlisation the "phrase unique word" objects take precedence,  is  This is best explained with an example. Suppose "a" has been set "chosen"  and "i" is "not chosen" and our ngram phrases are: 

Phrase       , ngram chosen , puw chosen   , chosen by puw  
"a constant" , [True]       , [True, True] , [True] 
"i remember" , [True]       , [True, True] , [True]  
"a positive" , [True]       , [True, True] , [True] 
"a standard" , [True]       , [True, True] , [True] 
 
after rationalization with the 1st "phrase unique word" the data would look like:

Phrase       , ngram chosen , puw chosen    ,  chosen by puw               
"a constant" , [True]       , [True, True]  , [True]              
"i remember" , [False]       , [False, True] , [False]             
"a positive" , [True]       , [True, True]  , [True]              
"a standard" , [True]       , [True, True]  , [True]              

If "i remember " is subsequently set to "chosen" on the ngram tab, and "i" is still set to "not chosen", it will revert to "not chosen" on the next raitonlization.  If it is wanted to keep  "i remember" chosen in genrtal then "i" will have to be chosen in the "phrase unique word" object.

All-pick tab
When the number of possible words has been reduced this tab can be useful to try and pick out an exact crib. So far, Minimal development time has been spent here.    

All phrases tab
At some point the number of unique phrases  that are still chosen shoudl reach a numerb that can be printed to screen / file in a reaosnabel amount of time. These crib lists could be used for further offline processing, or as cribs-lists to be passed to decrypting attempts. When this happens and if it even works as intended in the current version is not fully known at the time of writing. 






### Main Concepts  

The app allows users to select words from different lists to build up possible phrase cribs. A phrase is defined by the number of runes in each word: 

3 12 4 is phrase consisting of a 3 words, 1st 3 runes long, 2nd 12 runes long and the 3rd 4 runes long.  

Phrases are split into a series of n-grams. N-grams are defined in pre-computed lists. The number of words in an n-gram (the n) can be 2,3,4 or 5 (currently only 2 has been tested and only data for 2-grams is supplied with the application).  The phrase 3 6 5 5, can be split in a number of different ways, for example:

into 3 2-grams 3 6, 6 5 and  5 5
into 2 3-grams 3 6 5, 6 5 5
into 1 2-gram and 1 3-gram 3 6, 6 5 5
etc. 
 
Each n-gram must overlap at least one other n-gram. This results in another word list for each word in the phrase (the so-called phrase-unique-words, often abbreviated to "puw" ) For the phrase 3 6 5 5 split into 3 2-grams 3 6, 6 5 and  5 5 the puw are: 

1st word (length 3): The unique, 1st words in ngram  3 6 
2nd word (length 6): The unique, 2nd words in ngram  3 6 AND 1st words in ngram 6 5
3rd word (length 5): The unique, 2nd words in ngram  6 5 AND 1st words in ngram 5 5
4th word (length 5): The unique, 2nd words in ngram  5 5

Finally there is the main word list, these are lists of all possible 1-grams from the raw n-gram files. These are used as the "global" word list that are used to make cuts to the n-gram data when first defining a phrase. 


### Rationlize 

The words in the solved pages in the Liber Primus were contained in the google 2- and 3- gram data. Therefore it is reasonabel to assume that all (or teh majority) of phrases in the unsolved pages are also contained this data set. 

When working on a phrase and cutting words and n-grams it is important to keep the choices made  consistant with one another. 
In general while working on a phrase is making choices as to which words are chosen / not chosen the full implications of those choices are not automatically updated.   

for example in a 2 word phrase 1-8 if we select "a" as the only choice for the first word then all 1-8 ngrams must also start with "a". 


### Tutorial: Setting up a new phrase example. 


Here we will woalk throught setting up a new phrase and making some simple cuts.    

On startup you shoudl see a window simialr to this:  
 
 
 
 



 


  




### Main words 

There are the word 1-grams. They are always loaded on startup.  











## cribbing concepts 

The main purpose is to allow users to choose words and phrases they think could be the encrypted plaintext of the Liber Primus. 


In general this is achieved by setting the "chosen" flag in the display 


The application uses pre-computed lists of "main-words" and "word n-grams" to define a "phrase." once a phrase is generated it also  contains "phrase-unique-words." As well as word lists a phrase has associated meta-data that defines the phrase structure (number of words, word lengths), filepaths etc. This meta data is contained with a phrase config file.

When working on phrase, the application provides an interface to these three word lists. If no phrase is loaded the "main-words" are still avilable.  


### main-words (1-grams)

The main-words are the "complete" set of possible words, split into groups by their number of runes (1 to 14, as 14 is the longest word in the Liber Primus). 
main words are contained in files in ./data/ directopry. Curently there are three different sets of main words:
./data/1_grams-full the complete list of words 
./data/1_grams-cut  A working directory where main words are being cut  
./data/1_grams-debug In this version a much reduced list fop words i sused to allow fro quicker loadign during debugging the app. If debugging the app it is recommended to use this or something similar.    

### file format
The applciation expects 14 main-word word raw data files, with names defined in the config.ini. The format of the files is csv (for strings use of a " start/end tag is optional). Each line shoudl containg a new word with the following column order (column labels should not be added to the file):  
English spelling, Counts, Chosen status, Runeglish Spelling, Gematria Sum  
Here is an example from the 1-gram word list (raw1grams_01.csv):

a,30285331759,1,ᚪ,97
i,7899622215,1,ᛁ,31
s,1345355452,0,ᛋ,53
p,929049236,0,ᛈ,43
c,883092357,0,ᚳ,13
b,806805692,0,ᛒ,61
d,583588182,0,ᛞ,89
e,499506445,0,ᛖ,67
m,441367251,0,ᛗ,71
t,421492308,0,ᛏ,59
n,405955090,0,ᚾ,29
l,404160295,0,ᛚ,73
x,338527486,0,ᛉ,47
f,336518599,0,ᚠ,2
r,308793080,0,ᚱ,11
o,293738275,0,ᚩ,7
v,262510244,0,ᚢ,3
j,260144283,0,ᛂ,37
h,245856157,0,ᚻ,23
g,241673426,0,ᚷ,17
w,173611017,0,ᚹ,19
y,173548512,0,ᚣ,103
k,173196151,0,ᚳ,13
u,130609439,0,ᚢ,3
q,81915626,0,ᚳ,13
ing,62494414,0,ᛝ,79
z,62398951,0,ᛋ,53
th,48565541,0,ᚦ,5
io,17729936,0,ᛡ,107
ia,16870355,0,ᛡ,107
ea,13811698,0,ᛠ,109
ae,8451074,0,ᚫ,101
oe,4859024,0,ᛟ,83

Here we can see the 1 rune words a and i are chosen and other words are not. We can also see that there are multiple words that have the same spelling in Runelgish (e.g. u and v) This is not a problem, in general words are chosen using the english spelling. The sentences "V is my favorite letter." and "U is my favorite letter." is different in English but not in runes.   
     
 
### Raw ngram-words (N-grams)
Precomputed lists of word n-grams are used to build phrases. Currently only 2-grams are included in 
./data/n_grams/2-grams, more are avialable here []. or users can add thier own files as long as it is formated corectly. 
Filenames: The file name must match the rune length of the words in the file. E.g. A file names "3 12.txt" must conatin lists of 2-gram swith 1 then 9 runes per word. 2-gram file must contain two words, a 3-gram file must contain english words. Raw ngrams files just contain the english spelling of the words and a count.  
English spelling, Counts
and consequently 43276681
and particularly 25737921
and international 23263962
and subsequently 15867407
and universities 13564950
for consideration 12243832
and communication 11607661 
...


### Phrase ngram-words  
For a defined phrase, the required ngram-word files are copied into the phrase directory and extar columns are added (explained below). The names of the files are generated by the app. They are prepended by the order of this ngram in the phrase. For a  phrase with word lengths 3-12-5 there wre two 2-grams 3-12 and 12-5 so the phrase ngram files will be called "0_3 12.txt" and "1_12 5.txt" repectively.  The extra columns in a phrase ngram -

and,consequently,1,1,1,43276681,1
and,particularly,1,1,1,25737921,1
and,international,1,1,1,23263962,1
and,subsequently,1,1,1,15867407,1
and,universities,1,1,1,13564950,1



Word lists come in three main types, main-words, phrase-ngrams and phrase-unique words    




## Features

In order to effectively use the application an understanding of a few of the design principles may be of benefit. 

### Data Files 

#### configuration 

Main configuration information is loaded on startup. It's a simple text file  using a [key-word] = [value] syntax. Paths are relative to the   



path_to_lp_sentences=../data/lp_sents_2023.txt
path_to_raw_1grams=../data/1_grams-debug
path_to_raw_ngrams=../data/n_grams
path_to_phrase_save_root=../data/phrases
path_to_last_phrase=../data/phrases/test
load_last_phrase_on_startup=true
raw_1grams_01=raw1grams_01.csv
raw_1grams_02=raw1grams_02.csv
raw_1grams_03=raw1grams_03.csv
raw_1grams_04=raw1grams_04.csv
raw_1grams_05=raw1grams_05.csv
raw_1grams_06=raw1grams_06.csv
raw_1grams_07=raw1grams_07.csv
raw_1grams_08=raw1grams_08.csv
raw_1grams_09=raw1grams_09.csv
raw_1grams_10=raw1grams_10.csv
raw_1grams_11=raw1grams_11.csv
raw_1grams_12=raw1grams_12.csv
raw_1grams_13=raw1grams_13.csv
raw_1grams_14=raw1grams_14.csv






- [Feature 1]
- [Feature 2]
- [Feature 3]
- ...

## Installation

### to use it

For Windows OS a compiled executable using QT libraries has been provided. This requires mandatory compliance with (L)GPL  [https://www.gnu.org/licenses/lgpl-3.0.en.html] as described here [https://www.qt.io/licensing]

### Linux Flavors 
Good luck. I have compiled thsi on a test version of ubuntu and go tteh app to work with minmimal effort. For most linux flavors then the best solution i can recommend is to use the QT maintenance tool and install QT development tools as required  (Using the QT defaults will be fine, but you may prefer to configure your own system your own way). If there is a strong need i may try and create a app image or similar.        




https://doc.qt.io/qt-6/linux-building.html


[Describe how to install the project. Include any prerequisites, step-by-step installation instructions, and any additional setup required.]

```bash
$ git clone [repository-url]
$ cd [project-folder]
$ [install-command]
```










## Introduction

Liber-Primus-Crib-Assistant (LPCA) is an 


test commit

Working windows binaries shared run /bin/LPCribAssist.exe


More to follow




A documentation template is always helpful. There are a lot of solutions to make one; we wanted to create our self-hosted version based on our favorite static site generator [Eleventy](https://www.11ty.dev/).

By structure, it is simple, with two levels and additional custom templates like [FAQ]([/faq/](https://eleventy-documentation.sprucecss.com/faq/)) and [Changelog]([/changelog/](https://eleventy-documentation.sprucecss.com/changelog/)).

## Spruce CSS

The template is built on [Spruce CSS](https://sprucecss.com/), a small and customizable CSS framework. The main benefit of this is that you can use the Spruce UI components with dark mode and RTL support.

## Features

- Breadcrumb navigation built on [11ty Navigation Plugin](https://www.11ty.dev/docs/plugins/navigation/).
- HTML minification in production mode.
- Anchor headings.
- Table of Content.
- FAQ template.
- Changelog template.
- Static search integration with [pagefind](https://pagefind.app/).
- Code highlighting.
- RTL support.
- Dark theme mode.
- [svgIcon](https://github.com/conedevelopment/sprucecss-eleventy-documentation-template/blob/main/src/shortcodes/svg-icon.js) shortcode: render any SVG icon inline and add optional classes.
- [markdownRenderer](https://github.com/conedevelopment/sprucecss-eleventy-documentation-template/blob/main/src/shortcodes/markdown-render.js): render any string (markdown) into HTML.

## Setup

1. **Clone the repository.**

2. **Install the dependencies.**

    In the `package.json` file, you will find all of the dependencies (and scripts) to install them using the following command:

    ```shell
    npm install
    ```

3. **Run the development mode**

    To run the development mode, use the `npm script`.   This script will also watch for changes.

    ```shell
    npm start
    ```

4. **Run the production mode**

    Before you go live, you should use the production script to compress the Sass files.

    ```shell
    npm run prod
    ```

You can find some more npm scripts in the [package.json](https://github.com/conedevelopment/sprucecss-eleventy-documentation-template/blob/main/package.json) that can be helpful.

## Content Managment

Adding content to the template is easy as almost everything is in Eleventy.

### The Basic Structure

Our base folder for the documentation pages is the `posts` folder. You must follow the folder structure, which means the `category` here. If you create a folder, you must make a list page with the same name as the folder. You must also create another `posts` folder under the `category` folder where your posts go. You must create the `posts.json` file that will parameter your `layout` and `permalink` values.

### Eleventy Navigation

The theme utilizes the [Eleventy Navigation plugin](https://www.11ty.dev/docs/plugins/navigation/), so you must explicitly set up the hierarchy. This is needed for the automatic sidebar navigation, the navigation order, and breadcrumb generation.

### Other Pages

To create simple pages, make a file directly under the `src` folder and configure it with the available front matter.

## Structure

```html
spruecss-eleventy-documentation-template/
├─ node_modules/
├─ dist/
├─ src/
│  ├─ _data/
│  ├─ _includes/
│  ├─ css/
│  ├─ filters/
│  ├─ font/
│  ├─ img/
│  ├─ js/
│  ├─ posts/
│  ├─ scss/
│  ├─ shortcodes/
│  ├─ transforms/
│  ├─ changelog.md
│  ├─ faq.md
│  ├─ index.md
├─ .eleventy.js
├─ package.json
├─ README.md
├─ ...

```

- **_data**: Some global data, like the name of your site and helpers like the active navigation element or current year.
- **__includes**: All of the layout and partial templates.
- **css**: The compiled CSS.
- **filters**: The additional filters that you can use.
- **font**: The custom fonts.
- **img**: The static image files.
- **posts**: The markdown contents.
- **scss**: The Sass files.
- **shortcodes**: The available shortcodes.
- **transforms**: The transformations.

<br>

[![Netlify Status](https://api.netlify.com/api/v1/badges/b7560c95-7035-491b-8c3f-94c43bea761e/deploy-status)](https://app.netlify.com/sites/sprucecss-eleventy-documentation/deploys)
