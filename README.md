# CS-499 Chirp Project Phase 2

## Author: Anirudh Kaushik
## Email: anirkkau@usc.edu

1) Setting up Vagrant:

  Reference here for more detailed walkthrough: https://www.vagrantup.com/ 

  **Download Vagrant VM**
  -Download Vagrant VM: https://www.vagrantup.com/downloads.html

  **Set up directory**

  ```
    $ mkdir 499_anirkkau
    $ cd 499_anirkkau
  ```

  **Initialize VM**

  This will create a Vagrant file and it should specify:
  ```
    config.vm.box = "ubuntu/bionic64"
  ```

  ```
    $ vagrant init ubuntu/bionic64
  ```

  **Setting up dependencies:**

    1. Update apt-get: ```sudo apt-get update```
    2. Install g++: ```sudo apt-get install g++```
    3. Install cmake: ```sudo apt-get install cmake```
    4. Install prereqs: ```sudo apt-get install build-essential autoconf libtool pkg-config```
    5. Install gtests & gflags: ```sudo apt-get install libgflags-dev libgtest-dev . cd /usr/src/gtest``` b. ```sudo cmake CMakeLists.txt``` c. ```sudo make``` d. ```sudo cp *.a /usr/lib```
    6. Install clang: ```sudo apt-get install clang libc++-dev```

**Setting up gRPC**

    ```$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc ```
    $ ```cd grpc $ git submodule update --init```
    Install gRPC 
    a. ```sudo make ```
    b. ```sudo make install ```
    c. ```cd grpc/third_party/protobuf ```
    d. ```sudo make install```
    In grpc root folder: ```sudo make install```


2) Running Chirp

  To build all executables and tests, follow these commands:
  ```
    $ cd /vagrant/src/chirp/cpp
    $ make
  ```

  After which, three executables will be generated: backend_store_server, service_layer_server, command_line

  You will need to run each in a separate terminal window, as follows:
  - $ ./backend_store_server
  - $ ./service_layer_server
  - $ ./command_line <flags>

  To test chirp() functionality:
  Chirp IDs are generated chronologically with the format "cid//#",
  where # starts from 0 and is incremented by 1 with each new chirp.
  For example: cid//1, cid//2, cid//3, etc.

  All functionality is designed according to the Chirp Documentation. Commands will not work together and must be done one at a time.
   
**Cleaning:**
```$ make clean```: Removes all executables