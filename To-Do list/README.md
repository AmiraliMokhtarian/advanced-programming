# C++ ToDo List

Simple command line ToDo list manager written in C++.

## Commands

NEW_LIST `name`  
create a new todo list

ADD `id` `title`  
add a new task

ADD_DEP `id` `title` `dep1` [`dep2` ...]  
add a task with dependencies

DONE `id`  
mark a task as done

STATUS `id`  
show task information

PRINT_ALL  
print all tasks

PENDING  
show pending tasks

READY  
show tasks that can be done

REMOVE `id`  
remove a task (if no task depends on it)

EXIT  
exit the program
