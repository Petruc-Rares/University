#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define STATUS_OK 200
#define STATUS_CREATED 201
#define BAD_REQUEST 400
#define TOO_MANY_REQUEST 429
#define FORBIDDEN_REQUEST 403
#define STATUS_NOT_FOUND 404
#define INTERNAL_SERVER_ERROR 500

#define REGISTER_LENGTH 8
#define REGISTER_TEXT "register"

#define LOGIN_LENGTH 5
#define LOGIN_TEXT "login"

#define ENTER_LIBRARY_LENGTH 13
#define ENTER_LIBRARY_TEXT "enter_library"

#define GET_BOOKS_LENGTH 9
#define GET_BOOKS_TEXT "get_books"

#define GET_BOOK_LENGTH 8
#define GET_BOOK_TEXT "get_book"

#define ADD_BOOK_LENGTH 8
#define ADD_BOOK_TEXT "add_book"

#define DELETE_BOOK_LENGTH 11
#define DELETE_BOOK_TEXT "delete_book"

#define LOGOUT_LENGTH 6
#define LOGOUT_TEXT "logout"

#define EXIT_LENGTH 4
#define EXIT_TEXT "exit"

#define MAX_INPUT_LENGTH 20
#define MAX_USERNAME_LENGTH 30
#define MAX_PASSWORD_LENGTH 50
#define REGISTER_JSON_MAX_LENGTH 120
#define MAX_TITLE_LENGTH 30
#define MAX_AUTHOR_LENGTH 50
#define MAX_GENRE_LENGTH 20
#define MAX_PUBLISHER_LENGTH 20
#define MAX_PAGE_COUNT_LENGTH 40
#define MAX_ID_LENGTH 50
#define MAX_ADD_BOOK_LENGTH 210

#include "helpers.h"
#include "requests.h"

void send_register(int sockfd, char *username, char *password) {
    char *message;
    char *response;

    char *body_data = malloc(REGISTER_JSON_MAX_LENGTH);
    sprintf(body_data, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);

    message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/register",
                                "application/json", &body_data, 1, NULL, 0, NULL);
    free(body_data);

    send_to_server(sockfd, message);
    free(message);
    response = receive_from_server(sockfd);

    int statusCode;
    sscanf(response, "%*s %d %*s", &statusCode);

    if (statusCode == STATUS_CREATED) {
        printf("Account successfuly registered\n");
    } else if (statusCode == BAD_REQUEST) {
        char *error_temp = strstr(response, "{\"error\":");
        if (error_temp) {
            // get just the error message
            error_temp[strcspn(error_temp, "\n")] = '\0';
            printf("%s\n", error_temp);
        }
    } else {
        printf("status code not checked\n");
    }
    free(response);
}

char *send_login(int sockfd, char *username, char *password) {
    char *message;
    char *response;

    char *body_data = malloc(REGISTER_JSON_MAX_LENGTH);
    sprintf(body_data, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);

    message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/login",
                                "application/json", &body_data, 1, NULL, 0, NULL);
    free(body_data);

    send_to_server(sockfd, message);
    free(message);
    response = receive_from_server(sockfd);

    char *cookie_temp = strstr(response, "Set-Cookie:");
    if (cookie_temp == NULL) {
        cookie_temp = strstr(response, "{\"error\":");
        if (cookie_temp) {
            // get just the error message
            int index = strcspn(cookie_temp, "\n");
            cookie_temp[index] = '\0';
            printf("%s\n", cookie_temp);
            free(response);
            return NULL;
        }
        printf("content-empty from the server\n");
        free(response);
        return NULL;
    }
    char *cookie = malloc(LINELEN);
    sscanf(cookie_temp, "Set-Cookie: %s", cookie);
    cookie[strlen(cookie) - 1] = '\0';
    printf("Login Succeeded\n");
    free(response);
    return cookie;
}

char *send_enter_library(int sockfd, char *cookie) {
    char *message;
    char *response;

    if (cookie) {
        message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access",
                                NULL, &cookie, 1, NULL);
    } else {
        message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access",
                                NULL, NULL, 0, NULL);
    }

    send_to_server(sockfd, message);
    free(message);
    response = receive_from_server(sockfd);

    char *cookie_temp = strstr(response, "{\"token\":\"");
    // might have received an error
    if (cookie_temp == NULL) {
        cookie_temp = strstr(response, "{\"error\":");
        if (cookie_temp) {
            // get just the error message
            int index = strcspn(cookie_temp, "\n");
            cookie_temp[index] = '\0';
            printf("%s\n", cookie_temp);
            free(response);
            return NULL;
        }
        printf("content-empty from the server\n");
        free(response);
        return NULL;
    }
    // get just the token
    int index = strcspn(cookie_temp, "\n");
    cookie_temp[index] = '\0';
    char *token = malloc(LINELEN);

    sscanf(cookie_temp, "{\"token\":\"%s\"}", token);
    // remove " and }
    token[strlen(token) - 2] = '\0';
    printf("Welcome to the library! Here is your token: %s\n", token);
    free(response);
    return token;
}

void send_get_books(int sockfd, char *token) {
    char *message;
    char *response;
    
    message = compute_get_request("34.118.48.238", "/api/v1/tema/library/books",
                                NULL, NULL, 0, token);

    send_to_server(sockfd, message);
    free(message);

    response = receive_from_server(sockfd);

    int statusCode;
    sscanf(response, "%*s %d %*s", &statusCode);
    if (statusCode == STATUS_OK) {
        char *list_temp = strstr(response, "[{\"");
        if (list_temp) { 
            list_temp[strcspn(list_temp, "\n")] = '\0';
            printf("Here is the list of books: %s\n", list_temp);
        } else {
            printf("No books currently available!\n");
        }
    } else if (statusCode == FORBIDDEN_REQUEST) {
        char *error_temp = strstr(response, "{\"error\":");
        if (error_temp) {
            // get just the error message
            error_temp[strcspn(error_temp, "\n")] = '\0';
            printf("%s\n", error_temp);
        }
    } else {
        printf("status code not checked\n");
    }
    free(response);
}

void send_get_book(int sockfd, char *token, int id) {
    char *message;
    char *response;

    char *url = malloc(LINELEN);
    sprintf(url, "/api/v1/tema/library/books/%d", id);
    
    message = compute_get_request("34.118.48.238", url,
                                NULL, NULL, 0, token);

    free(url);
    
    send_to_server(sockfd, message);
    free(message);

    response = receive_from_server(sockfd);

    char *error_temp = strstr(response, "{\"error\":");
    if (error_temp) {
        // get just the error message
        error_temp[strcspn(error_temp, "\n")] = '\0';
        printf("%s\n", error_temp);
    } else {
        printf("Here is your book:\n");
        char *book_temp = strstr(response, "[{\"title");
        if (book_temp) {
            book_temp[strcspn(book_temp, "\n")] = '\0';
            printf("%s\n", book_temp);
        } else {
            printf("Something wrong happened\n");
        }
    }
    free(response);
}

//  send_add_book(sockfd, title, author, genre, publisher, page_count_integer)
void send_add_book(int sockfd, char *title, char *author, char *genre, char *publisher, int page_count_integer, char *token) {
    char *body_data = malloc(MAX_ADD_BOOK_LENGTH);
    char *message;
    char *response;

    sprintf(body_data, "{\"title\":\"%s\",\"author\":\"%s\",\"genre\":\"%s\",\"page_count\":%d,\"publisher\":\"%s\"}",
         title, author, genre, page_count_integer, publisher);

    message = compute_post_request("34.118.48.238", "/api/v1/tema/library/books",
                                "application/json", &body_data, 1, NULL, 0, token);
    free(body_data);

    send_to_server(sockfd, message);
    free(message);
    response = receive_from_server(sockfd);

    int statusCode;
    sscanf(response, "%*s %d %*s", &statusCode);

    if (statusCode == STATUS_OK) {
        printf("Successfuly added book\n");
    } else if (statusCode == FORBIDDEN_REQUEST) {
        char *error_temp = strstr(response, "{\"error\":");
        if (error_temp) {
            // get just the error message
            error_temp[strcspn(error_temp, "\n")] = '\0';
            printf("%s\n", error_temp);
        }
    } else if (statusCode == INTERNAL_SERVER_ERROR){
        printf("Might have an error internally. Are you sure you respected the format requested?\n");
    } else {
        printf("status code not checked\n");
    }

    free(response);
    
    return;
}

void send_delete_book(int sockfd, char *token, int idInteger) {
    char *message;
    char *response;
    char *url = malloc(LINELEN);
    sprintf(url, "/api/v1/tema/library/books/%d", idInteger);
    
    message = compute_delete_request("34.118.48.238", url,
                NULL, NULL, 0, token);
    free(url);
    
    send_to_server(sockfd, message);
    free(message);
    response = receive_from_server(sockfd);

    int statusCode;
    sscanf(response, "%*s %d %*s", &statusCode);

    if (statusCode == STATUS_OK) {
        printf("Successfuly deleted book\n");
    } else if ((statusCode == FORBIDDEN_REQUEST) || (statusCode == STATUS_NOT_FOUND)) {
        char *error_temp = strstr(response, "{\"error\":");
        if (error_temp) {
            // get just the error message
            error_temp[strcspn(error_temp, "\n")] = '\0';
            printf("%s\n", error_temp);
        }
    } else {
        printf("status code not checked\n");
    }

    free(response);    
}

void send_logout(int sockfd, char *cookie) {
    char *message;
    char *response;

    if (cookie) {
        message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout",
                                NULL, &cookie, 1, NULL);
    } else {
        message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout",
                                NULL, NULL, 0, NULL);
    }

    send_to_server(sockfd, message);
    free(message);

    response = receive_from_server(sockfd);

    int statusCode;
    sscanf(response, "%*s %d %*s", &statusCode);

    if (statusCode == STATUS_OK) {
        printf("Successfuly disconnected\n");
    } else if (statusCode == BAD_REQUEST) {
        char *error_temp = strstr(response, "{\"error\":");
        if (error_temp) {
            // get just the error message
            error_temp[strcspn(error_temp, "\n")] = '\0';
            printf("%s\n", error_temp);
        }
    } else {
        printf("Status code not checked\n");
    }

    free(response);
}

int main(int argc, char *argv[]) {
    int sockfd;
    char *cookie = NULL;
    char *token = NULL;

    char str[MAX_INPUT_LENGTH];
    while (fgets(str, MAX_INPUT_LENGTH - 1, stdin) != NULL) {
            str[strlen(str) - 1] = '\0';
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);

            if (strcmp(str, REGISTER_TEXT) == 0) {
                char username[MAX_USERNAME_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                memset(username, 0, MAX_USERNAME_LENGTH);
                memset(password, 0, MAX_PASSWORD_LENGTH);

                printf("username=");
                //scanf("%s", username);
                fgets(username, MAX_USERNAME_LENGTH - 1, stdin);
                username[strlen(username) - 1] = '\0';
                printf("password=");
                fgets(password, MAX_PASSWORD_LENGTH - 1, stdin);
                password[strlen(password) - 1] = '\0';

                send_register(sockfd, username, password);
            } else if (strcmp(str, LOGIN_TEXT) == 0) {
                char username[MAX_USERNAME_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                memset(username, 0, MAX_USERNAME_LENGTH);
                memset(password, 0, MAX_PASSWORD_LENGTH);

                printf("username=");
                fgets(username, MAX_USERNAME_LENGTH - 1, stdin);
                username[strlen(username) - 1] = '\0';
                printf("password=");
                fgets(password, MAX_PASSWORD_LENGTH - 1, stdin);
                password[strlen(password) - 1] = '\0';

                if (cookie) free(cookie);

                cookie = send_login(sockfd, username, password);
            } else if (strcmp(str, ENTER_LIBRARY_TEXT) == 0) {
                if (token) free(token);
                token = send_enter_library(sockfd, cookie);
            } else if (strcmp(str, GET_BOOKS_TEXT) == 0) {
                send_get_books(sockfd, token);
            } else if (strcmp(str, GET_BOOK_TEXT) == 0) {
                char id[MAX_ID_LENGTH];
                printf("id=");
                fgets(id, MAX_ID_LENGTH - 1, stdin);
                int idInteger;
                sscanf(id, "%d", &idInteger);
                send_get_book(sockfd, token, idInteger);
            } else if (strcmp(str, ADD_BOOK_TEXT) == 0) {
                char title[MAX_TITLE_LENGTH];
                char author[MAX_AUTHOR_LENGTH];
                char genre[MAX_GENRE_LENGTH];
                char publisher[MAX_PUBLISHER_LENGTH];
                char page_count[MAX_PAGE_COUNT_LENGTH];
                /*TODO: Fgets and convert for page_count*/
                printf("title=");
                fgets(title, MAX_TITLE_LENGTH - 1, stdin);
                title[strlen(title) - 1] = '\0';
                printf("author=");
                fgets(author, MAX_AUTHOR_LENGTH - 1, stdin);
                author[strlen(author) - 1] = '\0';
                printf("genre=");
                fgets(genre, MAX_GENRE_LENGTH - 1, stdin);
                genre[strlen(genre) - 1] = '\0';
                printf("publisher=");
                fgets(publisher, MAX_PUBLISHER_LENGTH - 1, stdin);
                publisher[strlen(publisher) - 1] = '\0';
                printf("page_count=");
                fgets(page_count, MAX_PAGE_COUNT_LENGTH - 1, stdin);
                int page_count_integer;
                sscanf(page_count, "%d", &page_count_integer);
                send_add_book(sockfd, title, author, genre, publisher, page_count_integer, token);
            } else if (strcmp(str, DELETE_BOOK_TEXT) == 0) {
                char id[MAX_ID_LENGTH];
                printf("id=");
                fgets(id, MAX_ID_LENGTH - 1, stdin);
                int idInteger;
                sscanf(id, "%d", &idInteger);
                send_delete_book(sockfd, token, idInteger);
            } else if (strcmp(str, LOGOUT_TEXT) == 0) {
                send_logout(sockfd, cookie); 
                if (cookie) free(cookie);
                cookie = NULL;
                if (token) free(token);
                token = NULL;
            } else if (strcmp(str, EXIT_TEXT) == 0) {
                close_connection(sockfd);
                if (cookie) free(cookie);
                if (token) free(token);
                return 0;
            } else {
                printf("Input not known\n");
            }
        }
 
    return 0;
}
