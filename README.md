# operating-system-project-2023
Commands
For mad_sequential<br/>
gcc mad_sequential.c -o mad_sequential 
./mad_sequential tiny.txt 
./mad_sequential large.txt

For mad_process
gcc mad_process.c -o mad_process
./mad_process tiny.txt 
./mad_process large.txt

For mad_thread
gcc mad_thread.c -o mad_thread or gcc mad_thread.c -lpthread
./mad_thread tiny.txt 3
./mad_thread large.txt 3

Aim of the project is using and creating processes and threads for communication of data. Ordinary pipes were used as IPC mechanism. 

For process,
The new process created by the fork () system call. The data was taken from the file and saved in an array. 
Then, the data in the array was divided into two for two separate processes. 
Operations such as mad calculation and range calculation by child and parent processes were carried out using 4 ordinary pipes.
To elaborate a little more, the child process calculates the sum of half of all the data and sends it to the parent process. 
The parent process calculates the average using this sum. The calculated average is sent to the child process by the parent 
process and the deviations calculation is made in both processes. Then the child process calculates the deviation sum which it was calculated.
It sends it to the parent process and the total deviation is calculated by the parent. Finally, the parent process calculates the MAD using the total deviation.
To calculate the range, the maximum and minimum values of the array in both the child and parent processes are calculated. 
Then the parent process sends its max and min values to the child process in an array. 
The child process finds the real max and min values by comparing both max and min values. Then using these, it calculates the range.

For basic program,
Range and mad calculations were made with basic c code without using thread and process. Since no communication way is used, the execution time will take longer.

For threads,
In mad_thread.c file we also saved numbers in array. After saving numbers in array, numbers size in array divided into given thread number.
With this way we create tread parts with using mutex lock and unlock. We used thread parts for MeanAbsoluteDeviation function and Range function.
The advantage of the thread is to make the function faster and make it parallel. In Range part, range calculated with given numbers with thread parts.
Each thread part searched for certain part of code. Then searching max and min numbers are subtracted from each other.
In main part, the given arguments will be declared. The file will be open. The size of file will be declared.
Then, we create 2 separate threads for MeanAbsoluteDeviation and Range functions. 


