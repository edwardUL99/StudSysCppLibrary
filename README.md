# StudSysCppLibrary
The library for a Student Management System coded in C++

This repository provides the library for the code behind the Student Management System. It provides a means for separation of backend implementation from ui implementation. This has been done so the library can be used with any UI, whether it is CLI or GUI, rather than duplicating code

# How to compile
Change directory into the root of project you cloned and open a terminal there and type make.
You can then copy the libstudsys.so file to /usr/lib/studsys or in a separate directory and specify the path to it with -L<path> flag

# How to use
You can copy all the headers into a studsys folder in /usr/include or into a different directory and then include the headers required
When compiling the UI code make sure you specify -lstudsys and if necessary -L<path> where path is the location of the libstudsys.so

For the sql files, when setting up the database, you need to set up a user that has full access to the database that will be used. You can then import the sql files using (on linux):
  mysql -u <user> --password=<password> <database-name> < <sql-file>.sql 
  See database setup

# Database Setup:
   You need a MySQL server either MySQL or MariaDB
   Steps:
       1. Login to MySQL using mysql -u [user-name] -p (this user must be able to create databases and users)
       2. Type CREATE DATABASE <database-name>;
       3. Exit the MySQL shell
       4. Now type mysql -u [user-name] --password=[user-password] [database-name] < [file-name.sql]. You will need to do this with schema.sql, triggers.sql and procedures.sql
       5. Log back into the mysql shell and create a user for the system to use and password by typing CREATE USER '<user-name>'@'localhost' IDENTIFIED BY 'user-password';
       6. Then type GRANT ALL PRIVILEGES ON [database-name].* TO '[user-name]'@'%';
       7. Then type FLUSH PRIVILEGES; and then exit
       8. Ensure bind-address in mysql config is set to 0.0.0.0 on the database server machine
   The MySQL server is now set-up and ready to use with the system
