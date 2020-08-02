# SSL-Enabled-Secure-Communication-Model



*This project was undertaken by me during the tenure of my Summer Training at C-DAC, Noida.*



__The Problem Statement__

*Construct a Client-Server Module in C and initiate a “secure” Communication between the two using standard functions of the OpenSSL Library.*



__Development Environments__

*The project was developed in the following environment :-*

* OS : Ubuntu
* Eclipse IDE for C/C++ Developers
 (*Do link "-lssl" and "-lcrypto" libraries to both programs*)
* Packages installed :-           
  * OpenSSL
      (sudo apt-get install openssl && sudo apt install libssl-dev)
  * OpenJDK (version 11.0.8) 
* Additional Software :-
  * SocketTest 3 (for testing Sockets)
  * WireShark (for analysis of Network Packets)
* Self-Signed SSL Certificate 
  *(steps for generation of the same are mentioned below)*



__Generation of Self-Signed SSL Certificate__

*Open Linux Terminal and execute the following commands :-*

1. openssl genrsa -des3 -out server.key 2048
2. openssl req -new -key server.key -out server.csr
3. cp server.key server.key.org
4. openssl rsa -in server.key.org -out server.key
5. openssl x509 -req -days 30 -in server.csr -signkey server.key -out server.crt
   * (*Here, the number "30" denotes the validity of the Certificate, which may be changed accordingly*)

__NOTE__ : The certificate & corresponding keys will be located in /home directory, by default. Copy and Paste the same into "Debug" folder of SERVER side program. 


__PROCEDURE for running and testing the Project__

1. Compile both the programs and check whether the code contains any errors.
2. In case of "Linker Error", do make sure that the libraries *'ssl'* and *'crypto'* are linked to the respective programs.
   * (*Steps to link respective libraries: Right Click your Project > Properties > Double-Click 'C++ Build' > Settings > Select 'Libraries' under 'GCC C Linker' tab > Click the 'Add' button > Type-in the name of libraries > Apply and Close*)
3. Run 'SERVER' from the Terminal. Enter the IP Address (eg. 127.0.0.1) and the Port Number (eg. 1111).
4. Run 'CLIENT' from another Terminal window. Enter the SERVER's IP Address and Port Number in order to connect with it.
5. Observe the communication (exchange of messages) happening between the two.
6. To test respective SOCKETS, use SocketTest.
7. To analyse the Network Packets and observe SSL Handshake, use WireShark.
