
# What's the format of bounding box?

Bounding boxes are used to define the object's location and size. Bounding boxes are represented by three different formats: Pascal VOC, COCO and YOLO.



## [Exploring Bounding Boxes and Different Annotation Formats in Object Detection](https://idiotdeveloper.com/exploring-bounding-boxes-and-different-annotation-formats-in-object-detection/)

Bounding boxes play a critical role in the object detection process, a fundamental task in computer vision. Object detection entails the identification and precise localization of objects within an image. These bounding boxes serve to define the position of an object within the image, allowing for the creation of a box that encapsulates it.


### What is Bounding Box?
A bounding box is a rectangular region that surrounds an object within an image. It is used to define the object’s location and size in an image.


There are different formats for representing the coordinate points of the bounding boxes. The three most commonly used formats are:

- Pascal VOC

- COCO

- YOLO


### Bounding Box Format: Pascal VOC
The pascal_voc format is used by the [Pascal VOC dataset](http://host.robots.ox.ac.uk/pascal/VOC/). It consists of four coordinate points, each represented by a real number, that define the location of the bounding box in an image. The four coordinate points represent by:

- x-min
- y-min
- x-max
- y-max

The x-min and y-min values represent the top-left corner of the box, while the x-max and y-max values indicate the bottom-right corner of the box.

```
   (x-min, y-min)
            oooooooooooooo
            o            o
            o            o
            o            o
            o            o
            oooooooooooooo
                    (x-max, y-max)
```

The pascal_voc format provides a standardized way to annotate object instances within images, making it a commonly used format in object detection research and development.

### Bounding Box Format: COCO

The coco format is used by the [Common Objects in Context COCO](https://cocodataset.org/) dataset. It is defined by four values, which are:

- x-min
- y-min
- width of the bounding box
- height of the bounding box

```
   (x-min, y-min)
            oooooooooooooo -
            o            o ^
            o            o |
            o            o |height
            o            o |
            oooooooooooooo v
            |<  width   >|
```
Here, the terms x-min and y-min denote the coordinates of the top-left corner of the bounding box, whereas the width and height specify the dimensions of the bounding box.


### Bounding Box Format: YOLO
The yolo format is introduced in the YOLOv1 paper and then it is used continuously. It also defines four values, which are:

- x-center
- y-center
- width of the bounding box
- height of the bounding box

Here, the terms x-center and y-center denote the coordinates of the center of the bounding box, whereas the width and height specify the dimensions of the bounding box.
```
            ooooooooooooooooooooooooo -
            o                       o ^
            o                       o |
            o           o           o |height
            o (x-center, y-center)  o |
            o                       o |
            ooooooooooooooooooooooooo v
            |<---------  width  ---- >|
```

### Summary
In summary, bounding boxes are a crucial component of object detection algorithms, providing the necessary information to locate and classify objects within an image. Bounding boxes are used to define the object’s location and size. Bounding boxes are represented by three different formats: Pascal VOC, COCO and YOLO.