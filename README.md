# Image Compression

This is an implementation of a content-aware image compression algorithm. The content awareness mechanism is based on entropy, which is computed using the hue value frequency distribution of color pixels in a given rectangle. Then the parent rectangle image will be split (either horizontally or vertically) so that the weighted average entropy of the two sub-rectangles gets minimized. This process is repeated recursively until the 2 sub-rectangles become single pixels. Technically, this approach is a modified KD Tree.<br/>

As indicated in the section below, the input images get compressed without losing resolution in the most informative regions. 

## Demo

Input image           |  Compressed image
:-------------------------:|:-------------------------:
![temple_raw](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/temple.png)  |  ![temple_comp](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/Result_temple.png)

Input image           |  Compressed image
:-------------------------:|:-------------------------:
![sakula_raw](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/sakula.png)  |  ![sakula_comp](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/Result_sakula.png)

Input image           |  Compressed image
:-------------------------:|:-------------------------:
![DSC_0316_raw](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/DSC_0316.png)  |  ![DSC_0316_comp](https://github.com/KaiTaiTong/image-compression/blob/master/images/demo/Result_DSC_0316.png)




## Reference
Skeleton code and structures are provided by [Prof. Cinda K. Heeren](https://www.cs.ubc.ca/people/cinda-heeren) as part of a course on data structure and algorithm in University of British Columbia (UBC).

Raw source images are cited here:<br/>
[temple image](https://www.pandotrip.com/top-10-natural-wonders-in-japan-29361/)<br/>
[fuji image](https://www.plazajapan.com/4977524331261/)<br/>
[totem image](https://where.ca/british-columbia/vancouver/a-activities-our-favourite-reasons-to-visit-ubc/)<br/>
