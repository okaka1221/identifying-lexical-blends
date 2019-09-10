# identifying-lexical-blends

## Structures
```
.
├── code
│   ├── header
│   │   ├── file_io.h
│   │   ├── ranking.h
│   │   ├── similarity.h
│   │   └── structure.h
│   │
│   ├── file_io.cpp
│   ├── main.cpp
│   ├── ranking.cpp
│   ├── similarity.cpp
│   └── plot-roc-curve-auc.py
├── data
├── output
└── README.md
```
  
  
## Description of each folers and files  

- header folder  
Contains all header files.

- file_io.cpp  
Read given data and preprocess them if necessary.

- main.cpp  
Read necessary files, obtain scores for predicting lexical blends, and output results.  
This is main file and calls file_io class and ranking class.

- ranking.cpp  
Obtain similarity between word and lexical blend, calculate weighted similarity based on frequency of word occurence for all possible splits mentioned in the paper, and sort scores in descending order.  
This class calls similarity clas.  

- similarity.cpp  
Calculate similarity between given two words using approximate string matching algorithms.  

- plot-roc-curve-auc.py  
Plot ROC curve, calculate AUC score and save graph as an image.  

- data folder  
Contains all given data.

- output  
Contains all outputs such as processed data, prediction and images.
