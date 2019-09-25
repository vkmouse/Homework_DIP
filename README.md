## The homework for digital image processing course
This project include five homeworks for digital image processing course. The code is written with C++ and OpenCV. The environment setup and the implements of each homework will be introduced below.

- [Environment setup](#Environment-setup)
- [Homework 1](#Homework-1)
- [Homework 2](#Homework-2)
- [Homework 3](#Homework-3)
- [Homework 4](#Homework-4)
- [Homework 5](#Homework-5)


### Environment setup
Install OpenCV on ubuntu. The following example installs OpenCV with the docker image. So first you have to install docker.

```bash
apt-get update
apt install docker.io
docker --version
```

Second, create a file named **dockerfile** in the root path. Paste the following illustration to the **dockerfile**.

	FROM ubuntu:18.04
	RUN apt-get update && apt-get install -y \	
		build-essential \
		cmake \
		vim \
		git	
    RUN git clone https://github.com/vkmouse/Homework_DIP.git
    CMD ["/bin/bash"]

Third, build a new image with the dockerfile. This image is built based on ubuntu:18.04.

```bash
docker build --rm=true -t opencv .
```

Fourth, allows everybody to use your host x server.

```bash
xhost +local:root
```

Fifth, run the image that allows the [GUI](http://wiki.ros.org/docker/Tutorials/GUI).

```bash
docker run -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix opencv:latest
```

Finally, install the OpenCV package in the docker.

```bash
apt-get install -y libopencv-dev
```

### Homework 1
Implement two image transformation method ,nearest-neighbor interpolation and bilinear interpolation, to resize the image with double scale.

Example:

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-1.jpg?raw=true)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-2.jpg?raw=true)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-3.jpg?raw=true)|
|---|---|:---:|
| Original picture | Nearest neighbor interpolation | Bilinear interpolation |

### Homework 2
Implement image registration with bilinear interpolation to turn the picture into positive.

Example:

|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW2-1.jpg" width="232" height="366" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW2-2.jpg" width="223" height="278" />|
|---|---|
| Original picture | After image registration |

### Homework 3
Sharpen an image with unsharp masking. Unsharp masking can enhance the edge instead of noise compared to high-pass filters.

Example: 

|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW3-1.jpg" width="177" height="266" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW3-2.jpg" width="177" height="266" />|
|---|---|
| Original picture | After image sharpening |

### Homework 4
Adding Gaussian noise and salt-and-pepper noise to the image. And then denoise the image using mean filter, median filter, and alpha-trimmed filter.

Example:

|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-1.jpg" width="144" height="144" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-2.jpg" width="144" height="144" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-3.jpg" width="144" height="144" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-4.jpg" width="144" height="144" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-5.jpg" width="144" height="144" />|
|:---:|:---:|:---:|:---:|:---:|
| Original picture | Noisy picture | Mean filter | Median Filter | Alpha-trimmed filter |

### Homework 5
Implement the morphological hole filling.

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-1.jpg)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-2.jpg)|
|---|---|
| Original picture | After hole filling |