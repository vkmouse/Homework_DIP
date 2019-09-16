## The homework for digital image processing course
This project include five homeworks for digital image processing course. The code is written with C++ and Opencv. The environment setup and the implements of each homework will be introduced below.

### Environment setup
Install OpenCV on ubuntu. The following example installs OpenCV with the docker image. So first you have to install docker.

```bash
apt-get update
apt install docker.io
docker --version
```

Second, create a file named **dockerfile** in the root path. Paste the following illustration to the **dockerfile**.

    FROM ubuntu:18.04
	RUN sudo apt-get update && apt-get install -y \	
		build-essential \
		cmake \
		vim \
		git	
    RUN git clone https://github.com/vkmouse/Homework_DIP.git
    CMD ["/bin/bash"]

Third, build a new image with the dockerfile. This image is built based on ubuntu:18.04.

```bash
docker build --rm=true -t OpenCV.
```

Fourth, allows everybody to use your host x server.

```bash
xhost +local:root
```

Fifth, run the image with [GUI](http://wiki.ros.org/docker/Tutorials/GUI) and execute the bash.

```bash
docker run -it --rm=true --name HW_DIP -e DISPLAY=$DISPLAY -v="/tmp/.X11-unix:/tmp/.X11-unix:rw" -d OpenCV:latest
docker exec -it HW_DIP bash
```

Finally, install the OpenCV package in the docker.

```bash
apt-get install -y libopencv-dev
```

### Homework 1
Implement two image transformation method ,nearest-neighbor interpolation and bilinear interpolation, to transform the scale of picture.

### Homework 2

### Homework 3

### Homework 4

### Homework 5

