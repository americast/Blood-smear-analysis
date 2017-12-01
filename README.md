# Blood Smear analysis

Counts number of RBCs and WBCs in a sample photo of blood smear.

## Requirements  
* OpenCV 3.3.0
* G++ 5.4

## Execution  
Compile using  
`make`  
  
Run using  
`./count.out <image file path>`  

## Example
`./count.out sample.JPG`


The output image with marked contours can be found as `sample.JPG_final.jpg`.  
The contours maked in red represent RBCs while the once in blue represent WBCs.
