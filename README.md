# CS-499 Chirp Project Phase 2

### Author: Anirudh Kaushik
### Email: anirkkau@usc.edu

## Setting up Vagrant:

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

  **Setting up dependencies**
Install grpc and protobuf

Use these instructions to install grpc and protobuf: https://github.com/grpc/grpc/blob/master/BUILDING.md
Go to grpc/third_party/protobuf in the grpc repository and run 

```./autogen.sh, ./configure, make && sudo make install```

Go to the root directory of the grpc repository and run sudo make install
Install gtest

```
  $ sudo apt-get install libgtest-dev
  $ sudo apt-get install cmake cd /usr/src/gtest 
  $ sudo cmake CMakeLists.txt 
  $ sudo make
  $ sudo cp *.a /usr/lib

```


## Running Chirp

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

## Running Tests
  To run tests, follow these commands:
  ```
    $ ./key_value_store_tests
    $ ./service_layer_tests
  ```

  All functionality is designed according to the Chirp Documentation. Commands will not work together and must be done one at a time.
  
## Command Line Reference

Register

```
  $ ./command_line --register <username>
```
(user must be registered to perform any other commands)
Chirp

```
  $ ./command_line --user <username> --chirp <chirp text>
```
Reply

```
  $ ./command_line --user <username> --chirp <chirp text> --reply <reply chirp id>
```
Follow

```
  $ ./command_line --user <username> --follow <username_to_follow>
```
Read

```
  $ ./command_line --read <chirp_id>
```
Monitor

```
  $ ./command_line --user <username> --monitor
```
**Cleaning:**
```$ make clean```: Removes all executables
