#define SHARED_IMPL
#ifndef SHARED_H
#define SHARED_H
	#include <ctype.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <errno.h>
	
	typedef struct {
		size_t capacity; /*The Amount Of Allocated Storage*/
		size_t used; /*The Amount Being Used*/
		size_t size; /*The Size Of One Element*/
		void**  data; /*The Data Being Stored*/
	} void_arr;
	
	
	#ifdef _WIN32
		#include <Windows.h>
		#define TEXT_EDITOR "C:\\Windows\\System32\\notepad.exe"
	#else
		/* Assuming Anything That Isnt Windows Is Posix */
		#include <unistd.h>
		#include <sys/wait.h>
		#define TEXT_EDITOR "vi"
	#endif

	#define SUCCESS_NOT_FINISHED -2

	void  strlower(const char* in, char* out);
	#ifdef _WIN32
	PROCESS_INFO* 
	#else
	pid_t
	#endif
	makechild(const char* name, const char* args[]);
	int   runchild(const char* name, const char* args[]);
	void  dyn_append(void_arr* array, void* element);
	int waitchild(
		#ifdef _WIN32
			PROCESS_INFO*
		#else
			pid_t
		#endif
			process, _Bool hang);
	
	#ifdef SHARED_IMPL

	void strlower(const char* in, char* out){
		for (size_t i = 0; i < strlen(in); i++){
			out[i] = tolower(in[i]);
		}
        out[strlen(in)] = '\0';
	}
	
	int runchild(const char* name, const char* args[]){
		#ifdef _WIN32
			STARTUPINFO  startup_info;
			PROCESS_INFO process_info;
	
			ZeroMemory(&startup_info, sizeof(startup_info));
			startup_info.cb = sizeof(startup_info);
			ZeroMemory(&process_info, sizeof(pi));
	
			int i = 0;
			char* args_str = "\"";
			while (args[i] != NULL){
				strcat(args_str, args[i]);
				strcat(args_str, args[i + 1] == NULL ? "\"" : "\" \"");
				i++;
			}
	
			bool state;
			state = CreateProcess(name, args_str,
							NULL , NULL ,
							false, 0    ,
							NULL , NULL ,
						&startup_info, &process_info);
			if (!state){
					return -1;
			}
	
			long int res = WaitForSingleObject(process_info.hProcess, INFINITE);
	
			int err_c;
			(void) GetExitCodeProcess(process_info.hProcess &err_c);

			CloseHandle(process_info.hProcess);
			CloseHandle(process_info.hThread);
			return err_c;
		#else
			/* Assuming Everything That Isnt Windows Is Posix */
			pid_t child_pid   ;
			int   child_status;
	
			child_pid = fork();
			if (child_pid == -1){
					return child_pid;
			}
	
			if (child_pid == 0){
					execvp(name, (char* const *)args);
			}
	
			do {
				waitpid(child_pid, &child_status, (int)(long long)NULL);
			} while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));
	
			if (WIFEXITED(child_status))   return WEXITSTATUS(child_status) ;
			if (WIFSIGNALED(child_status)) return WTERMSIG(child_status)    ;
	
			return -1;
		#endif
	}
	
	inline void dyn_append(void_arr* array, void* element){
		if (array->capacity == array->used){
			array->capacity *= 2;
			void** data = (void**)realloc(array->data, array->capacity * array->size);
			array->data = data;
		}
		array->used++;
		array->data[array->used] = element;
	}

	#ifdef _WIN32
	PROCESS_INFO* 
	#else
	pid_t
	#endif
	makechild(const char* name, const char* args[]){
		#ifdef _WIN32
			STARTUPINFO  startup_info;
			PROCESS_INFO process_info;
	
			ZeroMemory(&startup_info, sizeof(startup_info));
			startup_info.cb = sizeof(startup_info);
			ZeroMemory(&process_info, sizeof(pi));
	
			int i = 0;
			char* args_str = "\"";
			while (args[i] != NULL){
				strcat(args_str, args[i]);
				strcat(args_str, args[++i] == NULL ? "\"" : "\" \"");
			}

			bool state;
			state = CreateProcess(name, args_str,
							NULL , NULL ,
							false, 0    ,
							NULL , NULL ,
						&startup_info, &process_info);
			if (!state){
					return -1;
			}
			return &process_info;
		#else
			pid_t child_pid;
	
			child_pid = fork();
			if (child_pid == -1){
					return child_pid;
			}

			if (child_pid == 0){
					execvp(name, (char* const *)args);
			}
			return child_pid;
		#endif /* _WIN32 */
	}

	int waitchild(
		#ifdef _WIN32
			PROCESS_INFO*
		#else
			pid_t
		#endif
			process, _Bool hang){

		#ifdef _WIN32
			long int res = WaitForSingleObject(process_info.hProcess, hang ? INFINITE : 1);
	
			if (res == WAIT_TIMEOUT){
				return -2;
			} else if (WAIT_OBJECT_0){
				int err_c;
				(void) GetExitCodeProcess(process_info.hProcess &err_c);

				(void) CloseHandle(process_info.hProcess);
				(void) CloseHandle(process_info.hThread);
				return err_c;
			}
		#else
			int child_status;
			if (hang){
				do {
					waitpid(process, &child_status, (int)(long long)NULL);
				} while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));

				if (WIFEXITED(child_status))   return WEXITSTATUS(child_status) ;
				if (WIFSIGNALED(child_status)) return WTERMSIG(child_status)    ;
	
				return -1;
			} else {
				waitpid(process, &child_status, WNOHANG);
				if (WIFEXITED(child_status))   return WEXITSTATUS(child_status) ;
				if (WIFSIGNALED(child_status)) return WTERMSIG(child_status)    ;
				return -2;
			}
		#endif /* _WIN32 */
	}

	#endif /*SHARED_IMPL*/
#endif /*SHARED_H*/
