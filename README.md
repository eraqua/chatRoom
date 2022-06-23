# Chatoom

## How to run it

You need to compile the c/c++ code file via using the gcc/g++ command.
Otherwise,because we need to use the lib of pthread which is the lib in Linux but not the official lib of c/c++,so when we compile the file ,we need to link the pthraed,for example:" gcc server.c -o server -lpthread ".

After compiling the code file,we can start the application by using the command of " ./server ".
Otherwise,we need to start the server first before open the client.

Besides,you can use different command to control the application.

---
## Server commanad

1. " /tick + id ":can ban the connection of target id;
2. " /ban + words ":can flitter the target words which are no allowed to talk in the chat group;
3. " /save + time(hours) " can temporally store the chat history logs.If zero or less hour,it means will not delect the saved file;

---

## Client command

1. " /recall ":you can recall your recent sent message;
2. " /mute + username "you can use this command to avoid recieving the meaasge from the target user;
3. " /save ":you can save the recieved chat history in the local hard disk;
