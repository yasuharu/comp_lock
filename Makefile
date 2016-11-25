
default:
	gcc main.c -o main_sem -lpthread -DUSE_SEMAPHORE
	gcc main.c -o main_pthread -lpthread -DUSE_PTHREAD

