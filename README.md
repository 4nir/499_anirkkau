Author: Anirudh Kaushik
Email: anirkkau@usc.edu

1) Setting up:

  The Vagrant box I used:
  config.vm.box = "ubuntu/bionic64"

  Tools & Libraries:
  - grpc (installed according to official docs)
  - gflags (installed according to official docs)

2) Running Chirp

  To build the program, follow these commands:
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

  All functionality is designed according to the Chirp Documentation, with the assumption that input is valid.
  Given my restricted time, I could not get to handling bad inputs. 
   
3) General Notes: 

 - Running monitor will notify you if the last chirp is from someone you follow, and displays a feed of new chirps from everyone you follow 
 - If you don't see output in the command_line window, you might find responses in the other two servers.